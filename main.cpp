#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "reader.h"
#include "writer.h"
#include "dispatch.h"

int main() {
    srand(time(NULL));
    data dta;
    data_init(&dta);

    int reader_count = 300, writer_count = 35;

    //Creating readers and writers
    writer w_array[writer_count];
    for (int j = 0; j < writer_count; ++j) {
        writer_init(&w_array[j],j);
    }
    reader r_array[reader_count];
    for (int j = 0; j < reader_count; ++j) {
        reader_init(&r_array[j],j);
    }

    pthread_t read_dispatcher, write_dispatcher;
    dispatch_args d_r_args, d_w_args;
    d_r_args.count = reader_count;
    d_w_args.count = writer_count;
    d_r_args.dta = &dta;
    d_w_args.dta = &dta;
    d_r_args.array = r_array;
    d_w_args.array = w_array;

    pthread_create(&read_dispatcher, NULL, dispatchReaders_fn, &d_r_args);
    pthread_create(&write_dispatcher, NULL, dispatchWriters_fn, &d_w_args);

    pthread_join(read_dispatcher,NULL);
    pthread_join(write_dispatcher,NULL);
}
