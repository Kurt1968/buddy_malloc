#include "myalloc.h"

int initialized=0;
uint8_t mem[1024*1024+1024+sizeof(bitmap_tree)+sizeof(bitmap)+sizeof(buddy_alloc)];
buddy_alloc* alloc;
ptr_list_item* list=NULL;


void* myalloc(int size){
    if(!initialized){
        memset(mem,0,1024*1024);
        alloc=buddy_init(mem,1024*1024+512+sizeof(bitmap_tree)+sizeof(bitmap)+sizeof(buddy_alloc),LEVELS);
        initialized=1;
    }
    int* res;
    if(size>=SMALL_ALLOC){
MMAP:   res=mmap(NULL,size+sizeof(size),PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);
        if(res==MAP_FAILED){
            perror("mmap failed");
            return NULL;
        }
        *res=size;
        if(is_already_freed(&res[1])){
            pop_from_freed(&res[1]);
        }
        return (void*)&res[1];
    }else{
        void* temp=buddy_alloc_mem(alloc,size);
        if(temp==NULL){
            printf("no more memory-using mmap\n");
            goto MMAP;
            return res;
        }
        else{
            res=temp;
            if(is_already_freed(res)){
                pop_from_freed(res);
            }
            return res;
        }
    }
}

int myfree(void* ptr){
    int res;
    if(ptr==NULL){
        return -1;
    }
    if(is_buddy(ptr)){
        res=buddy_free_mem(alloc,ptr);
        return res;
    }
    else{
        int* temp=ptr-sizeof(int);
        int size=*temp;
        res=munmap(temp,size+sizeof(int));
        if(res==-1){
            perror("munmap failed");
            return -1;
        }
        else return res;


    }
}
int is_already_freed(void *ptr)  {
    ptr_list_item *current = list;
    while (current != NULL) {
        if (current->ptr == ptr) {
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}
int is_buddy(void* ptr){
    if(ptr==NULL){
        return 0;
    }
    if(alloc->buf<=ptr&&ptr<=alloc->buf+alloc->usable_mem){
        return 1;
    }
    else return 0;
}
void pop_from_freed(void *ptr) {
    ptr_list_item *current = list;
    ptr_list_item *prev = NULL;
    while (current != NULL) {
        if (current->ptr == ptr) {
            if (prev == NULL) {
                list = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void add_to_freed_list(void *ptr) {
    ptr_list_item *new_node = (ptr_list_item *)malloc(sizeof(ptr_list_item));
    if (new_node == NULL) {
        perror("malloc failed");
        exit(-1); 
    }
    new_node->ptr = ptr;
    new_node->next = list;
    list = new_node;
}
