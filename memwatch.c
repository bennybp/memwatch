#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <malloc.h>

#include "memwatch.h"



/* Function pointer typedefs */
typedef void * (*mallocptr)(size_t);
typedef void (*freeptr)(void *);


/* Pointer to the real malloc function */
static mallocptr real_malloc = NULL;

/* Pointer to the real free function */
static freeptr real_free = NULL;

/* Callback pointer */
static mallocfailptr malloc_fail_hook = NULL;

/* How much is currently allocated */
static size_t allocated_mem = 0;

/* Current memory limit */
/* size_t is unsigned, so -1 wraps around */
static size_t maxsize = -1;



void memwatch_init(void)
{
    /* Find the real_malloc and free */
    real_malloc = (mallocptr)dlsym(RTLD_NEXT, "malloc");
    if(real_malloc == NULL)
        printf("Error: Cannot find real_malloc! Error: %s\n", dlerror());

    real_free = (freeptr)dlsym(RTLD_NEXT, "free");
    if(real_free == NULL)
        printf("Error: Cannot find real_free! Error: %s\n", dlerror());
}


void * malloc(size_t size)
{
    void * ptr = NULL;
    size_t memsize = 0;  /* amount of memory actually allocated */

    /* This is the first time malloc is being called
       If so, we need to initialize */
    if(real_malloc == NULL)
        memwatch_init();

    /* check if we are beyond the limit. If so, call malloc_fail_hook() */
    if(((allocated_mem + size) > maxsize) && (malloc_fail_hook != NULL))
        malloc_fail_hook(allocated_mem, size, maxsize);

    /* actually allocate */
    ptr = real_malloc(size);
    memsize = malloc_usable_size(ptr);


    /* Add to allocated memory */
    allocated_mem += memsize; 

    /* Some debugging */
    /*printf("Allocating %ld bytes (Actual: %ld)\n", size, memsize); */
    /*printf("Allocated memory now %ld\n", allocated_mem);*/

    return ptr;
}



void free(void * ptr)
{
    size_t memsize = 0;

    /* umm ? */
    if(real_free == NULL)
        memwatch_init();

    /* How much are we deleting */
    memsize = malloc_usable_size(ptr);

    /* Free it and subtract from allocated memory counter */
    real_free(ptr);
    allocated_mem -=  memsize;


    /*printf("Freeing %ld bytes\n", memsize);*/
    /*printf("Allocated memory now %ld\n", allocated_mem);*/
}


size_t memwatch_query_allocated_memory(void)
{
    return allocated_mem;
}

size_t memwatch_set_max_memory(size_t max)
{
    size_t cur = maxsize;
    maxsize = max;

    /*! \todo Is this correct to do here? */
    if((allocated_mem > maxsize) && (malloc_fail_hook != NULL))
        malloc_fail_hook(allocated_mem, 0, maxsize);

    return cur;
}

size_t memwatch_get_max_memory(void)
{
    return maxsize;
}

void memwatch_set_malloc_fail_hook(mallocfailptr mhook)
{
    malloc_fail_hook = mhook;
}


int memwatch_running(void)
{
    if(real_free == NULL || real_malloc == NULL)
        return 0;
    else
        return 1;
}

