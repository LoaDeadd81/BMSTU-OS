#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "err.h"
#include "const.h"

typedef struct sl_list_token sl_list_token;

typedef struct sl_list_free sl_list_free;

struct sl_list_token
{
    char val[VAL_LEN + 1];
    sl_list_token *next;
};

struct sl_list_free
{
    sl_list_token *mem;
    sl_list_free *next;
};

//Освобождение списка
void token_list_free(sl_list_token *head);

//Создание узла списка
sl_list_token *token_create_node(char *token);

//Добавление узла в конец списка
sl_list_token *token_add_to_top(sl_list_token *head, sl_list_token *node);

//Получение последнего элемента списка
char *token_get_top_elem(sl_list_token *head);

//Получение адреса последнего элемента
sl_list_token *token_get_top_address(sl_list_token *head);

//Удаление последнего элемента списка
sl_list_token *token_del_top(sl_list_token *head);

//Вывод списка
void token_print_list(sl_list_token *head);

//Освобождение списка свободной памяти
void free_list_free(sl_list_free *head);

//Создание узла списка свободной памяти
sl_list_free *free_create_node(sl_list_token *mem);

//Добавления элемента в список свободной памяти
sl_list_free *free_add_to_end(sl_list_free *head, sl_list_free *node);

//Вывод списка свободной памяти
void free_print_list(struct sl_list_free *head);

#endif 
