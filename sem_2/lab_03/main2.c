#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "stack/const.h"

#define BUF_SIZE 10000

#define NO_ACCESS 1
#define BAD_ARGS 2


void print_maps(const int pid) {
    char *line;
    int start_addr, end_addr, page_size = 4096;
    size_t line_size;
    ssize_t line_length;

    char pathToOpen[PATH_MAX];
    snprintf(pathToOpen, PATH_MAX, "/proc/%d/maps", pid);
    char buf[BUF_SIZE] = {'\0'};
    FILE *file = fopen(pathToOpen, "r");
    printf("\nMAPS:\n");
    int lengthOfRead;
    do
    {
        line_length = getline(&line, &line_size, file);
        if (!feof(file) && line_length == -1)
        {
            perror("getline():");
            fclose(file);
            free(line);
            exit(1);
        }
        sscanf(line, "%x-%x", &start_addr, &end_addr);
        printf("%d\t%s", (end_addr - start_addr) / page_size,
               line);
    } while (!feof(file));
    fclose(file);
}

int get_pid(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Введите pid процесса для исследования\n");
        exit(BAD_ARGS);
    }

    int pid = atoi(argv[1]);
    char check_dir[PATH_MAX];
    snprintf(check_dir, PATH_MAX, "/proc/%d", pid);
    if (!pid || access(check_dir, F_OK)) {
        printf("Директории для введенного pid не найдено\n");
        exit(NO_ACCESS);
    }

    return pid;
}


int main(int argc, char *argv[]) {
    int pid = get_pid(argc, argv);

    print_maps(pid);

    return 0;
}