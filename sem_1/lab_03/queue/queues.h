#ifndef QUEUES_H
#define QUEUES_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "err.h"
#include "sl_list.h"

#define ARR_SIZE 10000
#define EPS 0.0001

typedef struct queue_arr queue_arr;
typedef struct queue_list queue_list;

struct queue_arr
{
    int arr[ARR_SIZE];
    int pin;
    int pout;
    int is_not_empty;
};

struct queue_list
{
    sl_list *begin;
    sl_list *end;
};

#include "queues_io.h"

//Очистка очереди-списка
void queue_list_free(queue_list* queue);

//Добавление элемента в очередь-массив
int queue_arr_push(queue_arr *queue, int data);

//Удаление элемента из очереди-массива
int queue_arr_pop(queue_arr *queue, int *res);

//Добавление элемента в очередь-список
int queue_list_push(queue_list *queue, int data);

//Удаление элемента из очереди-списка
int queue_list_pop(queue_list *queue, int *res, sl_list_free **listFree);

//Симуляция с очередью-массивом
int sim_arr_strap();

//Симуляция с очередью-списком
int sim_list_strap();

int measure(int n, int iter);

#endif 
