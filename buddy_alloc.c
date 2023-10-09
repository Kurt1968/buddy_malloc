#include "buddy_alloc.h"

buddy_alloc* buddy_init(uint8_t* buf,int buf_size, int levels){
    assert(levels<=LEVEL_MAX);
    buddy_alloc* alloc = (buddy_alloc*)buf;
    int tree_size=sizeof(bitmap_tree)+sizeof(bitmap)+pow(2,levels);
    uint8_t* tree_buf=buf+sizeof(buddy_alloc);
    alloc->tree=tree_init(tree_buf,tree_size,levels);
    alloc->levels=levels;
    alloc->buf=tree_buf+tree_size;
    alloc->buf_size=buf_size-tree_size-sizeof(buddy_alloc);
    alloc->items=pow(2,levels);
    alloc->min_bucket=alloc->buf_size>>levels-1;
    alloc->leafs=alloc->buf_size/alloc->min_bucket;
    alloc->usable_mem=alloc->leafs*alloc->min_bucket;
    return alloc;
}

void* get_buddy(buddy_alloc* alloc,int level){
    int offset=0;
    if(level<0){
        return NULL;
    }
    if(!level_free(alloc->tree,level)){
        return NULL;
    }
    if(level_free(alloc->tree,level)){
        int index=get_free_node(alloc->tree,level);
        offset=alloc->min_bucket*(alloc->levels-level)*(index-first_node(alloc->tree,index));
        void* res=(void*)(alloc->buf+offset);
        int* tmp=res;
        *tmp=index;
        res=res+sizeof(int);
        return res;
    }
} 

void* buddy_alloc_mem(buddy_alloc* alloc,int size){
    int level=tree_level(alloc->tree,alloc->usable_mem/(size+sizeof(int)));
 
    if(level>alloc->levels) level=alloc->levels;
    
    void* res=get_buddy(alloc,level);

    if(res==NULL){

       return NULL;
    }
    else{
        return res;}
}

int buddy_free_mem(buddy_alloc* alloc,void* ptr){
    if(ptr!=NULL){
       int offset=(uint8_t*)ptr-(int)alloc->buf-sizeof(int);
       int offset_max=alloc->usable_mem;
       if(offset>offset_max){
           return -1;
       }
        int* index_ptr=(int*)(ptr-sizeof(int));
        int index=*index_ptr;
        if(offset%alloc->min_bucket||get_tree_bit(alloc->tree,index)==FREE){
            return -1;
        }
        memset(ptr,0,alloc->min_bucket);
        set_tree_bit(alloc->tree,index,FREE);
        set_parent(alloc->tree,index,tree_level(alloc->tree,index),FREE);
        set_child(alloc->tree,index,FREE);
        return 0;
    }
}
