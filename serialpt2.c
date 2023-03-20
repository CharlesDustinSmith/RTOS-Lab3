#include <err.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

sem_t my_semaphore;
int shared_resource = 0;

void increment(int *countp);

int
main(int argc, char *argv[])
{
    int counter = 0;
    struct timespec begin, end;

    // Set C locale settings to get niceties like thousands separators
    // for decimal numbers.
    setlocale(LC_NUMERIC, "");

    // initialize semaphore
    if (sem_init(&my_semaphore, 0, 1) != 0)
    {
        err(-1, "Failed to initialize semaphore");
    }

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &begin) != 0)
    {
        err(-1, "Failed to get start time");
    }

    for (int i = 0; i < JOBS; i++)
    {
        increment(&counter);
    }

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end) != 0)
    {
        err(-1, "Failed to get end time");
    }

    long diff = end.tv_nsec - begin.tv_nsec;
    diff += (1000 * 1000 * 1000) * (end.tv_sec - begin.tv_sec);

    printf("Counter: %'d\n", counter);

    return 0;
}

void increment(int *countp)
{
    for (int i = 0; i < WORK_PER_JOB; i++)
    {
        sem_wait(&my_semaphore);
        (*countp)++;
        sem_post(&my_semaphore);
    }
}