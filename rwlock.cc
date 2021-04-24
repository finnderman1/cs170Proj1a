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
    while (readShouldWait()) {
        pthread_cond_wait(&readGo, &lock);
    }
    waitingReaders--;
    activeReaders++;
    pthread_mutex_unlock(&lock);
}

// Done reading. If no other active
// reads, a write may proceed.
void RWLock::doneRead() {
    pthread_mutex_lock(&lock);
    activeReaders--;
    if (activeReaders == 0 
         && waitingWriters > 0) {
        pthread_cond_signal(&writeGo);
    }
    pthread_mutex_unlock(&lock);
}

// Read waits if any active or waiting
// write ("writers preferred").
bool RWLock::readShouldWait() {
    return (activeWriters > 0 
             || waitingWriters > 0);
}


// Wait until no active read or
// write then proceed.
void RWLock::startWrite() {
    pthread_mutex_lock(&lock);
    waitingWriters++;
    while (writeShouldWait()) {
        pthread_cond_wait(&writeGo, &lock);
    }
    waitingWriters--;
    activeWriters++;
    pthread_mutex_unlock(&lock);
}

// Done writing. A waiting write or
// read may proceed.
void RWLock::doneWrite() {
    pthread_mutex_lock(&lock);
    activeWriters--;
    assert(activeWriters == 0);
    if (waitingWriters > 0) {
        pthread_cond_signal(&readGo);
    } 
    else {
        pthread_cond_broadcast(&readGo);
    }
    pthread_mutex_unlock(&lock);
}

// Write waits for active read or write.
bool RWLock::writeShouldWait() {
    return (activeWriters > 0 
             || activeReaders > 0);
}
// void RWLock::startRead() { }
// void RWLock::doneRead() { }
// void RWLock::startWrite() { }
// void RWLock::doneWrite() { }
