#include "sl_list.h"

sl_list *node_init(int data)
{
    sl_list *node = NULL;
    int n = 1, l = sizeof(sl_list);
    node = calloc(n, l);
    if (node != NULL)
        node->data = data;
    return node;
}

sl_list_free *node_init_free(sl_list *data)
{
    sl_list_free *node = calloc(1, sizeof(sl_list_free));
    if (node != NULL)
        node->data = data;
    return node;
}


void free_node(sl_list *node)
{
    free(node);
}

void free_list(sl_list *head)
{
    sl_list *next = NULL;
    for (; head; head = next)
    {
        next = head->next;
        free_node(head);
    }
}

void free_node_free(sl_list_free *node)
{
    free(node);
}

void free_list_free(sl_list_free *head)
{
    sl_list_free *next = NULL;
    for (; head; head = next)
    {
        next = head->next;
        free_node_free(head);
    }
}

sl_list_free *push_back_free(sl_list_free *head, sl_list_free *node)
{
    sl_list_free *cur = head;
    if (!head)
        return node;
    for (; cur->next; cur = cur->next);
    cur->next = node;
    return head;
}

sl_list *push_back(sl_list *end, sl_list *node)
{
    if (end != NULL)
        end->next = node;
    return node;
}

sl_list *pop_front(sl_list *begin, int *res, sl_list_free **listFree)
{
    if (begin == NULL)
        return NULL;
    sl_list *new_begin = begin->next;
    *res = begin->data;
    if (listFree != NULL)
    {
        sl_list_free *cur = node_init_free(begin);
        *listFree = push_back_free(*listFree, cur);
    }
    free_node(begin);
    return new_begin;
}



//void remove_duplicates(sl_list **head, int (*comparotor)(const void *, const void *))
//{
//    if (!(*head))
//        return;
//    for (sl_list *cur = *head; cur; cur = cur->next)
//    {
//        sl_list *prev = cur;
//        for (sl_list *cpy = cur->next; cpy; cpy = cpy->next)
//        {
//            if (comparotor(cur, cpy) == 0)
//            {
//                prev->next = cpy->next;
//                free_node(cpy);
//                cpy = prev;
//            }
//            prev = cpy;
//        }
//    }
//}


