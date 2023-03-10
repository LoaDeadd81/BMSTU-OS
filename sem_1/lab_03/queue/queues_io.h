#ifndef DTS_QUEUES_IO_H
#define DTS_QUEUES_IO_H

#include <stdio.h>

#include "queues.h"

//Вывод очереди-массива
void print_arr(queue_arr *arr);

//Вывод очереди-списка
void print_list(queue_list *list);

//Вывод свободной памяти
void print_list_free(sl_list_free *head);

#endif 
