#include "list.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>

#define T List_T

T List_append(T list, T tail)
{
    T* p = &list;

    while (p)
        p = &(*p)->rest;
    *p = tail;

    return list;
}

T List_copy(T list)
{
    T head, *p = head;

    for (; list; list = list->rest) {
        *p = (T)malloc(sizeof(**p));
        (*p)->first = list->first;
        p = &(*p)->rest;
    }
    *p = NULL;
    return head;
}

T List_list(void* x, ...)
{
    va_list ap;

    T list, *p = &list;

    va_start(ap, x);
    for (; x; x = va_arg(ap, void*)) {
        *p = (T)malloc(sizeof(**p));
        (*p)->first = x;
        p = &(*p)->rest;
    }
    *p = NULL;
    va_end(ap);

    return list;
}

T List_pop(T list, void** x)
{
    if (list) {
        T head = list->rest;
        if (x) {
            *x = list->first;
            free(list);
            return head;
        }
    } else
        return list;
}

T List_push(T list, void* x)
{
    T p;

    p = (T)malloc(sizeof(*p));

    p->first = x;
    p->rest = list;

    return p;
}

T List_reverse(T list)
{
    T head = NULL, next;

    for (; list; list = next) {
        next = list->rest;
        list->rest = head;
        head = list;
    }

    return list;
}

int List_length(T list)
{
    int n;

    for (n = 0; list; list = list->rest)
        n++;

    return n;
}

void List_free(T* list)
{
    T next;

    assert(list);

    for (; list; *list = next) {
        next = (*list)->rest;
        free(*list);
    }
}

void List_map(T* list, void apply(void** x, void* c), void* cl)
{
    assert(apply);

    for (; list; list = list->rest)
        apply(&list->first, cl)
}

void** List_toArray(T list, void* end)
{
    int i, n = List_length(list);

    void** array = (void*)malloc(sizeof(*array) * (n + 1));

    for (i = 0; i < n; i++) {
        array[i] = list->first;
        list = list->rest;
    }
    array[i] = end;

    return array;
}
