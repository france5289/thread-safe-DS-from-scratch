#ifndef __sloppy_counter_h__
#define __sloppy_counter_h__

#include "common_threads.h"
#include <unistd.h>

#define NUMCPUS 4
typedef struct __sloppy_counter_t
{
    int global; // global count
    pthread_mutex_t glock; // lock for global count
    int local[NUMCPUS]; // per-CPU count
    int local_locks[NUMCPUS]; // per-CPU lock
    int threshold; // update frequency
} sloppy_counter_t;

/**
 * @brief Initialize a `sloppy_counter_t` type variable with specific threshold.
 * 
 * @param counter `sloppy_counter_t` object.
 * @param threshold threshold value
 */
void init_counter(sloppy_counter_t* counter, int threshold);



#endif