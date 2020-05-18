//
// Created by Yusuf Sherif on 5/17/20.
//

#ifndef FINAL_Q2_DISPATCH_H
#define FINAL_Q2_DISPATCH_H

#define MAX_WAIT_TIME 1000

typedef struct action_args {
    data* dta;
    void * actor; //reader or a writer
} action_args;

void* write_fn(void* pointer){ //takes ownership of pointer
    action_args *args = (action_args*)pointer; //extracting the passed args
    writer* w = (writer*) args->actor;
    data_write(args->dta,w);
    free(pointer);
}

void* read_fn(void* pointer){ //takes ownership of pointer
    action_args *args = (action_args*)pointer; //extracting the passed args
    reader* r = (reader*) args->actor;
    data_read(args->dta,r);
    free(pointer);
}

typedef struct dispatch_args {
    int count;
    data* dta;
    void* array; //reader array or writer array
} dispatch_args;

void* dispatchReaders_fn(void* pointer) {
    dispatch_args* args = (dispatch_args*) pointer; //extracting the passed args
    pthread_t readers[args->count];
    reader *array = (reader*)args->array;

    //This loop is responsible for creating pthreads to simulate readers requesting to read
    for (int i = 0; i < args->count; ++i) {
        action_args* a_args = (action_args*) calloc(1, sizeof(action_args));
        a_args->dta = args->dta;
        a_args->actor = &array[i];

        pthread_create(&readers[i],NULL,read_fn,a_args);
        usleep((random() % MAX_WAIT_TIME)); //sleep for some random time before another reader comes.
    }
    for (int j = 0; j < args->count; ++j) {
        pthread_join(readers[j],NULL);
    }
    return NULL;
}

void* dispatchWriters_fn(void* pointer) {
    dispatch_args* args = (dispatch_args*) pointer; //extracting the passed args
    pthread_t writers[args->count];
    writer *array = (writer*)args->array;

    //This loop is responsible for creating pthreads to simulate writers requesting to write
    for (int i = 0; i < args->count; ++i) {
        action_args* a_args = (action_args*) calloc(1, sizeof(action_args));
        a_args->dta = args->dta;
        a_args->actor = &array[i];

        pthread_create(&writers[i],NULL,write_fn,a_args); //creation of thread
        usleep((random()%MAX_WAIT_TIME*10)); //sleep for some random time before another writer comes.
        // Multiplied by 10 as we only use writer priority scheme when there is a relatively smaller number of
        // writers and so I'm trying to spread the writers on the given time frame.
    }
    for (int j = 0; j < args->count; ++j) {
        pthread_join(writers[j],NULL);
    }
    return NULL;
}

#endif //FINAL_Q2_DISPATCH_H
