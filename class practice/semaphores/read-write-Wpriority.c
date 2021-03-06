#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread.h"
#include "semaphore.h"

sem_t *me_reader1, *me_reader2, *reading, *me_writter, *writting;

int nr = 0;
int nw = 0;

static void * reader(void *value){
    int *v = (int *) value;
    sleep(1);
    sem_wait(me_reader1);
        sem_wait(me_reader2);
            sem_wait(reading);
                nr++;
                if(nr == 1)
                    sem_wait(writting);
            sem_post(reading);
        sem_post(me_reader2);
    sem_post(me_reader1);

    printf ("Reading : %d\n", *v);
    sleep (1);

    sem_wait(reading);
        nr--;
        if(nr == 0)
            sem_post(writting);
    sem_post(reading);

    return NULL;
}


static void * writer(void *value){
    int *v = (int *) value;

    sem_wait(me_writter);
        nw++;
        if(nw == 1)
            sem_wait(me_reader2);
    sem_post(me_writter);
    sem_wait(writting);
        printf ("Writing: %d\n", *v);
        sleep(15);
    sem_post(writting);
    sem_post(me_writter);
        nw--;
        if(nw ==1)
            sem_post(me_reader2);
    sem_post(me_writter);

    return NULL;
}



int main (void){

    pthread_t th_reader;
    pthread_t th_writer;

    int i, *v;


    me_reader1 = (sem_t *) malloc (sizeof (sem_t));
    me_reader2 = (sem_t *) malloc (sizeof (sem_t));
    reading = (sem_t *) malloc (sizeof (sem_t));
    me_writter = (sem_t *) malloc (sizeof (sem_t));
    writting = (sem_t *) malloc (sizeof (sem_t));

    sem_init(me_reader1, 0, 1);
    sem_init(me_reader2, 0, 1);
    sem_init(reading, 0, 1);
    sem_init(me_writter, 0, 1);
    sem_init(writting, 0, 1);


    for (i = 0; i < 10; i++) {
        v = (int *) malloc (sizeof (int));
        *v = i;
        pthread_create(&th_reader, NULL, reader, v); 
    }

    for (i = 0; i < 10; i++) {
        v = (int *) malloc (sizeof (int));
        *v = i;
        sleep(1);
        pthread_create(&th_writer, NULL, writer, v); 
    }




}