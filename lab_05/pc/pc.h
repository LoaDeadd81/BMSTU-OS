#ifndef LAB_05_PC_H
#define LAB_05_PC_H

#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <time.h>

#define MAX_CONSUMER_DELAY_TIME 7
#define MAX_PRODUCER_DELAY_TIME 4

#define N 24
#define ITERS_NUM 8
#define PROD_NUM 3
#define CONS_NUM 3

#define BUF_FULL  0
#define BUF_EMPTY 1
#define BIN_SEM   2

void create_consumer(char **ptr, const int sid, const int cid);

void create_producer(char **ptr, char *ch, const int sid, const int pdid);

#endif
