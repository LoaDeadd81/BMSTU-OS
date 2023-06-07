#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd1 = open("out.txt", O_RDWR);
    int fd2 = open("out.txt", O_RDWR);

    for (char letr = 'a'; letr < '{'; letr++) {
        letr % 2 ? write(fd2, &letr, 1) : write(fd1, &letr, 1);
    }

    close(fd2);
    close(fd1);

    return 0;
}