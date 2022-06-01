#ifndef __counter_h__
#define __counter_h__

#include "common_threads.h"
typedef struct __counter_t
{
    int value;
    pthread_mutex_t lock;
} counter_t;

/**
 * @brief Init a global counter.
 * 
 * @param c pointer of the global counter.
 */
void init(counter_t* c);

/**
 * @brief Update counter with specific amount.
 * 
 * @param c pointer of the global counter.
 * @param amount value to be added to the counter.
 */
void update(counter_t* c, int amount);

/**
 * @brief Get value of the counter.
 * 
 * @param c pointer of the global counter.
 * @return int value of the counter.
 */
int get(counter_t* c);

#endif