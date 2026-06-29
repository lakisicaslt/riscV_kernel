#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include "hw.h"

class MemoryAllocator {
public:
    static void init();

    // Prima broj blokova, ne broj bajtova.
    // Ovo odgovara kernel/ABI nivou.
    static void* mem_alloc(size_t blocks);

    static int mem_free(void* ptr);

private:
    struct FreeMemory {
        size_t size;        // veličina segmenta u blokovima
        FreeMemory* next;
        FreeMemory* prev;
    };

    static FreeMemory* head;
    static bool initialized;

    static uint64 alignUp(uint64 address);
    static uint64 alignDown(uint64 address);

    static void removeFromFreeList(FreeMemory* segment);
    static void insertIntoFreeList(FreeMemory* segment);
    static void tryToJoin(FreeMemory* segment);
};

#endif
