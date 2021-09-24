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


/*
 * Initialize: returns false on error, true on success.
 */
bool mm_init(void)
{
    /* IMPLEMENT THIS */
    int mem_size = 100;
 
    int *heap = malloc(mem_size);
    // memory was not allocated
    if (heap == NULL) {
        return false;
    }
    return true;
}

/*
 * malloc
 */
void* malloc(size_t size)
{
    /* IMPLEMENT THIS */
    if (size == 0) {
        return NULL
    }
        
    if (size + mem_brk > mem_max_addr) {
        return NULL
    }
    

    return NULL;
}

/*
 * free
 */
void free(void* ptr)
{
    /* IMPLEMENT THIS */
    // this should do nothing if ptr has a value of NULL
    if (ptr == NULL) {
        return;
    }
    return;
}

/*
 * realloc
 */
void* realloc(void* oldptr, size_t size)
{
    /* IMPLEMENT THIS */
    void *ptr;

    if (oldptr == NULL) {
        ptr = malloc(size);
        // Check to make sure malloc was successful 
        if (ptr == NULL) {
            return NULL;
        }
        return ptr;
    }
    else if (size == 0) {
        free(oldptr);
        return NULL;
    }
    else {
        if (oldptr && (size > 16)) {
            ptr = malloc(size);
        }
        else {
            mem_memcpy(ptr, oldptr, size);
            free(oldptr);
            return ptr;
        }
        // if malloc was sucessful copy contents to new ptr
        if (ptr) {
            mem_memcpy(ptr, oldptr, size);
            free(oldptr);
        }
        return ptr;
    }
    
    return NULL;
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
