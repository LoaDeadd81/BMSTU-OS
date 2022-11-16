#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

#define LOCKFILE "/var/run/daemonos .pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

// journalctl | grep daemonos

sigset_t mask;

int lockfile(int fd)
{
    struct flock fl;

    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}

int already_running(void)
{
    int fd;
    char buf[16];

    fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
    if (fd < 0)
    {
        syslog(LOG_ERR, "unable to open %s: %s",
               LOCKFILE, strerror(errno));
        exit(1);
    }

    if (lockfile(fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            close(fd);
            return (1);
        }
        syslog(LOG_ERR, "it is not possible to set a lock on %s: %s",
               LOCKFILE, strerror(errno));
        exit(1);
    }

    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long) getpid());
    write(fd, buf, strlen(buf) + 1);
    return (0);
}


void daemonize(const char *cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        perror("it is not possible to get the maximum handle number");

    if ((pid = fork()) < 0)
        perror("function call error fork");
    else if (pid != 0)
        exit(0);

    if (setsid() == -1)
    {
        syslog(LOG_ERR, "it is not possible to make a process a session and group leader: %s", strerror(errno));
        exit(1);
    }

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        perror("it is impossible to ignore the signal SIGHUP");

    if (chdir("/") < 0)
        perror("it is not possible to make the current working directory");

    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "erroneous file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}

void *thr_fn(void *arg)
{
    int err, signo;

    for (;;)
    {
        err = sigwait(&mask, &signo);
        if (err != 0)
        {
            syslog(LOG_ERR, "function call error sigwait");
            exit(1);
        }
        switch (signo)
        {
            case SIGHUP:
                syslog(LOG_INFO, "reading the configuration file");
                break;
            case SIGTERM:
                syslog(LOG_INFO, "received SIGTERM; exit");
                exit(0);
            default:
                syslog(LOG_INFO, "an unexpected signal was received %d\n", signo);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    time_t t;
    int err;
    pthread_t tid;
    char *cmd;
    struct sigaction sa;

    if ((cmd = strrchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else
        cmd++;

    daemonize(cmd);

    if (already_running())
    {
        syslog(LOG_ERR, "%s: the daemon is already running", cmd);
        exit(1);
    }

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGHUP, &sa, NULL) == -1)
    {
        syslog(LOG_ERR, "the action cannot be restored SIG_DFL for SIGHUP");
        exit(1);
    }
    sigfillset(&mask);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
    {
        syslog(LOG_ERR, "operation execution error SIG_BLOCK");
        exit(1);
    }

    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0)
    {
        syslog(LOG_ERR, "unable to create a stream");
        exit(1);
    }

    while (1)
    {
        t = time(NULL);
        struct tm tm = *localtime(&t);
        syslog(LOG_INFO, "%s: Current time is: %02d:%02d:%02d\n",
               cmd, tm.tm_hour, tm.tm_min, tm.tm_sec);
        sleep(1);
    }
    return 0;
}