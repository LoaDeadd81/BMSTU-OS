#include "calc.h"

void clean(char **arr, int *n)
{
    for (int i = 0; i < *n - 1; i++)
        free(arr[i]);
    free(arr);
}

int add_str(char **arr, char *str, int *n)
{
    arr[*n] = calloc(sizeof(str + 1), sizeof(char));
    if (!arr[*n])
        return MEM_ERROR;
    strcpy(arr[*n], str);
    (*n)++;
    return NO_ERROR;
}

int split(char *inp, char ***out, int *n)
{
    *n = 0;
    (*out) = calloc(100, sizeof(char *));
    if (!(*out))
        return MEM_ERROR;
    int mem_size = 100;
    char cur_elem[VAL_LEN + 1];
    memset(cur_elem, 0, sizeof(char) * (VAL_LEN + 1));
    int cur_i = 0, dot_cnt = 0, post_i = 0, rc = 0;
    char ch = inp[0];
    for (int i = 0; i < strlen(inp); i++)
    {
        ch = inp[i];
        if (isspace(ch))
            continue;
        if (strchr("0123456789.", ch) != NULL)
        {
            while (strchr("0123456789.", ch) != NULL)
            {
                if (i == strlen(inp))
                    break;
                if (ch == '.')
                    dot_cnt++;
                if (dot_cnt > 1)
                {
                    clean((*out), n);
                    return WRONG_DATA;
                }
                cur_elem[cur_i++] = ch;
                ch = inp[++i];
            }
            rc = add_str((*out), cur_elem, n);
            if (rc)
            {
                clean((*out), n);
                return MEM_ERROR;
            }
            memset(cur_elem, 0, sizeof(char) * (VAL_LEN + 1));
            cur_i = 0;
            dot_cnt = 0;
            i--;
        }
        else if (strchr("+-*/()", ch) != NULL)
        {
            rc = add_str((*out), &ch, n);
            if (rc)
            {
                clean((*out), n);
                return MEM_ERROR;
            }
        }
        else
        {
            clean((*out), n);
            return WRONG_DATA;
        }
        if (*n >= mem_size)
        {
            mem_size *= 2;
            char **tmp = realloc((*out), sizeof(char *) * mem_size);
            if (!tmp)
            {
                clean((*out), n);
                return MEM_ERROR;
            }
            (*out) = tmp;
        }

    }
    char **tmp = realloc((*out), sizeof(char *) * (*n));
    if (!tmp)
    {
        clean((*out), n);
        return MEM_ERROR;
    }
    (*out) = tmp;
    return NO_ERROR;
}

int infix_to_postfix_arr(char *infix, char *postfix, int *n_push)
{
    stack_arr stack;
    memset(&stack, 0, sizeof(stack));
    char **out = NULL;
    int n = 0, pst_i = 0;
    int rc = split(infix, &out, &n);
    if (rc)
        return rc;
    for (int i = 0; i < n; i++)
    {
        if (isdigit(out[i][0]))
        {
            for (int j = 0; j < strlen(out[i]); j++)
                postfix[pst_i++] = out[i][j];
            postfix[pst_i++] = ' ';
            continue;
        }
        switch (out[i][0])
        {
            case '+':
            case '-':
            case '*':
            case '/':
            {
                char tmp = 0;
                char cur = out[i][0];
                int tmp_n = n;
                rc = arr_pop(&stack, &tmp);
                n = tmp_n;
                if (rc != 2)
                {
                    if (rc)
                    {
                        clean(out, &n);
                        return rc;
                    }
                    if ((cur == '+' || cur == '-') && (tmp == '*' || tmp == '/' || tmp == '+' || tmp == '-'))
                    {
                        postfix[pst_i++] = tmp;
                        postfix[pst_i++] = ' ';
                    }
                    else
                    {
                        rc = arr_push(&stack, &tmp);
                        if (n_push != NULL)
                            *n_push += sizeof(cur) + sizeof(char *);
                        if (rc)
                        {
                            clean(out, &n);
                            return rc;
                        }
                    }
                }
                rc = arr_push(&stack, &cur);
                if (n_push != NULL)
                    *n_push += sizeof(cur) + sizeof(char *);
                if (rc)
                {
                    clean(out, &n);
                    return rc;
                }
                break;
            }
            case '(':
                rc = arr_push(&stack, &out[i][0]);
                if (n_push != NULL)
                    *n_push += sizeof(out[i][0]) + sizeof(char *);
                if (rc)
                {
                    clean(out, &n);
                    return rc;
                }
                break;
            case ')':
                while (1)
                {
                    char tmp = 0;
                    int tmp_n = n;
                    rc = arr_pop(&stack, &tmp);
                    n = tmp_n;
                    if (rc)
                    {
                        clean(out, &n);
                        return rc;
                    }
                    if (tmp == '(')
                        break;
                    postfix[pst_i++] = tmp;
                    postfix[pst_i++] = ' ';
                }
                break;
            default:
                clean(out, &n);
                return WRONG_DATA;
        }
    }
    while (1)
    {
        char tmp = 0;
        int tmp_n = n;
        rc = arr_pop(&stack, &tmp);
        n = tmp_n;
        if (rc == POP_EMPTY_STACK_ERROR)
            break;
        if (rc)
        {
            clean(out, &n);
            return rc;
        }
        postfix[pst_i++] = tmp;
        postfix[pst_i++] = ' ';
    }
    clean(out, &n);
    return NO_ERROR;

}

