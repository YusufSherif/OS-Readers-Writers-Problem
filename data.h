//
// Created by Yusuf Sherif on 5/17/20.
//

#ifndef FINAL_Q2_DATA_H
#define FINAL_Q2_DATA_H

#include <semaphore.h>

//Through the use of the following semaphores, the idea would be modelled as an abstract priority queue, where
//writers will be able to skip the line to the front of the queue and start writing as soon as all readers have
//stopped reading or the current writer has finished writing.

typedef struct data {
    sem_t reader_lock; //Used in data_write so that writers can stop readers from entering and in data_read so that the
    //readers wait until all other writers have finished.
    sem_t writer_lock; //To stop other writers from writing at the same time and used to block reading and writing at
    //the same time.

    sem_t mutex_w; //This semaphore is there so we can ensure the integrity of the writers field.
    sem_t mutex_r; //This semaphore is there so we can ensure the integrity of the readers field.

    int readers; //Keep track of number of readers reading or wanting to read.
    int writers; //Keep track of number of writers waiting to write.

    long data; //The actual data.
} data;

//Initializing all semaphores to one and the number of readers, writers, and the data field to zeros.
void data_init(data* d){
    d->data = 0;
    d->readers = 0;
    d->writers = 0;
    sem_init(&d->reader_lock, 0,1);
    sem_init(&d->writer_lock, 0,1);
    sem_init(&d->mutex_r, 0, 1);
    sem_init(&d->mutex_w, 0, 1);
}

//self explanatory.
void data_destroy(data* d){
    sem_destroy(&d->mutex_w);
    sem_destroy(&d->mutex_r);
    sem_destroy(&d->writer_lock);
    sem_destroy(&d->reader_lock);
}

#endif //FINAL_Q2_DATA_H
