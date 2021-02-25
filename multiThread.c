#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

/**
 * Ryan Hoang
 * CECS 326
 * 2/21/2021
 */

// variable will be use in the *SimpleThread Function
int sharedVariable = 0;
// A shared mutex
pthread_mutex_t mutex;
// A barrier variable
pthread_barrier_t barrier;

// function's purpose is to create the pthreads
// through synchronization and non-synchronization
void *SimpleThread(void *which){
    // variable used for the for loop
    int num;
    // variable used for the pthread's value
    int val;
    
    // this loop is used for the pthreads and
    // displaying its result
    for(num = 0; num < 20; num++){
        // there is a time lapse
        if(random() > RAND_MAX/2){
            usleep(500);
        }
        #ifdef SYNC
        // Locks the thread
        pthread_mutex_lock(&mutex);
        #endif

        val = sharedVariable;
        printf("***thread %d sees value %d\n",(int) which, val);
        // incrementing the sharedVariable by 1
        sharedVariable = val + 1;

        #ifdef SYNC
        // Unlock the thread
        pthread_mutex_unlock(&mutex);
        #endif
    }
    #ifdef SYNC
    // barrier that allowed the shared variable to display the final value.
    pthread_barrier_wait(&barrier);
    #endif
    val = sharedVariable;
    printf("Thread %d sees final value %d\n", (int) which, val);
}

int main(int argc, char *argv[]){
    //checking the validiation of argc to see if its valid
    if(argc != 2){
        printf("Invaild Argument!!!\n");
        printf("Has to be one command line parameter\n");
    }

    //atoi() will convert a string to int
    int threadNums = atoi(argv[1]);
    if(threadNums < 1){
        printf("Cannot be a negative number or string\n");
        printf("Must be a POSITIVE number\n");
    }
    
    // initialize a pthread_t array
    pthread_t newThread[threadNums];

    //Creating the mutex and checking if it was initialize correctly
    if(pthread_mutex_init(&mutex, NULL)){
        printf("Unable to initialize a mutex\n");
    }

    //Creating the barrier and checking if it was initialize correctly
    if(pthread_barrier_init(&barrier, NULL, threadNums)){
        printf("Could not create a barrier\n");
    }

    for(int i =0; i < threadNums; i++){
        //creating the pthread
        pthread_create(&newThread[i], NULL, SimpleThread, (void *) i);
    }

    for(int j = 0; j < threadNums; j++){
        //joining the pthread
        pthread_join(newThread[j], NULL);
    }

    return 0;
}
