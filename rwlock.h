#ifndef __RWLOCK_H__
#define __RWLOCK_H__
#include<semaphore.h>

class RWLock{
private:
    // Synchronization variables
    Lock lock;
    CV readGo;
    CV writeGo;

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
