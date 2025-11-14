#include <stdlib.h>

#include "ArrayList.h"


/**
 * Creates and initializes a new ArrayList.
 * @param initial_capacity The starting capacity of the list.
 * @return A pointer to the new ArrayList, or NULL if allocation fails.
 */
ArrayList* List_Create(size_t initial_capacity) 
{
    if (initial_capacity == 0) {
        initial_capacity = 10; 
    }
    
    ArrayList* list = malloc(sizeof(ArrayList));
    if (!list) {
        return NULL; 
    }

    
    list->data = malloc(initial_capacity * sizeof(char*));
    if (!list->data) {
        free(list); 
        return NULL; 
    }

    list->size = 0;
    list->capacity = initial_capacity;
    
    return list;
}

/**
 * Frees all memory associated with the ArrayList.
 * @param list A pointer to the ArrayList to be freed.
 */
void List_Free(ArrayList *list) 
{
    if (list) {
        free(list->data); 
    }
}

/**
 * Appends a new element to the end of the list, resizing if necessary.
 * @param list A pointer to the ArrayList.
 * @param item The integer value to add.
 * @return 0 on success, -1 on failure (resize failed).
 */
int List_Append(ArrayList *list, char* item) 
{
    
    if (list->size == list->capacity) 
    {
        size_t new_capacity = list->capacity * 2; 
        
        
        char** new_data = realloc(list->data, new_capacity * sizeof(char*));
        
        if (!new_data) 
        {
            fprintf(stderr, "Error: Failed to reallocate memory.\n");
            return -1; 
        }
        
        list->data = new_data;
        list->capacity = new_capacity;
    }

    
    list->data[list->size] = item;
    list->size++;
    
    return 0; 
}

/**
 * Gets an element at a specific index.
 * @param list A pointer to the ArrayList.
 * @param index The index of the element to retrieve.
 * @param value_out A pointer to an integer where the value will be stored.
 * @return 0 on success, -1 if the index is out of bounds.
 */
int List_Get(ArrayList *list, size_t index, char** value_out) 
{
    if (index >= list->size) 
    {
        fprintf(stderr, "Error: Index %zu out of bounds for size %zu.\n", index, list->size);
        return -1; 
    }
    
    *value_out = list->data[index];
    return 0; 
}

void List_Print(ArrayList *list) 
{
    printf("List (Size: %zu, Capacity: %zu): [ ", list->size, list->capacity);
    for (size_t i = 0; i < list->size; i++) 
    {
        printf("%s, ", list->data[i]);
    }
    printf("]\n");
}