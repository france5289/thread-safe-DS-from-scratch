#ifndef __sloppy_counter_h__
#define __sloppy_counter_h__

#include "common_threads.h"
#include <unistd.h>

typedef struct __sloppy_counter_t
{
    int global;                   // global count
    pthread_mutex_t global_lock;        // lock for global count
    int *local;                   // per-CPU count
    pthread_mutex_t *local_locks; // per-CPU lock
    int threshold;                // update frequency
    int cpu_num;                  // use how many CPUs.
} sloppy_counter_t;

/**
 * @brief Initialize a `sloppy_counter_t` type variable with specific threshold.
 *
 * @param c `sloppy_counter_t` object.
 * @param threshold threshold value.
 * @param cpu_num number of CPU cores.
 */
void init_counter(sloppy_counter_t *c, int threshold, int cpu_num);

/**
 * @brief Update a sloppy counter with specific threadID and amount.
 *
 * @param c `sloppy_counter_t` object.
 * @param threadID ID of caller thread.
 * @param amount specific value to update counter.
 */
void update(sloppy_counter_t *c, int threadID, int amount);

/**
 * @brief Get the value of the global counter.
 *
 * @param c `sloppy_counter_t` object.
 * @return int value of the global counter.
 */
int get(sloppy_counter_t *c);

#endif