int eval_arr(char *postfix, double *res, int *n_push)
{
    stack_arr stack;
    memset(&stack, 0, sizeof(stack));
    char **out = NULL;
    int n = 0;
    int rc = split(postfix, &out, &n);
    if (rc)
        return rc;
    if (n < 1)
    {
        clean(out, &n);
        return WRONG_DATA;
    }
    for (int i = 0; i < n; i++)
    {
        if (isdigit(out[i][0]))
        {
            rc = arr_push(&stack, out[i]);
            if (n_push != NULL)
                *n_push += sizeof(out[i]) + sizeof(char *);
            if (rc)
            {
                clean(out, &n);
                return rc;
            }
            continue;
        }
        char cur = out[i][0];
        if (strchr("+-*/", cur) == NULL)
        {
            clean(out, &n);
            return WRONG_DATA;
        }
        int tmp_n = n;
        char op1_c[VAL_LEN + 1], op2_c[VAL_LEN + 1];
        memset(&op1_c, 0, sizeof(char) * (VAL_LEN + 1));
        memset(&op2_c, 0, sizeof(char) * (VAL_LEN + 1));
        rc = arr_pop(&stack, op1_c);
        n = tmp_n;
        if (rc)
        {
            clean(out, &n);
            return rc;
        }
        rc = arr_pop(&stack, op2_c);
        n = tmp_n;
        if (rc)
        {
            clean(out, &n);
            return rc;
        }
        double op1_d = 0, op2_d = 0;
        op1_d = atof(op1_c);
        op2_d = atof(op2_c);
        char resl[VAL_LEN + 1];
        memset(&resl, 0, sizeof(char) * (VAL_LEN + 1));
        switch (cur)
        {
            case '+':
                sprintf(resl, "%lf", op2_d + op1_d);
                arr_push(&stack, resl);
                if (n_push != NULL)
                    *n_push += sizeof(resl) + sizeof(char *);
                break;
            case '-':
                sprintf(resl, "%lf", op2_d - op1_d);
                arr_push(&stack, resl);
                if (n_push != NULL)
                    *n_push += sizeof(resl) + sizeof(char *);
                break;
            case '*':
                sprintf(resl, "%lf", op2_d * op1_d);
                arr_push(&stack, resl);
                if (n_push != NULL)
                    *n_push += sizeof(resl) + sizeof(char *);
                break;
            case '/':
                if (fabs(op1_d) < EPS)
                {
                    clean(out, &n);
                    return ZERO_DIV;
                }
                sprintf(resl, "%lf", op2_d / op1_d);
                arr_push(&stack, resl);
                if (n_push != NULL)
                    *n_push += sizeof(resl) + sizeof(char *);
                break;
            default:
                break;
        }
    }
    char res_c[VAL_LEN + 1];
    memset(&res_c, 0, sizeof(char) * (VAL_LEN + 1));
    int tmp_n = n;
    rc = arr_pop(&stack, res_c);
    n = tmp_n;
    if (rc)
    {
        clean(out, &n);
        return rc;
    }
    *res = atof(res_c);
    clean(out, &n);
    return NO_ERROR;
}

