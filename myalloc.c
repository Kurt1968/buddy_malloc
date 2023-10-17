#include "myalloc.h"

int initialized=0;
uint8_t mem[1024*1024+1024+sizeof(bitmap_tree)+sizeof(bitmap)+sizeof(buddy_alloc)];
buddy_alloc* alloc;


void* myalloc(int size){
    if(!initialized){
        memset(mem,0,1024*1024);
        alloc=buddy_init(mem,1024*1024+1024+sizeof(bitmap_tree)+sizeof(bitmap)+sizeof(buddy_alloc),LEVELS);
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
    int size=0;
    if(ptr==NULL){
        return -1;
    }
    if(is_buddy(ptr)){
        res=buddy_free_mem(alloc,ptr);
        return res;
    }
    else{
        void* temp=ptr-sizeof(int);
        res=munmap(temp,size+sizeof(int));
        if(res==-1){
            //perror("munmap failed");
            return -1;
        }
        return res;
    }
}

int is_buddy(void* ptr){
    if(ptr==NULL){
        return 0;
    }
    if(alloc->buf<=(uint8_t*)ptr&&(uint8_t*)ptr<=alloc->buf+alloc->usable_mem){
        return 1;
    }
    else return 0;
}
void myprint(){
    printf("=============================================\n");
    print_bitmap(alloc->tree->bm);
    printf("=============================================\n");
}