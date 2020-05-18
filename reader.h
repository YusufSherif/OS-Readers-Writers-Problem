//
// Created by Yusuf Sherif on 5/17/20.
//

#ifndef FINAL_Q2_READER_H
#define FINAL_Q2_READER_H

#include "data.h"

typedef struct reader {

long data_read;
int id;

} reader;

void reader_init(reader* r, int id){
    r->data_read = 0; //Initially data is set to zero
    r->id = id;
}

void reader_read(reader* r,data* d){
    r->data_read = d->data;
    printf("Reader %i read data %li\n", r->id, r->data_read);
}

void data_read(data* d, reader* r){
    //Wait until the lock from writers is released, if it was. This line was included for this purpose only. reader_lock
    //is released as soon as we are done with the critical section.
    sem_wait(&d->reader_lock);

    sem_wait(&d->mutex_r);
    //If this is the first reader to enter the read queue, then block any writer from writing until reading operation is completed.
    if(d->readers==0) {
        sem_wait(&d->writer_lock);
    }
    d->readers++;
    sem_post(&d->mutex_r);
    //Note: this critical section does not block writers until all readers are done as in the example given to us in class.
    //This is because if a writer were to request writing, as soon as it does that, it will wait on the reader_lock,
    //which would in turn stop any more readers from reading until that writer, or all the
    //other writers that have followed have already finished.
    sem_post(&d->reader_lock);

    reader_read(r, d);
    //Do read operation. Notice how it doesn't have semaphores around it. This ensures that without the presence of writers,
    //any number of readers can read simultaneously.

    sem_wait(&d->mutex_r);
    d->readers--;
    //If last reader in the reading queue, then release writer_lock. Note that if a writer were to come in into the queue,
    //it would immediately stop any more readers from getting to this point due to the locking of the reader_lock. Thus,
    //if Reader1 entered then entered Reader2 and Writer1 at the same time, and the Writer1 thread was able to lock reader_lock
    //before Reader2, then readers would be equal to zero at this point for Reader1 because Reader2 didn't manage to enter
    //the critical section above. Thus, as soon as all readers currently reading stop reading and a writer is present,
    //the writer is guaranteed to step into action.
    if(d->readers==0)
        sem_post(&d->writer_lock);
    sem_post(&d->mutex_w);
}
#endif //FINAL_Q2_READER_H
