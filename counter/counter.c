#include "counter.h"
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

#define COUNTO 1000000

counter_t COUNTER; // global counter.

void init(counter_t* c)
{
    c->value = 0;
    Pthread_mutex_init(&c->lock, NULL);
}

void update(counter_t* c, int amount)
{
    // acquire lock first.
    Pthread_mutex_lock(&c->lock);
    // update counter.
    c->value = c->value + amount;
    // release lock.
    Pthread_mutex_unlock(&c->lock);
}

int get(counter_t* c)
{
    // acquire lock first.
    Pthread_mutex_lock(&c->lock);
    // get counter value;
    int value = c->value;
    // release lock.
    Pthread_mutex_unlock(&c->lock);
    return value;
}
/**
 * @brief Our worker rountine, pass in a target value to accumulate counter to that value.
 * 
 * @param arg target value.
 * @return void* no return.
 */
void* worker(void* arg)
{
    int target = *((int*) arg);
    // incremet counter by one.
    for (size_t i = 0; i < target; i++)
    {
        update(&COUNTER, 1);
    }
    return NULL;
}


int main(int argc, char const *argv[])
{
    // get num of threads
    int num_threads = 0;
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        fprintf(stderr, "where <n> is the number of threads\n");
        return 1;
    }
    num_threads = atoi(argv[1]);
    pthread_t* threads_array = NULL;
    threads_array = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
    int target = COUNTO / num_threads;

    // create threads.
    for (size_t i = 0; i < num_threads; i++)
    {
        Pthread_create(&threads_array[i], NULL, worker, &target);
    }

    // join threads.
    for (size_t i = 0; i < num_threads; i++)
    {
        Pthread_join(threads_array[i], NULL);
    }

    // main thread get counter value and print it.
    printf("Counter value: %d\n", get(&COUNTER));

    // free thread array.
    free(threads_array);
    threads_array = NULL;

    return 0;
}
