/*
 * main.cpp
 *
 * Serial version
 *
 * Compile with -O2
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "skiplist2.h"
#include <pthread.h>
#define buffer_size 100

using namespace std;
// thread and locks
pthread_mutex_t buffer_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_nonempty=PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_nonfull=PTHREAD_COND_INITIALIZER;
pthread_mutex_t list_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sum_odd_lock=PTHREAD_MUTEX_INITIALIZER;

// aggregate variables
long sum = 0;
long odd = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;


skiplist<int, int> list(0,1000000);
char buffer1[buffer_size];
int buffer2[buffer_size];
int put=0;
int get=0;
int t=2;
int t_end=0;
int idle=0;

// function prototypes

void *worker(void* p_id){
    while(1){
    pthread_mutex_lock(&buffer_lock);
    while(put==get){
        if(t_end){
            pthread_mutex_unlock(&buffer_lock);
            pthread_exit(NULL);
        }
        pthread_cond_wait(&buffer_nonempty,&buffer_lock);
    }
    char action=buffer1[get];
    int num=buffer2[get];
    get=(get+1)%buffer_size;
    pthread_cond_signal(&buffer_nonfull);
    pthread_mutex_unlock(&buffer_lock);

    if (action == 'i') {            // insert
        pthread_mutex_lock(&list_lock);
        list.insert(num,num);
        pthread_mutex_unlock(&list_lock);
        // update aggregate variables
        pthread_mutex_lock(&sum_odd_lock);
        sum += num;
        if (num % 2 == 1) {
            odd++;
        }
        pthread_mutex_unlock(&sum_odd_lock);
    }
    else if (action == 'q') {      // qeury
        if(list.find(num)!=num)
    cout << "ERROR: Not Found: " << num << endl;
    } else if (action == 'w') {     // wait
        usleep(num*1000);
    } else {
        printf("ERROR: Unrecognized action: '%c'\n", action);
        exit(EXIT_FAILURE);
    }
    printf("I do: %c %d\n",action,num);
    if(t_end){
        pthread_exit(NULL);
    }
}
}



int main(int argc, char* argv[])
{
    struct timespec start, stop;

    // check and parse command line options
    if (argc != 3) {
        printf("Usage: sum <infile>\n");
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    int num_thread=atoi(argv[2]);
    pthread_t tid[num_thread];

    clock_gettime( CLOCK_REALTIME, &start);
    for(int temp=0;temp<num_thread;temp++){
        pthread_create(&tid[temp],NULL,worker,(void*)tid[temp]);
    }
    // load numbers and add them to the queue
    FILE* fin = fopen(fn, "r");
    char action;
    long num;
    while ((t=fscanf(fin, "%c %ld\n", &action, &num)) == 2) {
        pthread_mutex_lock(&buffer_lock);
        while((put>get&&(put-get)==buffer_size-1)
            ||(put<get&&(put+get)==buffer_size-1)){
            pthread_cond_wait(&buffer_nonfull,&buffer_lock);
        }
        buffer1[put]=action;
        buffer2[put]=num;
        put=(put+1)%buffer_size;
        pthread_cond_signal(&buffer_nonempty);
        pthread_mutex_unlock(&buffer_lock);
    }
    while(t_end!=2){
        if(t!=2 && put==get){
            t_end=1;
            pthread_cond_broadcast(&buffer_nonempty);
            for(int temp=0;temp<num_thread;temp++){
                pthread_join(tid[temp],NULL);
                if(temp==num_thread-1){
                    t_end=2;
                }
            }
        }
    }
    fclose(fin);
    clock_gettime( CLOCK_REALTIME, &stop);

    // print results
    cout << list.printList() << endl;
    cout << sum << " " << odd << endl;
    cout << "Elapsed time: " << (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION << " sec" << endl;

    // clean up and return
    return (EXIT_SUCCESS);

}

