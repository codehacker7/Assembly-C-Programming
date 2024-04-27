#include "list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_string(element_t ev)
{
    char *e = (char *)ev;
    printf("%s\n", e);
}
void print_in_line(element_t ev)
{
    char *e = (char *)ev;
    printf("%s ", e);
}

int not_null(element_t sv)
{
    char *s = (char *)sv;
    return s != NULL;
}

void converting(element_t *rv, element_t sv)
{
    char *s = (char *)sv;
    intptr_t *r = (intptr_t *)rv;
    char *e;

    *r = strtol(s, &e, 10);

    if (e == s)
    {
        *r = -1;
    }
}

void set_null(element_t *rv, element_t sv, element_t ev)
{
    char **r = (char **)rv;
    char *s = (char *)sv;
    intptr_t e = (intptr_t)ev;

    if (e < 0)
        *r = s;
    else
        *r = NULL;
}

int set(element_t sv)
{
    intptr_t s = (intptr_t)sv;
    return s >= 0;
}

void max(element_t *rv, element_t sv, element_t ev)
{

    intptr_t *r = (intptr_t *)rv;
    intptr_t s = (intptr_t)sv;
    intptr_t e = (intptr_t)ev;

    if (s >= e)
        *r = s;
    else
        *r = e;
}

void truncate(element_t *rv, element_t sv, element_t ev)
{
    char **r = (char **)rv;
    char *s = (char *)sv;
    intptr_t e = (intptr_t)ev;

    if (strlen(s) > e)
        s[(int)e] = 0;

    *r = s;
}

int main(int argc, char *argv[])
{
    struct list *l0 = list_create();
    for (int i = 1; i < argc; i++)
    {
        list_append(l0, argv[i]);
    }

    struct list *l1 = list_create();
    list_map1(converting, l1, l0);
    struct list *l2 = list_create();
    list_map2(set_null, l2, l0, l1);
    struct list *l3 = list_create();
    list_filter(set, l3, l1);
    struct list *l4 = list_create();
    list_filter(not_null, l4, l2);
    struct list *l5 = list_create();
    list_map2(truncate, l5, l4, l3);
    list_foreach(print_string, l5);
    list_foreach(print_in_line, l5);
    element_t maxVal = 0;
    list_foldl(max, &maxVal, l1);
    printf("\n%d\n", maxVal);
    list_destroy(l0);
    list_destroy(l1);
    list_destroy(l2);
    list_destroy(l3);
    list_destroy(l4);
    list_destroy(l5);
}