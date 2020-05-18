//
// Created by Yusuf Sherif on 5/17/20.
//

#ifndef FINAL_Q2_WRITER_H
#define FINAL_Q2_WRITER_H

#include <stdlib.h>
#include "data.h"

typedef struct writer {

long data_to_write;
int id;

} writer;

void writer_init(writer* w, int id){
    w->data_to_write = rand();
    w->id = id;
}

void writer_write(writer* w, data* d){
    d->data = w->data_to_write;
    printf("Writer %i wrote data %li\n", w->id, w->data_to_write);
}

void data_write(data* d, writer* w){
    //since we're going to be using the count of writers and change it in every data_write operation, then the following
    //is a critical section as they are not atomic operations.
    sem_wait(&d->mutex_w);
    if(d->writers==0) {//If this is the first writer to enter the write queue, then block all readers from reading.
        sem_wait(&d->reader_lock);
    }
    d->writers++; //Increment writer count
    sem_post(&d->mutex_w); //end of critical section, release lock.

    sem_wait(&d->writer_lock); //Start of critical section, one writer writes at a time.
    writer_write(w,d);
    sem_post(&d->writer_lock); //end of critical section, release lock.

    sem_wait(&d->mutex_w); //Again, for keeping the integrity of writer count
    d->writers--; //Decrement writer count
    if(d->writers==0) //If finally, no more writers in the queue, then release readers lock, allowing readers to read once again.
        sem_post(&d->reader_lock);
    sem_post(&d->mutex_w); //end of critical section, release lock.

}

#endif //FINAL_Q2_WRITER_H
