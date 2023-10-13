#include "myalloc.h"

int initialized=0;
uint8_t mem[1024*1024+1024+sizeof(bitmap_tree)+sizeof(bitmap)+sizeof(buddy_alloc)];
buddy_alloc* alloc;
void* list[100];
int last_index=-1;


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
            return res;
        }
    }
}

int myfree(void* ptr){
    int res;
    if(ptr==NULL||is_already_freed(ptr)){
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
        else{
            add_to_freed_list(ptr);
            return res;
        }


    }
}
int is_already_freed(void *ptr)  {
    for(int i=0;i<100;i++){
        if(list[i]==ptr){
            return 1;
        }
        else if(list[i]==NULL){
            return 0;
        }
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
    for(int i=0;i<100;i++){
        if(list[i]==ptr){
            list[i]=NULL;
            for (int j = i; j < 99; j++) {
                list[j] = list[j + 1];
            }
            return;
        }
    }
}
void print_freed_list() {
    for(int i=0;i<100;i++){
        if(list[i]!=NULL){
            printf("%p\n",list[i]);
        }
    }
}

void add_to_freed_list(void *ptr) {
    last_index++;
    if(last_index==100){
        last_index=0;
    }
    list[last_index]=ptr;
    return;
}
