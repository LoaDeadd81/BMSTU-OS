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

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

// journalctl | grep daemonos

sigset_t mask;

int lockfile(int fd)
{
    struct flock fl; // структура для управления блокировкой

    fl.l_type = F_WRLCK; // блокировка на запись
    fl.l_start = 0;  // определяет смещение начала области, к которой применяется блокировка, и задается в байтах относительно l_whence.
    fl.l_whence = SEEK_SET; // начало файла
    fl.l_len = 0; // длина блокируемого участка (при 0 блокируется область до конца файла)
    return(fcntl(fd, F_SETLK, &fl)); // блокировка
}

int already_running(void)
{
    int fd;
    char buf[16];

    // создаём файл, если не существует, и окрываем на чтение и запись
    fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
    if (fd < 0) {
        syslog(LOG_ERR, "unable to open %s: %s",
               LOCKFILE, strerror(errno));
        exit(1);
    }

    //установка и проверка блокировки файла
    if (lockfile(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) { // файл заблокирован (демон уже запущен)
            close(fd);
            return(1);
        }
        syslog(LOG_ERR, "it is not possible to set a lock on %s: %s",
               LOCKFILE, strerror(errno));
        exit(1);
    }

    //пишем в файл pid демона
    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf)+1);
    return(0);
}

void daemonize(const char *cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;
    /*
    * 1. Сбросить маску режима создания файла.
    */
    umask(0);

    /*
    * Получить максимально возможный номер дескриптора файла.
    */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        perror("it is not possible to get the maximum handle number");
    /*
    * 2. Стать лидером нового сеанса, чтобы утратить управляющий терминал.
    */
    if ((pid = fork()) < 0)
        perror("function call error fork");
    else if (pid != 0) /* родительский процесс */
        exit(0);

    /*
    * 3. Вызвать setsid()
    */

    if (setsid() == -1)
    {
        syslog(LOG_ERR, "it is not possible to make a process a session and group leader: %s", strerror(errno));
        exit(1);
    }
    /*
    * Обеспечить невозможность обретения управляющего терминала в будущем.
    */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        perror("it is impossible to ignore the signal SIGHUP");

    /*
    * 4. Назначить корневой каталог текущим рабочим каталогом,
    * чтобы впоследствии можно было отмонтировать файловую систему.
    */
    if (chdir("/") < 0)
        perror("it is not possible to make the current working directory");
    /*
    * 5. Закрыть все открытые файловые дескрипторы.
    */
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);
    /*
    * Присоединить файловые дескрипторы 0, 1 и 2 к /dev/null.
    */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    /*
    * 6. Инициализировать файл журнала.
    */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "erroneous file descriptors %d %d %d",fd0, fd1, fd2);
        exit(1);
    }
}

void *thr_fn(void *arg)
{
    int err, signo;


    // ждём сигнал и обрабатываем
    for (;;) {
        err = sigwait(&mask, &signo);
        if (err != 0) {
            syslog(LOG_ERR, "function call error sigwait");
            exit(1);
        }
        switch (signo) {
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

    // убирает '/' из пути
    if ((cmd = strrchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else
        cmd++;

    daemonize(cmd);     // демонизируем

    // проверяет не запущен ли уже демон (через блокировку файла)
    if (already_running()) {
        syslog(LOG_ERR, "%s: the daemon is already running", cmd);
        exit(1);
    }

    sa.sa_handler = SIG_DFL; // SIG_DFL для выполнения стандартных действий
    sigemptyset(&sa.sa_mask); // очищает маску (блокирует все сигналы)
    sa.sa_flags = 0;
    //настраваем для SIGHUP (по умолчанию и блок всез сигналов)
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        syslog(LOG_ERR, "the action cannot be restored SIG_DFL for SIGHUP");
        exit(1);
    }
    sigfillset(&mask); // разрешает все сигналы
    // устанавливаем синальную маску для потоков(cur union mask)
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0) {
        syslog(LOG_ERR, "operation execution error SIG_BLOCK");
        exit(1);
    }

    //создаём новый поток tid для thr_fn
    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0) {
        syslog(LOG_ERR, "unable to create a stream");
        exit(1);
    }

    while (1) {
        t = time(NULL);
        struct tm tm = *localtime(&t);
        syslog(LOG_INFO, "%s: Current time is: %02d:%02d:%02d\n",
               cmd, tm.tm_hour, tm.tm_min, tm.tm_sec);
        sleep(1);
    }
    return 0;
}