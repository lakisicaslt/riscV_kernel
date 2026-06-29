#include "MemoryAllocator.hpp"

MemoryAllocator::FreeMemory* MemoryAllocator::head = 0;
bool MemoryAllocator::initialized = false;

uint64 MemoryAllocator::alignUp(uint64 address) {
    uint64 remainder = address % MEM_BLOCK_SIZE;
    if (remainder ==0) return address;
    return address + (MEM_BLOCK_SIZE - remainder);
}

uint64 MemoryAllocator::alignDown(uint64 address) {
    return address - (address % MEM_BLOCK_SIZE);
}

void MemoryAllocator::init() {
    if(initialized) return; // provera da li je inicijalizovan

    uint64 heapStart = alignUp((uint64) HEAP_START_ADDR);
    uint64 heapEnd = alignDown((uint64) HEAP_END_ADDR);

    if (heapEnd <= heapStart) {
        head = 0;
        initialized = true;
        return;
    }

    head = (FreeMemory*)heapStart;
    head->size = (heapEnd - heapStart) / MEM_BLOCK_SIZE;
    head->next = 0;
    head->prev = 0;

    initialized = true;

}


void* MemoryAllocator::mem_alloc(size_t blocks) {
    if(blocks ==0) return 0;

    if(!initialized) init();

    // +1 blok je header koji čuvamo ispred korisničke memorije.
    size_t neededBlocks = blocks + 1;

    for(FreeMemory* curr = head; curr != 0; curr = curr->next){
        if(curr->size < neededBlocks){
            continue;
        }

        size_t remainingBlocks = curr->size - neededBlocks;

        //segment odgovara zahtevu
        if(remainingBlocks == 0){
            removeFromFreeList(curr);

            curr->size = neededBlocks;

            return (void*)((char*) curr + MEM_BLOCK_SIZE);
        }

        FreeMemory* leftOver = (FreeMemory*)((char*) curr + neededBlocks * MEM_BLOCK_SIZE);
        if(leftOver->prev !=0){
            leftOver->prev->next = leftOver;
        } else {
            head =  leftOver;

        }
        if(leftOver->next!=0){
            leftOver->next->prev = leftOver;
        }
        curr->size = neededBlocks;

        return (void*)((char*) curr + MEM_BLOCK_SIZE);
    }

    //nema dovoljno mem
    return 0;
}

int MemoryAllocator::mem_free(void* ptr) {
    if (ptr == 0) return -1;

    if (!initialized) init();

    uint64 address = (uint64)ptr;

    if (address < (uint64)HEAP_START_ADDR || address >= (uint64)HEAP_END_ADDR) {
        return -1;
    }

    if (address % MEM_BLOCK_SIZE != 0) {
        return -1;
    }

    FreeMemory* segment = (FreeMemory*)((char*)ptr - MEM_BLOCK_SIZE);

    insertIntoFreeList(segment);

    tryToJoin(segment);
    tryToJoin(segment->prev);

    return 0;
}

void MemoryAllocator::removeFromFreeList(FreeMemory* segment) {
    if (segment == 0) return;

    if (segment->prev != 0) {
        segment->prev->next = segment->next;
    } else {
        head = segment->next;
    }

    if (segment->next != 0) {
        segment->next->prev = segment->prev;
    }

    segment->next = 0;
    segment->prev = 0;
}

void MemoryAllocator::tryToJoin(FreeMemory* segment) {
    if (segment == 0 || segment->next == 0) return;

    char* segmentEnd = (char*)segment + segment->size * MEM_BLOCK_SIZE;

    if (segmentEnd == (char*)segment->next) {
        segment->size += segment->next->size;

        segment->next = segment->next->next;

        if (segment->next != 0) {
            segment->next->prev = segment;
        }
    }
}

void MemoryAllocator::insertIntoFreeList(FreeMemory* segment) {
    if (segment == 0) return;

    FreeMemory* previous = 0;
    FreeMemory* current = head;

    while (current != 0 && (char*)current < (char*)segment) {
        previous = current;
        current = current->next;
    }

    segment->prev = previous;
    segment->next = current;

    if (previous != 0) {
        previous->next = segment;
    } else {
        head = segment;
    }

    if (current != 0) {
        current->prev = segment;
    }
}