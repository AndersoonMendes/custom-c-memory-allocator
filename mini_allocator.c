#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define SIZE_HEAP 1024 // 1KB of RAM

typedef struct Header{
    size_t size; // size of data area
    int free; // 1-> free block | 0-> ocupped block
    struct Header *next; // linked list: the pointer points to the address of the next block.
}Header;

static uint8_t pool_memory[SIZE_HEAP]; // byte array with a size of SIZE_HEAP(1024 bytes)

static Header *init_heap = NULL;

void initialize_heap(){
    init_heap = (Header*) pool_memory; // casting the pool_memory array and treating it as an object of the Header struct.
    init_heap->size= SIZE_HEAP - sizeof(Header); //1024 -12
    init_heap->free = 1;
    init_heap->next = NULL;
}

void* my_malloc(size_t size_desired){
    if(init_heap == NULL){
        initialize_heap();
    }
    // sweep the heap
    Header *current = init_heap;
    while (current != NULL){
        if(current->free && current->size >= size_desired){

            if(current->size >= size_desired + sizeof(Header) + 8){ // verify if has space for more one
                Header *new_block = (Header*)((uint8_t*)current + sizeof(Header) + size_desired); // address of new block
                new_block->size = current->size - size_desired - sizeof(Header); //1012 - 20 - 12
                new_block->free = 1;
                new_block->next = current->next;

                current->size = size_desired; // resizes from 1012 to 20 bytes.
                current->next = new_block;

            }
            current->free = 0;

            return (void*)((uint8_t*)current + sizeof(Header));
        }

        current = current->next;
    }
    
    return NULL;
}

void my_free(void *ptr){ // coalescing
    if(ptr == NULL) return;

    Header *header = (Header*)((uint8_t*)ptr - sizeof(Header)); // to find pointer address
    header->free = 1; // define free block

    // sweep the heap
    Header *current = init_heap;
    while (current != NULL && current->next != NULL){
        if(current->free && current->next->free){
            current->size += sizeof(Header) + current->next->size; //20 + 12 + 50
            current->next = current->next->next; // after integrating the neighboring block, it jumps to the next one in the second block.
        } else {
        current = current->next;
        }
    }
}

void inspect_heap(const char *title){
    printf("\n--- %s ---\n", title);
    // sweep the heap
    Header *current = init_heap;
    int i = 0;
    
    if(current==NULL){
        printf("Space available on HEAP: %zu bytes\n", SIZE_HEAP);
    } else{
        while (current != NULL){
            printf("Block #%d [%s] | Address : %p | Payload Size: %zu bytes\n", i++, current->free? "FREE" : "OCCUPIED", (void*)current, current->size);
            current = current->next;
        }
    }
    printf("-----------------------------------------------------------------------\n");
}

int main(){

    inspect_heap("1. Initial state of HEAP");

    int *p1 = (int*) my_malloc(20);
    int *p2 = (int*) my_malloc(50);
    
    inspect_heap("2. After allocating p1 and p2");

    my_free(p1);
    my_free(p2);

    inspect_heap("3. After freeing the memory (coalescing)");

    return 0;
}