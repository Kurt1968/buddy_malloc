#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define LEVELS 10
//===================== BITMAP =====================
//Enumerazione degli stati per facilità di lettura
typedef enum state{
    FREE=0,
    BUSY=1
}state;

//Struttura della bitmap
typedef struct{
    uint8_t* buf_start;
    uint8_t* buf_end;
    int buf_size;
    int bits;
    int allocated;
}bitmap;

//Funzioni
//Inizializza la bitmap
bitmap* bitmap_init(uint8_t* buf, int buf_size);
//Converte bit a byte
int get_bytes(int bits);
//Controlla lo stato di un bit
uint8_t get_bit(bitmap* bm, int index);
//Setta lo stato di un bit
int set_bit(bitmap* bm, int index, state s);
//Print
void print_bitmap(bitmap* bm);

//===================== BITMAP TREE =====================
//Struttura dell'albero
typedef struct{
    bitmap* bm;
    int levels;
    int nodes;
    int leafs;
}bitmap_tree;

//Funzioni
//Inizializza l'albero
bitmap_tree* tree_init(uint8_t* buf, int buf_size, int levels);
//Dato un indice, restituisce il livello nell'albero
int tree_level(bitmap_tree* tree, int index);
//Dato un indice, restituisce il primo nodo del livello
int first_node(bitmap_tree* tree, int index);
//Dato un livello, restituisce il primo nodo libero nel livello, -1 se non presente
int free_node(bitmap_tree* tree, int level);
//Dato un indice, calcola l'offset del nodo
int node_offset(bitmap_tree* tree, int index);
//Dato un indice, restituisce il nodo padre
int parent_node(int index);
//Dato un indice, restituisce quello del buddy
int buddy_node(int index);
//Controlla se ci sono nodi occupati sul livello
int level_occupied(bitmap_tree* tree, int level);
//Calcola il numero di nodi dati i livelli
int get_nodes(int levels);
//Calcola il numero di foglie dati i livelli
int get_leafs(int levels);
//Controlla la presenza di nodi liberi sul livello
int level_free(bitmap_tree* tree, int level);
//Prende un nodo libero sul livello
int get_free_node(bitmap_tree* tree, int level);
//Wrapper per get_bit della bitmap
int get_tree_bit(bitmap_tree* tree, int bits);
//Wrapper per set_bit della bitmap
void set_tree_bit(bitmap_tree* tree, int bit,state status);
//Setta i nodi padre
void set_parent(bitmap_tree* tree, int index,int level,state status);
//Setta i nodi figli
void set_child(bitmap_tree* tree, int index,state status);
