#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "buddy_alloc.h"

#define SMALL_ALLOC 1024
#define LEVELS 10

typedef struct{
    void* ptr;
    struct ptr_list_item* next;
}ptr_list_item;

void* myalloc(int size);
int myfree(void* ptr);
int is_already_freed(void *ptr);
void pop_from_freed(void *ptr);
void add_to_freed_list(void *ptr);