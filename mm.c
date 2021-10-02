/*
 * mm.c
 *
 * Name: Ryan Hayes and Ben Song
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
    //block_header *header = (block_header *)mem_heap_lo();
    struct header *ptr = mem_sbrk(align(sizeof(block_header)));
    //block_header *footer = (block_header *)mem_heap_hi();
    //size_t size = 16;
    // this is arbritary right now we don't care about inital size 
    //header->size = align(sizeof(block_header));
    //header->next_block = ptr;
    //header->prev_block = NULL;
    ptr->size = 1;
    ptr->next_block = ptr;
    ptr->prev_block = ptr;
    //footer->size = align(sizeof(block_header));
    //footer->next_block = NULL;
    //footer->prev_block = ptr;
    return true;
}

/*
 * malloc
 */
void* malloc(size_t size)
{

    /* IMPLEMENT THIS */
    // nothing to allocate if size is zero
    if (size == 0) {
        return NULL;
    }
    // we set the new size to also include the header size then we call helper fuction to find a free block
    int new_size = align(size + align(sizeof(block_header)));
    block_header *ptr = find_open_block(new_size);
    // if the helper fuction could not find a open block we use mem_sbrk to extend heap to get enough space to have a block big enough 
    // we then set the block to be allocated
    if (ptr == NULL) {
        ptr = mem_sbrk(new_size);
        ptr->size = new_size | 1;
        } 
    // if find_open_block finds a free block that is big enough then we set that block to be allocated and now that this block is 
    // allocated we need to update the blocks surrounding it so they no longer point to the allocated block  
    else {
        ptr->size |= 1;
        ptr->next_block->prev_block = ptr->prev_block;
        ptr->prev_block->next_block = ptr->next_block;
    }
    // we return the ptr which now points to the next block in the heap
    return (char *)ptr + align(sizeof(block_header));
}

// we start by looking at the first block. We always want a free block to basically have a free linked list
void *find_open_block(size_t size) {
    block_header *ptr;
 /* we say that our first block will always be the free, so we immediently go to next block 
    we now want to check the block size and make sure that the next block is large enough to hold our current size
    and we need to check that we have not reached the end of out heap if those conditions are not met we move to next block*/
    for (ptr = ((block_header *)mem_heap_lo())->next_block; ptr != mem_heap_lo() && ptr->size < size;  ptr = ptr->next_block) {
    }
    // if ptr is not the first block then we have found a free block and we return ptr 
    if (ptr != mem_heap_lo()) {
        return ptr;
    }
     // if we do not find a free block and will return NULL
    else {
        return NULL;
    }
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
    // We initialize a pointer as the pointer that needs to be reallocated without the header
    block_header *block_ptr = oldptr - sizeof(block_header);
    // We create a new pointer which will be returned. This is the proper size. If malloc does not work here, we return NULL.
    void *ptr = malloc(size);
    if (ptr == NULL) {
        return NULL;
    }
    // We initialize a new variable that is the size of the data that we need to reallocate.
    size_t copySize = block_ptr->size-sizeof(block_header);
    // If the size we are hoping to reallocte is less than this, then we reduce the size that we are copying to the new reallocated size.
    if (align(size) < copySize) {
        copySize = align(size);
    }
    // We copy the data that we are able to fit into the pointer that we allocated earlier.  We then free up the pointer that was passed in. Return the new pointer.
    mem_memcpy(ptr, oldptr, copySize);
    mm_free(oldptr);
    return ptr;
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