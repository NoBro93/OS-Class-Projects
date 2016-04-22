/* Multithreaded search implementation for Unix
 * Nora Brown
 * Creates a random large array, then prompts for 
 * a value to search for, and the number of threads to use    
 * note: must be linked with pthread when compiled
 */
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

//global variables are shared by all threads 

//Define the struct to be passed to each search thread
struct ThreadParam {
    int * array;
    int value; 
    int startIndex;
    int endIndex;
}; 
//Define struct to be returned from each thread
struct Found {
    int num;
    int index;
};

//To pass the function to pthread_create, 
//must take generic pointer as parameter,
//and return a generic pointer  
void * Search  (void * arg)
{
    ThreadParam * param = (ThreadParam *) arg;
    Found * result = new Found;    //create pointer to struct to return 
    result->num = 0;
    result->index = -1;

    for (int i=param->startIndex; i<param->endIndex; i++){
        if (param->array[i]==param->value){
            result->num++;
            if (result->index == -1){
                result->index = i;
            }
        }
    }
    return (void*)result; //cast result to generic pointer, so can return it
}


int main(int argc, char * argv[])
{
    pthread_t * threads;
    ThreadParam * threadsParams;

    const int MAX_SIZE=5000;
    int BigArray[MAX_SIZE];
    int length = 0;

    int thread_num; 
    int status, i;
    int value;

    //create an array to test with
    for (int j=0; j<5000; j++){
        BigArray[j] = random() % 100 + 1;
        length++;
    }

    cout <<"Value to search for:";
    cin >> value;

    do {
        cout <<"Number of threads to use:";
        cin >> thread_num; 
    } while (thread_num<=1);

    threads = new pthread_t[thread_num]; //dynamically allocate the thread handler array
    threadsParams = new ThreadParam[thread_num]; //and the array for parameters

    for (i=0; i<thread_num; i++){
        //prepare threadsParams
        threadsParams[i].array = BigArray;
        threadsParams[i].value = value; 
        threadsParams[i].startIndex = i*(length/thread_num);
        threadsParams[i].endIndex = (i+1)*(length/thread_num);
        
        status = pthread_create (&threads[i], NULL, Search, (void *)&(threadsParams[i]));
        if (status!=0){
            printf ("oops, pthread_create returned error code %d\n", status);
            exit(-1);

        }

    }

    void * result; 
    Found * res;
    for (i=0; i<thread_num; i++){
        pthread_join (threads[i],(void **)&result); //result will point to the data that the thread returns
        res = (Found*)result; //cast generic pointer back to Found struct
        cout << "Thread " << i << " result: " << res->num << " found. ";
	if (res->num>0)
		cout << "First found at " << res->index << endl;
	else
		cout << endl;
    }


}
