#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "sloppy_counter.h"

#define COUNTO 40000000

sloppy_counter_t COUNTER; // global sloppy counter.

typedef struct _worker_args
{
    int target;
    int threadID;
} worker_args_t;

void init_counter(sloppy_counter_t *c, int threshold, int cpu_num)
{
    c->global = 0;
    Pthread_mutex_init(&c->global_lock, NULL);
    c->local = (int *)malloc(cpu_num * sizeof(int));
    c->local_locks = (pthread_mutex_t *)malloc(cpu_num * sizeof(pthread_mutex_t));
    for (size_t i = 0; i < cpu_num; i++)
    {
        c->local[i] = 0;
        Pthread_mutex_init(&c->local_locks[i], NULL);
    }
    c->threshold = threshold;
    c->cpu_num = cpu_num;
}

void update(sloppy_counter_t *c, int threadID, int amount)
{
    // find thread running on which CPU.
    int cpu = threadID % c->cpu_num;
    // acquire local lock on running CPU.
    Pthread_mutex_lock(&c->local_locks[cpu]);
    // update local counter.
    c->local[cpu] += amount;
    // if value of local counter >= threshold -> transfer to global.
    if (c->local[cpu] >= c->threshold)
    {
        // acquire global lock.
        Pthread_mutex_lock(&c->global_lock);
        c->global += c->local[cpu];
        // release global lock.
        Pthread_mutex_unlock(&c->global_lock);
        c->local[cpu] = 0;
    }
    // release local lock.
    Pthread_mutex_unlock(&c->local_locks[cpu]);
}

int get(sloppy_counter_t *c)
{
    // acquire global lock.
    Pthread_mutex_lock(&c->global_lock);
    int val = c->global;
    // release global lock.
    Pthread_mutex_unlock(&c->global_lock);
    // return approximate value.
    return val;
}

void *worker(void *arg)
{
    worker_args_t *myarg = (worker_args_t *)arg;
    // increment counter by one.
    for (size_t i = 0; i < myarg->target; i++)
    {
        update(&COUNTER, myarg->threadID, 1);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    // init timer.
    struct timeval start;
    struct timeval end;
    unsigned long diff;

    // get configurations from command line arguments.
    int num_threads = 0;
    int threshold = 0;
    int cpu_num = 0;
    if (argc != 4)
    {
        fprintf(stderr, "usage: %s <num_threads> <threshold> <cpunum>\n", argv[0]);
        fprintf(stderr, "where <num_threads> is the number of threads\n");
        fprintf(stderr, "where <threshold> is the transfer threshold\n");
        fprintf(stderr, "where <cpunum> is the number of CPU cores\n");
    }
    num_threads = atoi(argv[1]);
    threshold = atoi(argv[2]);
    cpu_num = atoi(argv[3]);

    // init sloppy counter.
    init_counter(&COUNTER, threshold, cpu_num);

    // init threads.
    pthread_t *threads_array = NULL;
    threads_array = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    int target = COUNTO / num_threads;

    // init arguments.
    worker_args_t *myargs = (worker_args_t *)malloc(num_threads * sizeof(worker_args_t));
    for (size_t i = 0; i < num_threads; i++)
    {
        myargs[i].target = target;
        myargs[i].threadID = i;
    }

    // create threads.
    // start timer.
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < num_threads; i++)
    {
        Pthread_create(&threads_array[i], NULL, worker, &myargs[i]);
    }

    // join threads.
    for (size_t i = 0; i < num_threads; i++)
    {
        Pthread_join(threads_array[i], NULL);
    }

    // stop timer.
    gettimeofday(&end, NULL);
    // main thread get counter value and print it.
    printf("Counter value: %d\n", get(&COUNTER));
    // print approximate running time.
    diff = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    printf("Time elapsed: %ld ms\n", diff);

    // free thread array.
    free(threads_array);
    threads_array = NULL;

    // free arguments.
    free(myargs);
    myargs = NULL;

    return 0;
}
