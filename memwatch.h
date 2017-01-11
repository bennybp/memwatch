/* \file
 *
 * \brief Total program memory watcher
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MEMWATCH__MEMWATCH_H_
#define PULSAR_GUARD_MEMWATCH__MEMWATCH_H_

#include <stddef.h> /* for size_t */


#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Pointer to a callback that gets called when we are over the limit
 * 
 * Arguments given to the callback (in order): Currently allocated, new allocation, memory limit
 */ 
typedef void (*mallocfailptr)(size_t, size_t, size_t);




/*! \brief Get how memory is currently used (in bytes)
 */
size_t memwatch_query_allocated_memory(void);


/*! \brief Set the maximum amount of memory (in bytes)
 *
 * If an allocation will go above this limit, the callback
 * (set via memwatch_set_malloc_fail_hook) will be called.
 *
 * \return The previous limit
 */
size_t memwatch_set_max_memory(size_t max);


/*! \brief Get the current maximum memory to be used (in bytes)
 */
size_t memwatch_get_max_memory(void);


/*! \brief Set the callback function
 */
void memwatch_set_malloc_fail_hook(mallocfailptr mhook);


/* \brief See if memwatch was loaded and is running
 */
int memwatch_running(void);



#ifdef __cplusplus
}
#endif

#endif
