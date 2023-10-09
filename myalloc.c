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
    if(ptr==NULL||is_already_freed(ptr)){
        return -1;
    }

    int* tmp=(int*)ptr;
    int len;
    tmp--;
    len=*tmp;
    if(len>=SMALL_ALLOC){
MUNMAP: res=munmap(tmp,len+sizeof(int));
        if(res==-1){
            //perror("munmap failed");
            return -1;
        }
        add_to_freed_list(ptr);
        return 0;

}   else{
        //printf("using buddy_free\n");
        res=buddy_free_mem(alloc,ptr);
        if(res==-1){
            //perror("free failed-using munmap\n");
            goto MUNMAP;
            return -1;
        }
        add_to_freed_list(ptr);
        return 0;
    }}
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