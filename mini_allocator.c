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

void* my_malloc(size_t size_desired){
    if(init_heap == NULL){
        initialize_heap();
    }
    Header *atual = init_heap;

    while (atual != NULL){
        if(atual->free && atual->size >= size_desired){

            if(atual->size >= size_desired + sizeof(Header) + 8){ // Verify if has space for more one
                Header *new_block = (Header*)((uint8_t*)atual + sizeof(Header) + size_desired); // Adress of new block
                new_block->size = atual->size - size_desired - sizeof(Header); //1012 - 20 - 12
                new_block->free = 1;
                new_block->next = atual->next;

                atual->size = size_desired; // Reajusta o tamanho de 1012 para 20 bytes
                atual->next = new_block;

            }
            atual->free = 0;

            return (void*)((uint8_t*)atual + sizeof(Header));
        }

        atual = atual->next;
    }
    
    return NULL;
}

int main(){

    int *p1 = (int*) my_malloc(5*sizeof(int)); // 20 bytes

    printf("Total RAM, available: %d bytes\n", SIZE_HEAP);

    printf("Header size: %zu bytes\n", sizeof(Header));
    
    printf("Adress initial of Heap simmuled: %p\n", (void*)init_heap);    
    printf("p1 adress: %p\n", (void*)p1);
    
    printf("Space available: %zu bytes\n", init_heap->next->size);

    return 0;
}