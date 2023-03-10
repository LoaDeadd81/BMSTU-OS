#include "sl_list.h"

sl_list_token * token_create_node(char *token)
{
    if (strlen(token) > VAL_LEN)
        return NULL;
    sl_list_token *node = calloc(1, sizeof(sl_list_token));
    if (!node)
        return NULL;
    memset(node, 0, sizeof(sl_list_token));
    strcpy(node->val, token);
    node->next = NULL;
    return node;
}

void token_free_node(sl_list_token *node)
{
    free(node);
}

void token_print_node(struct sl_list_token *node)
{
    printf("Address: %p\n", node);
    printf("Value: %s\n", node->val);
//    printf("Next address: %p\n", node->next);
}

void token_list_free(sl_list_token *head)
{
    sl_list_token *next;
    for (; head; head = next)
    {
        next = head->next;
        token_free_node(head);
    }
}

sl_list_token *token_add_to_top(sl_list_token *head, sl_list_token *node)
{
    sl_list_token *prev = head;
    head = node;
    if (!head)
    {
        head->next = NULL;
    }
    else
        node->next = prev;
    return head;
}

char *token_get_top_elem(sl_list_token *head)
{
    return head->val;
}

sl_list_token *token_get_top_address(sl_list_token *head)
{
    return head;
}

sl_list_token *token_del_top(sl_list_token *head)
{
    sl_list_token *prev = head->next;
    if (head == NULL)
        return NULL;
    token_free_node(head);
    if (prev == NULL)
        head = NULL;
    else
        head = prev;
    return head;
}

void token_print_list(sl_list_token *head)
{
    if (!head)
        return;
    for (; head; head = head->next)
    {
        token_print_node(head);
        printf("\n");
    }
}

//free list

void free_free_node(sl_list_free *node)
{
    free(node);
}

void free_print_node(struct sl_list_free *node)
{
    printf("Address: %p\n", node->mem);
}

sl_list_free *free_create_node(sl_list_token *mem)
{
    sl_list_free *node = calloc(1, sizeof(sl_list_free));
    if (!node)
        return NULL;
    node->mem = mem;
    node->next = NULL;

    return node;
}

void free_list_free(sl_list_free *head)
{
    sl_list_free *first = head;
    sl_list_free *next;
    for (; head; head = next)
    {
        next = head->next;
        free_free_node(head);
    }
    first = NULL;
}

sl_list_free *free_add_to_end(sl_list_free *head, sl_list_free *node)
{
    sl_list_free *cur = head;
    if (!head)
        head = node;
    else
    {
        for (; cur->next; cur = cur->next);
        cur->next = node;
    }
    return head;
}

void free_print_list(struct sl_list_free *head)
{
    if (!head)
    {
        printf("Empty list\n");
        return;
    }
    for (; head; head = head->next)
    {
        free_print_node(head);
        printf("\n");
    }
}