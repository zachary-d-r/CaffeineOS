#include "../Kernel/system.h"
#include "../Kernel/multiboot.h"
#include "../Drivers/vga.h"

int freeMem;

multiboot_memory_map_t *mmap;
multiboot_info_t *mbiMem;
char *start;
char *end;

// Count bytes of data from 'src' to 'dest' and return 'dest'
unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count) {
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for ( ; count != 0; count--) *dp++ = *sp++;

    return dest;
}

// Set 'count' bytes in 'dest' to 'val' and return 'dest'
void *memset(void *dest, char val, size_t count) {
    char *temp = (char *)dest;
    for ( ; count != 0; count--) *temp++ = val;

    return dest;
}

void* kmalloc(int size) {
    /*
    This function allocates space liniarly starting at freeMem by using memset to copy 0 into memory size times. We
    then comput where freeMem should be and return the address

    Parameters
    ----------
    size : int
        How much memory we want to alocate
    */
   while (freeMem+size > start && freeMem+size < end) { freeMem = freeMem + end-start; }

   if (freeMem + size < start && freeMem + size > end) {
    memset((void*)freeMem, 0, size);
    void* address = (void*) freeMem;
    freeMem += size;

    return address;
   }
}

// Initialize Memory
void initializeMemory(unsigned long addr, char startAddr, char endAddr) {

    start = startAddr;
    end = endAddr;

    mbiMem = (multiboot_info_t *)addr;  // Get the multiboot information from the address

    freeMem = mbiMem->mmap_addr;
    return;

    // This for loop runs through each address of free memory
    for (mmap = (multiboot_memory_map_t *) mbiMem->mmap_addr; (unsigned long) mmap < mbiMem->mmap_addr + mbiMem->mmap_length; mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))) {
        
        // if (mmap->addr > end) { freeMem = mmap->addr; }
        // return;
    }

}