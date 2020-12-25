#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include "semaphore.h"


#define BUFFER_SIZE 16

sem_t *empty, *full;
int in, out ;
int buffer[BUFFER_SIZE];

static void *producer(void *data){

    int n;
    for (n =0; n < 100; n++){
        printf(" %s \n" , "put");
        printf ("%d \n", n);
        sem_wait(empty);
        buffer[in] = n;
        in = (in + 1) % BUFFER_SIZE;
        sem_post(full);
    }

    sem_wait(empty);
    buffer[in] = -1;
    in = (in + 1) % BUFFER_SIZE;
    sem_post(full);
      return NULL;

} 

static void *consumer(void *data){
    int n;
    while (1){
        int get;
        sem_wait(full);
        get = buffer[out];
        if (get == -1)
	      break;
        out = (out + 1) % BUFFER_SIZE;
        printf ("%d get--->\n", out);
        sem_post(empty);
    }
      return NULL;

}

int main (void){
    pthread_t th_prod, th_cons;
    void *retval;


    empty = (sem_t *) malloc(sizeof(sem_t));
    full = (sem_t *) malloc(sizeof(sem_t));

    sem_init(empty, 0, BUFFER_SIZE);
    sem_init(full, 0, 0);

    in = 0;
    out = 0;

    pthread_create(&th_prod, NULL, producer, 0);
    pthread_create(&th_cons, NULL, consumer, 0);


}