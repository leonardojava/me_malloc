#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#define CAP 640000 //640 kilobytes 
#define CHUNK_CAP 1024 //fuck it why not

typedef struct Heap_Chunk{
    void* start;
    size_t size;
} Heap_Chunk;
typedef struct Chunk_List{
    size_t size;
    Heap_Chunk chunks[CHUNK_CAP];
} Chunk_List; 
//since sorted chunk list we can binary search? 
int chunk_find(const Chunk_List *list, void *ptr){
    for(int i = 0; i < list->size; i++){
        if(list->chunks[i].start == ptr) return i;
    }
    return -1;
}
void chunk_insert(Chunk_List *list, void *ptr, size_t size){
    assert(list->size < CHUNK_CAP);
    list->chunks[list->size].start = ptr;
    list->chunks[list->size].size = size;
    //insertion sort the chunk
    int i = list->size;
    while(i > 0 && list->chunks[i - 1].start > list->chunks[i].start){
        const Heap_Chunk tmp = list->chunks[i];
        list->chunks[i] = list->chunks[i - 1];
        list->chunks[i-1] = tmp;
        i--;
    }
    list->size++;
}
void chunk_remove(Chunk_List *list, size_t index){
    assert(index < list->size);
    for(int i = index; i < list->size - 1; i++){
        list->chunks[i] = list->chunks[i + 1];
    }
    list->size--;
}
void chunk_list_dump(const Chunk_List *list){ //print all our chunks
    printf("Chunks (%zu):\n", list->size);
    for(size_t i = 0; i < list->size; i++){
        printf(" start: %p, size: %zu\n",
        list->chunks[i].start,
        list->chunks[i].size
        );
    }
}
char heap[CAP] = {0}; 
Chunk_List heap_alloced = {0}; //alloced chunks
Chunk_List heap_freed = {0}; //freed chunks
size_t heap_size = 0;
size_t alloced_size = 0;
size_t freed_size = 0;

void *heap_alloc(size_t size){
    if(size <= 0) return NULL;
    assert(heap_size + size <= CAP);
    void *res = heap + heap_size; //linearly alloc memory by performing pointer arithmetic just slide it forward
    heap_size += size;
    chunk_insert(&heap_alloced, res, size); // insert chunk into alloced chunk list
    return res;
}

//make sure ptr is valid within heap
//check if ptr is valid by iterating through entire heap lol 
void heap_free(void *ptr){
    const int index = chunk_find(&heap_alloced, ptr);
    assert(index >= 0);
    chunk_insert(&heap_freed, heap_alloced.chunks[index].start, heap_alloced.chunks[index].size);
    chunk_remove(&heap_alloced, (size_t)index);
}

int main(){
    
    for(int i = 1; i < 100; i++){
        void *p = heap_alloc(i);
        if(i % 2 == 0) heap_free(p);
    }
    chunk_list_dump(&heap_alloced);
    chunk_list_dump(&heap_freed);
    //heap_free(root);
}