#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "buddy_alloc.h"

#define SMALL_ALLOC 1024


void* myalloc(int size);
int myfree(void* ptr);
void myprint();
int is_buddy(void* ptr);