#include "stack.h"

void list_stack_free(stack_list *stack)
{
    token_list_free(stack->list);
    stack->list = NULL;
}

//void list_free_mem_free(stack_list *stack)
//{
//    token_list_free(stack->list);
//}

int list_push(stack_list *stack, char *token)
{
    sl_list_token *node = token_create_node(token);
    if (!node)
        return MEM_ERROR;
    stack->list = token_add_to_top(stack->list, node);
    return NO_ERROR;
}

int list_pop(stack_list *stack, char *token, sl_list_free **free_list)
{
    if (stack->list == NULL)
        return POP_EMPTY_STACK_ERROR;
    char *res = token_get_top_elem(stack->list);
    strcpy(token, res);
    if(free_list != NULL)
    {
        sl_list_token *tmp_p = token_get_top_address(stack->list);
        sl_list_free *node = free_create_node(tmp_p);
        if (!node)
            return MEM_ERROR;
        (*free_list) = free_add_to_end((*free_list), node);
    }
    stack->list = token_del_top(stack->list);
    return NO_ERROR;
}

void print_stack(stack_list *stack)
{
    if(stack->list == NULL)
    {
        printf("Empty stack\n");
        return;
    }
    token_print_list(stack->list);
}

void print_free_mem(sl_list_free *free_list)
{
    free_print_list(free_list);
}

//arr list

int arr_push(stack_arr *stack, char *token)
{
    if(stack->n == ARR_LEN)
        return ARR_OVERFLOW;
    strcpy(stack->arr[stack->n], token);
    stack->n++;
    return NO_ERROR;
}

int arr_pop(stack_arr *stack, char *token)
{
    if(stack->n == 0)
        return POP_EMPTY_STACK_ERROR;
    stack->n--;
    strcpy(token, stack->arr[stack->n]);
    return NO_ERROR;
}

void print_stack_arr(stack_arr *stack)
{
    if(stack->n == 0)
    {
        printf("Empty stack\n");
        return;
    }
    for (int i = 0; i < stack->n; i++)
        printf("Value %s\n", stack->arr[i]);
}