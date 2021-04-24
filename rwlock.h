#ifndef __RWLOCK_H__
#define __RWLOCK_H__
#include<semaphore.h>
#include <pthread.h>

class RWLock{
private:
    // Synchronization variables
    pthread_mutex_t lock;
    pthread_cond_t readGo;
    pthread_cond_t writeGo;

    int activeReaders;
    int activeWriters;
    int waitingReaders;
    int waitingWriters;

    bool readShouldWait();
    bool writeShouldWait();
    //Your solution to add more private fields
    
public:
    RWLock();
    ~RWLock();
    //Reader
    void startRead();
    void doneRead();
    // Writer
    void startWrite();
    void  doneWrite();
};

#endif
