#ifndef CALC_H
#define CALC_H

#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "stack.h"

#define EPS 0.0001

//Преобразование инфиксной к постфиксной записи с помощью стека-массива
int infix_to_postfix_arr(char *infix, char *postfix, int *n_push);

//Вычисление выражения с помощью стека-массива
int eval_arr(char *postfix, double *res, int *n_push);

//Преобразование инфиксной к постфиксной записи с помощью стека-списка
int infix_to_postfix_list(char *infix, char *postfix);

//Вычисление выражения с помощью стека-списка
int eval_list(char *postfix, double *res);

#endif 
