/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Alvi Alam
 *
 * Created on February 17, 2025, 9:14 AM
 */

#include  <stdio.h> /* printf */
#include  <sys/types.h> /* pid_t */
#include <unistd.h> /* get_pid */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <sys/wait.h>  /* wait */
#include <pthread.h>
#include <sys/stat.h>

#define MAX_THREADS 5 //Number of threads to be created (5)

void * count(void *);
int globalNumber = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; //Create a mutex

int main(void) {
    int i;
    pthread_t counterThread[MAX_THREADS];

    //Create the 5 threads
    for (i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&counterThread[i], NULL, count, NULL) != 0) { //create a new thread
            perror("Failed Creating Thread"); 
            exit(EXIT_FAILURE);
        }
    }
    
    //Wait for all treads to finish
    for (i = 0; i < MAX_THREADS; i++) { 
        pthread_join(counterThread[i], NULL); //Wait for current thread to finish executing
    }

    printf("Final Counter/globalNumber: %d\n", globalNumber); //print final count
    return (0);
}

void * count(void * junk) {
    int loopCount = 0;
    pthread_mutex_lock(&mutex1);
    while (loopCount < 10) {
        int tmpNumber = globalNumber;
        printf("counter: %d, Thread: %ld, PID: %d\n", tmpNumber, pthread_self(), getpid());
        tmpNumber = globalNumber;
        tmpNumber++;
        usleep(random() % 2);
        globalNumber = tmpNumber;
        loopCount++;
    }
    pthread_mutex_unlock(&mutex1);
    return (0);
}
