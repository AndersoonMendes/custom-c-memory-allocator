#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define SIZE_HEAP 1024    // 1KB of RAM

typedef struct Header{
    size_t size;    // size of data area, if 64-bit: 8 bytes, elif 32-bit: 4 bytes
    int free;   // 1-> free block | 0-> ocupped block
    struct Header *next;    //Linked list: the pointer points to the address of the next block.
}Header;

static uint8_t pool_memory[SIZE_HEAP]; // byte array with a size of SIZE_HEAP(1024 bytes)

static Header *init_heap = NULL;

void initialize_heap(){
    init_heap = (Header*) pool_memory; // Casting the pool_memory array and treating it as an object of the Header struct.
    init_heap->size= SIZE_HEAP - sizeof(Header);
    init_heap->free = 1;
    init_heap->next = NULL;
}

int main(){
    printf("Total RAM, available: %d bytes\n", SIZE_HEAP);
    printf("Header size: %zu bytes\n", sizeof(Header));
    
    initialize_heap();

    printf("Adress initial of Heap simmuled: %p\n", (void*)init_heap);
    printf("Space available: %zu bytes\n", init_heap->size);
    return 0;
}