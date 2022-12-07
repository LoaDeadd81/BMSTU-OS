#ifndef LAB_05_RW_H
#define LAB_05_RW_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <time.h>

#define N 1

#define N_READERS 4
#define N_WRITERS 3

#define ACTIVE_READERS 0
#define CAN_WRITE 1
#define CAN_READ 2
#define WAITING_WRITERS 3
#define BIN_SEM 4

int reader_run(int *const shared_mem, const int s_id, const int reader_id);

int writer_run(int *const shared_mem, const int s_id, const int writer_id);

#endif
