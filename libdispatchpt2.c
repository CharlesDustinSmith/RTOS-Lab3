/*
 * Copyright 2018 Jonathan Anderson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <err.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dispatch/dispatch.h>
#include <semaphore.h>

#define JOBS 100 // define JOBS constant
#define WORK_PER_JOB 1000 // define WORK_PER_JOB constant

sem_t my_semaphore;
int shared_resource = 0;

dispatch_function_t increment(int *countp);

int main(int argc, char *argv[])
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

    dispatch_queue_t que = dispatch_queue_create(NULL, NULL);
    dispatch_group_t groups = dispatch_group_create();

    for (int i = 0; i < JOBS; i++)
    {
        dispatch_group_async_f(groups, que, &counter, increment(&counter));
    }

    dispatch_group_wait(groups, DISPATCH_TIME_FOREVER);
    dispatch_release(groups);

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end) != 0)
    {
        err(-1, "Failed to get end time");
    }

    // release semaphore
    sem_destroy(&my_semaphore);

    long diff = end.tv_nsec - begin.tv_nsec;
    diff += (1000 * 1000 * 1000) * (end.tv_sec - begin.tv_sec);

    printf("%f\n", ((double) diff));

    return 0;
}

dispatch_function_t increment(int *countp)
{
    for (int i = 0; i < WORK_PER_JOB; i++)
    {
        sem_wait(&my_semaphore);
        (*countp)++;
        sem_post(&my_semaphore);
    }
}
