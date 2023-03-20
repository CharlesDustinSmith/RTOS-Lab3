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
#include <pthread.h>

#define JOBS 100 // define JOBS constant
#define WORK_PER_JOB 1000 // define WORK_PER_JOB constant

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *countp);

int main(int argc, char *argv[])
{
    int counter = 0;
    struct timespec begin, end;

    // Set C locale settings to get niceties like thousands separators
    // for decimal numbers.
    setlocale(LC_NUMERIC, "");

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &begin) != 0)
    {
        err(-1, "Failed to get start time");
    }

    pthread_t threads[JOBS];
    int rc;
    for (int i = 0; i < JOBS; i++)
    {
        rc = pthread_create(&threads[i], NULL, increment, &counter);
        if (rc != 0)
        {
            err(-1, "Failed to create thread");
        }
    }
    for (int i = 0; i < JOBS; i++)
    {
        rc = pthread_join(threads[i], NULL);
        if (rc != 0)
        {
            err(-1, "Failed to join thread");
        }
    }

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end) != 0)
    {
        err(-1, "Failed to get end time");
    }

    long diff = end.tv_nsec - begin.tv_nsec;
    diff += (1000 * 1000 * 1000) * (end.tv_sec - begin.tv_sec);

    printf("Counter: %ld\n", counter);

    return 0;
}

void *increment(void *countp)
{
    for (int i = 0; i < WORK_PER_JOB; i++)
    {
        pthread_mutex_lock(&mutex);
        (*((int *)countp))++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
