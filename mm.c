/*
 * mm.c
 *
<<<<<<< HEAD
 * Name: Ryan Hayes and Ben Song
=======
 * Name: Ben Song
>>>>>>> 24849b257f8206bc1111ff5b846167ba0b14836b
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 * Also, read the project PDF document carefully and in its entirety before beginning.
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#include "mm.h"
#include "memlib.h"

/*
 * If you want to enable your debugging output and heap checker code,
 * uncomment the following line. Be sure not to have debugging enabled
 * in your final submission.
 */
// #define DEBUG

#ifdef DEBUG
/* When debugging is enabled, the underlying functions get called */
#define dbg_printf(...) printf(__VA_ARGS__)
#define dbg_assert(...) assert(__VA_ARGS__)
#else
/* When debugging is disabled, no code gets generated */
#define dbg_printf(...)
#define dbg_assert(...)
#endif /* DEBUG */

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#define memset mem_memset
#define memcpy mem_memcpy
#endif /* DRIVER */

/* What is the correct alignment? */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
static size_t align(size_t x)
{
    return ALIGNMENT * ((x+ALIGNMENT-1)/ALIGNMENT);
}

/* Global Variables */
void *find_open_block(size_t size);

struct header {
    size_t size;
    struct header *next_block;
    struct header *prev_block;
};
typedef struct header block_header;

/*
 * Initialize: returns false on error, true on success.
 */
bool mm_init(void)
{
    /* IMPLEMENT THIS */
    struct header *ptr = mem_sbrk(align(sizeof(block_header)));
    //size_t size = 16;
    // this is arbritary right now we don't care about inital size 
    ptr->size = 1;
    ptr->next_block = ptr;
    ptr->prev_block = ptr;
    return true;
}

/*
 * malloc
 */
void* malloc(size_t size)
{
    
    /* IMPLEMENT THIS */
    
    if (size == 0) {
        return NULL;
    }
    int new_size = align(size + align(sizeof(block_header)));
    block_header *ptr = find_open_block(new_size);
    if (ptr == NULL) {
        ptr = mem_sbrk(new_size);
        if ((long)ptr == -1) 
            return NULL;
        
        else 
            ptr->size = new_size | 1;
        } 
    else {
        ptr->size |= 1;
        ptr->prev_block->next_block = ptr->next_block;
        ptr->next_block->prev_block = ptr->prev_block;
    }
    return (char *)ptr + align(sizeof(block_header));
}

// we start by looking at the first block. We always want a free block to basically have a free linked list
void *find_open_block(size_t size) {
    block_header *ptr;
    // we say that our first block will always be the free, so we immediently go to next block 
    for (ptr = ((block_header *)mem_heap_lo())->next_block; 
    /* we now want to check the block size and make sure that the next block is large enough to hold our current size
    and we need to check that we have not reached the end of out heap */
        ptr != mem_heap_lo() && ptr->size < size; 
        // if those conditions are not met we move to next block
        ptr = ptr->next_block); 
    // if ptr is not the first block then we have found a free block that is not the first block
    if (ptr != mem_heap_lo()) 
        return ptr;
    else 
        return NULL;
}

/*
 * free
 */
void free(void* ptr)
{
    /* IMPLEMENT THIS */
    if (ptr != NULL) {
        /* we want the contents of the ptr without the header content and we also need a pointer to the 
        head of the free list */
        block_header *blockPtr = ptr - align(sizeof(block_header));
        block_header *head = mem_heap_lo();
        // *foot = mem_heap_hi(); eventually we will need to implement this  
        // free up the block and change block pointers to point to the next free block 
        blockPtr->size &= ~1;
        blockPtr->next_block = head->next_block;
        blockPtr->prev_block = head;
        head->next_block = blockPtr;
        blockPtr->next_block->prev_block = blockPtr;
    }
    return;
}

/*
 * realloc
 */
void* realloc(void* oldptr, size_t size)
{
    // when oldptr is null, it is equivalent to malloc(size)
    if (oldptr == NULL) {
        return malloc(size);
    }
    // when size is 0, it is equivalent to free(oldptr)
    else if (size == 0) {
        free(oldptr);
        return NULL;
    }
    // if size is less than the old block then we will just be retuning the contents of the oldptr
    else if (size <= 16)
    {
        return oldptr;
    }
    /* when oldptr is not null and the size is greater than the block size we 
    initilize a new ptr that will have a block size of size and we then copy the contents
    of the oldptr into the new ptr and return the new ptr*/
    else {
        void *ptr = malloc(size);
        if (ptr != NULL) {
            mem_memcpy(ptr, oldptr, 16);
            free(ptr);
        }
        return ptr;
    }
}

/*
 * calloc
 * This function is not tested by mdriver, and has been implemented for you.
 */
void* calloc(size_t nmemb, size_t size)
{
    void* ptr;
    size *= nmemb;
    ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

/*
 * Returns whether the pointer is in the heap.
 * May be useful for debugging.
 */
static bool in_heap(const void* p)
{
    return p <= mem_heap_hi() && p >= mem_heap_lo();
}

/*
 * Returns whether the pointer is aligned.
 * May be useful for debugging.
 */
static bool aligned(const void* p)
{
    size_t ip = (size_t) p;
    return align(ip) == ip;
}

/*
 * mm_checkheap
 */
bool mm_checkheap(int lineno)
{
#ifdef DEBUG
    /* Write code to check heap invariants here */
    /* IMPLEMENT THIS */
#endif /* DEBUG */
    return true;
}