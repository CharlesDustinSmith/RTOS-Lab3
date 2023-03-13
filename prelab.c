#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 5 

void *print_hello(void *thread_id)
{
    long tid = (long)thread_id;
    printf("Hello from thread %ld\n", tid);
    pthread_exit(NULL);
}


int main(void)
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    for(t = 0; t < NUM_THREADS; t++)
    {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, print_hello, (void *)t);

        if(rc) 
        {
            printf("ERROR; return code from pthread_create() is %d", rc);
            return 1;
        }
    }
    pthread_exit(NULL);
    return 0;
}
