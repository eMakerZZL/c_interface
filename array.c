#include "array.h"
#include "arrayrep.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define T Array_T

void ArrayRep_init(T array, int length, int size, void* ary)
{
    assert(array);
    assert(ary && length > 0 || length == 0 && ary == NULL);
    assert(size > 0);
    array->length = length;
    array->size = size;
    if (length > 0)
        array->array = ary;
    else
        array->array = NULL;
}

T Array_new(int length, int size)
{
    T array;

    array = (T)malloc(sizeof(*array));
    if (length > 0) {
        ArrayRep_init(array, length, size, calloc(length, size));
    } else {
        ArrayRep_init(array, length, size, NULL);
    }

    return array;
}

void Array_free(T* array)
{
    assert(array && *array);
    if ((*array)->array != NULL)
        free((*array)->array);
    free(*array);
}

int Array_length(T array)
{
    assert(array);
    return array->length;
}

int Array_size(T array)
{
    assert(array);
    return array->size;
}

void* Array_get(T array, int i)
{
    assert(array);
    assert(i >= 0 && i < array->length);

    return array->array + i * array->size;
}

void* Array_put(T array, int i, void* elem)
{
    assert(array);
    assert(i >= 0 && i < array->length);
    assert(elem);

    memcpy(array->array + i * array->size, elem, array->size);

    return elem;
}

void Array_resize(T array, int length)
{
    assert(array);
    assert(length >= 0);

    if (length == 0)
        free(array->array);
    else if (array->length == 0)
        array->array = (char*)malloc(length * array->size);
    else
        array->array = (char*)realloc(array->array, length * array->size);
    array->length = length;
}

T Array_copy(T array, int length)
{
    assert(array);
    assert(length >= 0);
    copy = Array_new(length, array->size);
    if (copy->length >= array->length && array->length > 0)
        memcpy(copy->array, array->array, array->length * array->size);
    else if (array->length > copy->length && copy->length > 0)
        memcpy(copy->array, array->array, copy->length * copy->size);

    return copy;
}
