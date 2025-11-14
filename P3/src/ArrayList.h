#ifndef ARRLIST_H
#define ARRLIST_H

#include <stdio.h>

typedef struct {
    char** data;      // Pointer to the actual data (heap-allocated array)
    size_t size;      // Current number of elements stored
    size_t capacity;  // Total allocated memory (in terms of elements)
} ArrayList;

ArrayList* List_Create(size_t initial_capacity);
void List_Free(ArrayList *list);
int List_Append(ArrayList *list, char* item);
int List_Get(ArrayList *list, size_t index, char** value_out);
void List_Print(ArrayList *list);

#endif