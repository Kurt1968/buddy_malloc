#include "bitmap.h" 
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define LEVEL_MAX 32

//Struttura dell'allocatore
typedef struct{
    int levels;
    int buf_size;
    int usable_mem;
    int min_bucket;
    int items;
    int leafs;
    bitmap_tree* tree;
    uint8_t* buf;
}buddy_alloc;

//Funzioni
//Inizializza l'allocatore
buddy_alloc* buddy_init(uint8_t* buf,int buf_size, int levels);
//Dato un livello ritorna l'indirizzo del buddy
void* get_buddy(buddy_alloc* alloc,int level);
//Alloca il buddy chiamando get_buddy
void* buddy_alloc_mem(buddy_alloc* alloc,int size);
//Libera il buddy
int buddy_free_mem(buddy_alloc* alloc,void* ptr);

