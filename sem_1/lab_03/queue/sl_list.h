#ifndef SL_LIST_H
#define SL_LIST_H

#include <stdlib.h>

typedef struct sl_list sl_list;
typedef struct sl_list_free sl_list_free;

struct sl_list
{
    int data;
    sl_list *next;
};

struct sl_list_free
{
    sl_list *data;
    sl_list_free *next;
};

//Инициализация элемента списка
sl_list *node_init(int data);

//Высвобождение списка
void free_list(sl_list *head);

//Высвобождение списка свободной памяти
void free_list_free(sl_list_free *head);

//Добавление в конец списка
sl_list *push_back(sl_list *end, sl_list *node);

//Удаление из начала
sl_list *pop_front(sl_list *begin, int *res, sl_list_free **listFree);

#endif 
