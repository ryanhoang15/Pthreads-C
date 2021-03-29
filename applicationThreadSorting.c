/**
 * proj_2
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Ryan Hoang
 * CECS 326 
 * Due Date: 3/14/2021
 */


void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

// creating the array size
#define size 10
// creating the unsorted array
int array[size] = {7,12,19,3,18,4,2,6,15,8};
// initializing the sorted array
int sortedArray[size];

// creating a struct that can hold several data types
struct thread_data {
    // varaible to hold the start index
    int *startIndex;
    // vaaible to hold the last index
    int *endIndex;
};

// This method's purpose is to the array's
// first half and second half through the 
// void *params since we are passing in a struct
// and the struct hold the start and end index of the array
void *sorter(void *params){
    // these variable are to get the start and end value for the index of the array
    int start = ((struct thread_data *)params)->startIndex;
    int end = ((struct thread_data *)params)->endIndex;

    // Looping through the array using bubble sort on
    // the first half or second half of the array
    for(int i = start; i < end; i++){
        for(int j = start; j < end - 1; j++){
            if(array[j] > array[j + 1]){
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

// this function's purpose is to merge the first and second half
// of the array. We are passing in the our struct varaiable 
// since it holds the indexes of where we need to begin sorting
// and merging it
void *merger(void *params){
    // the variables for the length of the first half and second half of the array
    int firstArrHalf_length = size / 2;
    int secondArrHalf_length = size;

    // the variables for the index of the first half and second half of the array
    int firstIndex = ((struct thread_data *)params)->startIndex;
    int secondIndex = ((struct thread_data *)params)->endIndex;
    // for the sortedArray index count
    int count = 0;

    // looping through the array's and comparing the value from the first half
    // and second half of the array
    while(firstIndex < firstArrHalf_length && secondIndex < secondArrHalf_length){
        if(array[firstIndex] < array[secondIndex]){
            sortedArray[count] = array[firstIndex];
            firstIndex++;
        }
        else{
            sortedArray[count] = array[secondIndex];
            secondIndex++;
    
        }
        count++;
    }

    // adding the leftover value on the first half the array
    while(firstIndex < firstArrHalf_length){
        sortedArray[count] = array[firstIndex];
        firstIndex++;
        count++;
    }

    // adding the leftover value of the second half of the array
    while(secondIndex < secondArrHalf_length){
        sortedArray[count] = array[secondIndex];
        secondIndex++;
        count++;
    }
}


int main (int argc, const char * argv[]) {
    //initializing a pthread array
	pthread_t arrayThread[3];
    // creating a struct thread_data pointer variable
    struct thread_data *indexes = (struct thread_data *)malloc(sizeof(struct thread_data));

    // displaying the value of the array in unsorted order
    printf("\n Unsorted Array: ");
    for(int a = 0; a < size; a++){
        printf("%d ", array[a]);
    }
    
    // assigning the struct variable values of the first half of the array's indexes
    indexes->startIndex = 0;
    indexes->endIndex = (size / 2);
    // creating the thread for the first half of the array
    pthread_create(&arrayThread[0], NULL, &sorter, (void*)indexes);
    // joining the first half of the array
    pthread_join(arrayThread[0], NULL);

    // displaying the value of the array's where the first half is sorted
    printf("\n Sorted First Half of the Array: ");
    for(int k = 0; k < size; k++){
        printf("%d ", array[k]);
    }

    // assigning the struct variable values of the second half of the array's indexes
    indexes->startIndex = (size / 2);
    indexes->endIndex = size;
    // creating the thread for the second half of the array
    pthread_create(&arrayThread[1], NULL, &sorter, (void*)indexes);
    // joining the second half of the array
    pthread_join(arrayThread[1], NULL);

    // displaying the value of the array's where the second half is now sorted
    printf("\n Sorted Second Half of the Array: ");
    for(int k = 0; k < size; k++){
        printf("%d ", array[k]);
    }

    // assigning the struct variable values of the array's indexes to be merged
    indexes->startIndex = 0;
    indexes->endIndex = size / 2;
    // creating the thread to merge the first half and second half of the array's
    pthread_create(&arrayThread[2], NULL, &merger, (void*)indexes);
    // joining the merge array together
    pthread_join(arrayThread[2], NULL);

    // displaying the value of the array after merging 
    printf("\n Sorted Merge Array: ");
    for(int m = 0; m < size; m++){
        printf("%d ", sortedArray[m]);
    }

    printf("\n");

}
