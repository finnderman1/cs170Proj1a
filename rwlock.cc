#include<stdio.h>
#include <pthread.h>
#include <iostream> 

#include "rwlock.h"
#include <cassert>

//Your solution to implement each of the following methods
//
RWLock::RWLock() {
    pthread_mutex_init( &(lock), NULL);
    pthread_cond_init(&(readGo), NULL);
    pthread_cond_init(&(writeGo), NULL);

    activeReaders = 0;
    activeWriters = 0;
    waitingReaders = 0;
    waitingWriters = 0;
 }
RWLock::~RWLock() { }
void RWLock::startRead() {
    pthread_mutex_lock(&lock);
    waitingReaders++;
    while (activeWriters > 0 || waitingWriters > 0) {
        pthread_cond_wait(&readGo, &lock);
    }
    waitingReaders--;
    activeReaders++;
    pthread_mutex_unlock(&lock);
}

void RWLock::doneRead() {
    pthread_mutex_lock(&lock);
    activeReaders--;
    if (activeReaders == 0 
         && waitingWriters > 0) {
        pthread_cond_signal(&writeGo);
    }
    pthread_mutex_unlock(&lock);
}

void RWLock::startWrite() {
    pthread_mutex_lock(&lock);
    waitingWriters++;
    while (activeWriters > 0 || activeReaders > 0) {
        pthread_cond_wait(&writeGo, &lock);
    }
    waitingWriters--;
    activeWriters++;
    pthread_mutex_unlock(&lock);
}

void RWLock::doneWrite() {
    pthread_mutex_lock(&lock);
    activeWriters--;
    assert(activeWriters == 0);
    if (waitingWriters > 0) {
        pthread_cond_signal(&writeGo);
    } 
    else {
        pthread_cond_broadcast(&readGo);
    }
    pthread_mutex_unlock(&lock);
}