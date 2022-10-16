#ifndef STACK_LIST_H
#define STACK_LIST_H

#include <string.h>

#include "sl_list.h"
#include "const.h"


typedef struct stack_list stack_list;

typedef struct stack_arr stack_arr;

struct stack_list
{
    sl_list_token *list;
};

struct stack_arr
{
    char arr[ARR_LEN][VAL_LEN + 1];
    int n;
};

//Освобождение памяти стека-списка
void list_stack_free(stack_list *stack);

//Добавление элемента в стек-список
int list_push(stack_list *stack, char *token);

//Удаление элемента из стека-списка
int list_pop(stack_list *stack, char *token, sl_list_free **free_list);

//Вывод стека-списка
void print_stack(stack_list *stack);


//Добавление элемента в стек-массив
int arr_push(stack_arr *stack, char *token);

//Удаление элемента из стека-массива
int arr_pop(stack_arr *stack, char *token);

//Вывод массива
void print_stack_arr(stack_arr *stack);

#endif 
