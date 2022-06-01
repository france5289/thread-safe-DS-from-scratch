#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#define NUM_ELEMENTS 4096

int main(int argc, char const *argv[])
{
    struct timeval start;
    struct timeval end;
    unsigned long diff;
    int arr[NUM_ELEMENTS];
    // init array.
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < NUM_ELEMENTS; i++)
    {
        arr[i] = 0;
    }
    gettimeofday(&end, NULL);
    // print time elapsed in microseconds.
    diff = 1000000*(end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec);
    printf("It takes %ld microseconds to initialize an array\n", diff);

    // `clock()` is not as accurate as `gettimeofday`.
    // cause it measure in seconds.
    clock_t start_cpu_time = clock();
    for (size_t i = 0; i < NUM_ELEMENTS; i++)
    {
        arr[i] = 1;
    }
    clock_t end_cpu_time = clock();
    double elapsed = (double)(end_cpu_time - start_cpu_time)/CLOCKS_PER_SEC;
    printf("Time measured: %.3f seconds.\n", elapsed);
    return 0;
}
