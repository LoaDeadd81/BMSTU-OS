#include "queues_io.h"

void print_arr(queue_arr *arr)
{
    if(arr->is_not_empty == 0)
    {
        printf("Empty queue\n");
        return;
    }
    for (int i = arr->pout; i != arr->pin; i = (i + 1) % ARR_SIZE)
        printf("%d ", arr->arr[i]);
    printf("\n");
}

void print_list(queue_list *list)
{
    if(list == NULL)
        printf("Empty queue\n");
    sl_list *cur = list->begin;
    for (; cur; cur = cur->next)
    {
        printf("%d ", cur->data);
    }
    printf("\n");
}

void print_list_free(sl_list_free *head)
{
    if (head == NULL)
        printf("Empty begin\n");
    for (; head; head = head->next)
        printf("%p\n ", head->data);
}