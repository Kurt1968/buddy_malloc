#include "bitmap.h"

//IMPLEMENTAZIONE BITMAP

bitmap* bitmap_init(uint8_t *buf,int buf_size){
    bitmap* bm = (bitmap*)buf;
    bm->buf_start = buf + sizeof(bitmap);
    bm->bits=buf_size;
    bm->buf_size=get_bytes(bm->bits);
    bm->buf_end = buf+ bm->buf_size;

    for(int i=0;i<bm->buf_size;i++){
        set_bit(bm,i,FREE);
    }
    return bm;
}

int set_bit(bitmap* bm, int index, state s){
   if(index<0||index>bm->bits){
        return 0;
   }
    int i=index>>3;
    int offset=index%8;
    if(s==FREE){
        bm->buf_start[i] &= ~(1<<offset);
        return 1;
    }
    else if(s==BUSY){
        bm->buf_start[i] |= (1<<offset);
        return 1;
    }
    else return 0;
}  

uint8_t get_bit(bitmap* bm, int index){
   if(index<0||index>bm->bits){
        return 0;
   }
    int i=index>>3;
    int offset=index%8;
    return (bm->buf_start[i] & (1U<<offset))!=0;
}

int get_bytes(int bits){
    return(bits>>(3+(bits%8))!=0);
}

//IMPLEMENTAZIONE BITMAP TREE

bitmap_tree* tree_init(uint8_t* buf, int buf_size, int levels){
    bitmap_tree* tree=(bitmap_tree*)buf;
    uint8_t* bm_s=buf+sizeof(bitmap_tree);
    uint8_t* bm=bm_s+sizeof(bitmap);
    tree->bm=(bitmap*)bm_s;tree->levels=levels;tree->nodes=get_nodes(levels);tree->leafs=get_leafs(levels);
    int bm_size=buf_size-sizeof(bitmap_tree)-sizeof(bitmap);
    bitmap_init(bm_s, bm_size);
    return tree;
}

int first_node(bitmap_tree* tree, int index){
    return ((0x01<<tree_level(tree, index)));
}

int tree_level(bitmap_tree* tree, int index){
    int ret=(int)floor(log2(index));
    if(ret>tree->levels){
        return tree->levels-1;
    }
    else if(ret>0){
        return ret;}
    else return 0;
}

int node_offset(bitmap_tree* tree,int index){
    return first_node(tree, tree_level(tree, index))-index;
}

int free_node(bitmap_tree* tree, int level){
    int start = pow(2, level); int end = pow(2, level+1);
    for(int i=start;i<end;i++){
        if(get_tree_bit(tree, i)==FREE) return i;
    }
    return -1;
}

int buddy_node(int index){
   return (index & 0x0001)?(index-1):(index);
}

int parent_node(int index){
    return (uint16_t)index/2;
}

int level_occupied(bitmap_tree* tree, int level){
    int start = pow(2, level);
    int end= pow(2, level+1);
    int ret = 0;
    for(int i = start; i<end; i++){
        if(get_tree_bit(tree, i)==BUSY)ret++;
    }
    return ret;
}

int level_free(bitmap_tree* tree, int level){
    int start = pow(2, level);
    int end= pow(2, level+1);
    int ret = 0;
    for(int i = start; i<end; i++){
        if(get_tree_bit(tree, i)==FREE)ret++;
    }
    return ret;
}

int get_nodes(int levels){
    return pow(2, levels)+1;
}

int get_leafs(int levels){
    return pow(2, levels);
}

int get_free_node(bitmap_tree* tree, int level){
    int start = pow(2, level);
    int end= pow(2, level+1);
    for(int i = start; i<end; i++){
        if(get_tree_bit(tree, i)==FREE){
            set_parent(tree, level, i, BUSY);
            set_child(tree, i, BUSY);
            set_tree_bit(tree, i, BUSY);
            return i;
    }}
    return -1;
}

void set_parent(bitmap_tree* tree, int level, int index, state s){
    if(s==BUSY){
        int parent_idx = parent_node(index);
        while(level>0){ 
            set_tree_bit(tree, parent_idx, s);
            parent_idx = parent_node(parent_idx);
            level--;
        }
    }
    if(s==FREE){   
        int parent_idx = parent_node(index);
        while(level>0){
            if(get_tree_bit(tree, buddy_node(index))==FREE){
                set_tree_bit(tree, parent_idx, s);
                parent_idx = parent_node(parent_idx);
                level--;
            }else return;
        }
        
    }
}

void set_child_rec(bitmap_tree* tree, int left,int right, state s){
    if(left>tree->nodes || right>tree->nodes){
        return;
    }
    else{
        set_tree_bit(tree, left, s);
        set_tree_bit(tree, right, s);
        set_child_rec(tree, left<<1,(left<<1)+1, s);
        set_child_rec(tree, right<<1,(right<<1)+1, s);
    }
}

void set_child(bitmap_tree* tree, int index, state s){
    set_child_rec(tree, index<<1, (index<<1)+1, s);
}

void set_tree_bit(bitmap_tree *tree, int bit, state status){
    set_bit(tree->bm, bit, status);
}

int get_tree_bit(bitmap_tree *tree, int bit){
    return get_bit(tree->bm, bit);
}

void print_bitmap(bitmap* bm){
    printf("Bitmap:\n");
    /*
    int i=0;
    while (i<LEVELS){
        int start = pow(2, i);
        int end = pow(2, i+1);
        for(int j=start;j<end;j++){
            printf("%d", get_bit(bm, j));
        }
        printf("\n");
        i++;
    printf("\n");}
    */
    for(int i=0;i<bm->bits;i++){
        printf("%d", get_bit(bm, i));
    }
    printf("\n");
}