int infix_to_postfix_list(char *infix, char *postfix)
{
    int max = 0;
    stack_list stack;
    memset(&stack, 0, sizeof(stack));
    char **out = NULL;
    int n = 0, pst_i = 0;
    int rc = split(infix, &out, &n);
    if (rc)
        return rc;
    for (int i = 0; i < n; i++)
    {
        if (isdigit(out[i][0]))
        {
            for (int j = 0; j < strlen(out[i]); j++)
                postfix[pst_i++] = out[i][j];
            postfix[pst_i++] = ' ';
            continue;
        }
        switch (out[i][0])
        {
            case '+':
            case '-':
            case '*':
            case '/':
            {
                char tmp = 0;
                char cur = out[i][0];
                int tmp_n = n;
                rc = list_pop(&stack, &tmp, NULL);
                n = tmp_n;
                if (rc != 2)
                {
                    if (rc)
                    {
                        clean(out, &n);
                        list_stack_free(&stack);
                        return rc;
                    }
                    if ((cur == '+' || cur == '-') && (tmp == '*' || tmp == '/' || tmp == '+' || tmp == '-'))
                    {
                        postfix[pst_i++] = tmp;
                        postfix[pst_i++] = ' ';
                    }
                    else
                    {
                        rc = list_push(&stack, &tmp);
                        if (rc)
                        {
                            clean(out, &n);
                            list_stack_free(&stack);
                            return rc;
                        }
                    }
                }
                rc = list_push(&stack, &cur);
                if (rc)
                {
                    clean(out, &n);
                    list_stack_free(&stack);
                    return rc;
                }
                break;
            }
            case '(':
                rc = list_push(&stack, &out[i][0]);
                if (rc)
                {
                    clean(out, &n);
                    list_stack_free(&stack);
                    return rc;
                }
                break;
            case ')':
                while (1)
                {
                    char tmp = 0;
                    int tmp_n = n;
                    rc = list_pop(&stack, &tmp, NULL);
                    n = tmp_n;
                    if (rc)
                    {
                        clean(out, &n);
                        list_stack_free(&stack);
                        return rc;
                    }
                    if (tmp == '(')
                        break;
                    postfix[pst_i++] = tmp;
                    postfix[pst_i++] = ' ';
                }
                break;
            default:
                clean(out, &n);
                list_stack_free(&stack);
                return WRONG_DATA;
        }
    }
    while (1)
    {
        char tmp = 0;
        int tmp_n = n;
        rc = list_pop(&stack, &tmp, NULL);
        n = tmp_n;
        if (rc == POP_EMPTY_STACK_ERROR)
            break;
        if (rc)
        {
            clean(out, &n);
            list_stack_free(&stack);
            return rc;
        }
        postfix[pst_i++] = tmp;
        postfix[pst_i++] = ' ';
    }
    clean(out, &n);
    return NO_ERROR;
}

int eval_list(char *postfix, double *res)
{
    int max = 0;
    stack_list stack;
    memset(&stack, 0, sizeof(stack));
    char **out = NULL;
    int n = 0;
    int rc = split(postfix, &out, &n);
    if (rc)
        return rc;
    if (n < 1)
    {
        clean(out, &n);
        return WRONG_DATA;
    }
    for (int i = 0; i < n; i++)
    {
        if (isdigit(out[i][0]))
        {
            rc = list_push(&stack, out[i]);
            if (rc)
            {
                clean(out, &n);
                return rc;
            }
            continue;
        }
        char cur = out[i][0];
        if (strchr("+-*/", cur) == NULL)
        {
            clean(out, &n);
            return WRONG_DATA;
        }
        int tmp_n = n;
        char op1_c[VAL_LEN + 1], op2_c[VAL_LEN + 1];
        memset(&op1_c, 0, sizeof(char) * (VAL_LEN + 1));
        memset(&op2_c, 0, sizeof(char) * (VAL_LEN + 1));
        rc = list_pop(&stack, op1_c, NULL);
        n = tmp_n;
        if (rc)
        {
            clean(out, &n);
            list_stack_free(&stack);
            return rc;
        }
        rc = list_pop(&stack, op2_c, NULL);
        n = tmp_n;
        if (rc)
        {
            clean(out, &n);
            list_stack_free(&stack);
            return rc;
        }
        double op1_d = 0, op2_d = 0;
        op1_d = atof(op1_c);
        op2_d = atof(op2_c);
        char resl[VAL_LEN + 1];
        memset(&resl, 0, sizeof(char) * (VAL_LEN + 1));
        switch (cur)
        {
            case '+':
                sprintf(resl, "%lf", op2_d + op1_d);
                break;
            case '-':
                sprintf(resl, "%lf", op2_d - op1_d);
                break;
            case '*':
                sprintf(resl, "%lf", op2_d * op1_d);
                break;
            case '/':
                if (fabs(op1_d) < EPS)
                {
                    clean(out, &n);
                    list_stack_free(&stack);
                    return ZERO_DIV;
                }
                sprintf(resl, "%lf", op2_d / op1_d);
                break;
            default:
                break;
        }
        list_push(&stack, resl);
    }
    char res_c[VAL_LEN + 1];
    memset(&res_c, 0, sizeof(char) * (VAL_LEN + 1));
    int tmp_n = n;
    rc = list_pop(&stack, res_c, NULL);
    n = tmp_n;
    if (rc)
    {
        clean(out, &n);
        list_stack_free(&stack);
        return rc;
    }
    if (stack.list != NULL)
    {
        clean(out, &n);
        list_stack_free(&stack);
        return WRONG_DATA;
    }
    *res = atof(res_c);
    clean(out, &n);
    return NO_ERROR;
}
