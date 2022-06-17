#include "segel.h"
#include "request.h"
#include <string.h>
#include "Queue.h"
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include "macros2.h"






//
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//


void getargs(int *port,int* threads, int* queue_size,enum schedAlg* overload_alg, int argc, char *argv[])
{
    if (argc < 5) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *threads = atoi(argv[2]);
    *queue_size = atoi(argv[3]);
    if(strcmp(argv[4], "block") == 0)
    {
        *overload_alg = block_sched;
        return;
    }
    else if(strcmp(argv[4], "dt") == 0)
    {
        *overload_alg = dt_sched;
        return;
    }
    else if(strcmp(argv[4], "dh") == 0)
    {
        *overload_alg = dh_sched;
        return;
    }
    else if(strcmp(argv[4], "random") == 0)
    {
        *overload_alg = random_sched;
        return;
    }
    else
    {
        fprintf(stderr , "Poopoo");
        exit(1);
    }
}

pthread_cond_t condition, condition_master;
pthread_mutex_t lock;
volatile int requests_running;
volatile Queue wait_queue;


void* doRoutine(void* thread_index_ptr)
{    
    int thread_index = *((int *) thread_index_ptr);
    volatile ThreadEntry* thread_entry_ptr = workers+thread_index;
    assert(thread_index == thread_entry_ptr->thread_index);
    while(true)
    {
        pthread_mutex_lock(&lock);
        THREAD_LOCKED_INDEX(thread_index);
        while (queueIsEmpty(wait_queue))
        {
            PRINTF_STRING("Im gonna condition this thread up!")

            pthread_cond_wait(&condition, &lock);
        }
        PRINTF_STRING("yes papa im working\n");
        QueueElement elem;
        if(queueRemove(wait_queue, &elem) == QUEUE_SUCCESS) {
            requests_running++;
            //printf("yes papa number now is %d\n", requests_running);
        }
        else{
            fprintf(stderr,"oh no we couldn't remove from the queueue");
            exit(5);
        }

        ((ThreadEntry *) thread_entry_ptr)->request_arrival = *elem->request_arrival;
        struct timeval runtime;
        gettimeofday(&runtime,NULL);
        ((ThreadEntry *) thread_entry_ptr)->request_work_start = runtime;

        pthread_mutex_unlock(&lock);

        /* * work neto:*/
        requestHandle(elem->connfd, thread_entry_ptr);
        //for pointer timevals
        //((ThreadEntry *) thread_entry_ptr)->request_arrival = NULL;
        //((ThreadEntry *) thread_entry_ptr)->request_work_start = NULL;
        Close(elem->connfd);
        destroyQueueElement(&elem);

        pthread_mutex_lock(&lock);
        THREAD_LOCKED_INDEX(thread_index);
        requests_running--;
        pthread_cond_signal(&condition_master); //for master thread in block
        pthread_cond_signal(&condition); //for all of my friends
        pthread_mutex_unlock(&lock);
    }
}

void overloadQueue(enum schedAlg overload_alg, int last_conf, int max_request)
{
    QueueElement elem = createQueueElement(last_conf);
    switch(overload_alg) {

        case random_sched:{
            if(queueIsEmpty(wait_queue)){
                close(last_conf);
                return;
            }
            queueRemoveRandom(wait_queue);
            queueInsert(wait_queue, createQueueElement(last_conf));
            return;
        }

        case dh_sched: {
            switch (queueSize(wait_queue)) {
                case 0:
                    Close(last_conf);
                    return;
                default:{
                    QueueElement temp_elem;
                    queueRemove(wait_queue, &temp_elem);
                    Close(temp_elem->connfd);
                    destroyQueueElement(&temp_elem);
                    queueInsert(wait_queue, elem);
                    return;
                }
            }
        }

        case dt_sched: {
            Close(last_conf);
            return;
        }

        case block_sched:{
            pthread_mutex_unlock(&lock);
            while (max_request == queueSize(wait_queue) + requests_running)
            {
                pthread_cond_wait(&condition_master, &lock);
            }
            queueInsert(wait_queue, elem);
            return;
        }
        default:
			return;
        //block case is in the master thread code

    }
}

int main(int argc, char *argv[]) {
    // Initialization
    enum schedAlg overload_alg;
    struct sockaddr_in clientaddr;
    int listenfd, connfd, port, threads, max_request_size, clientlen;
    requests_running = 0;
    getargs(&port, &threads, &max_request_size, &overload_alg, argc, argv);
    wait_queue = queueCreate(max_request_size);
    if (pthread_cond_init(&condition, NULL) != 0) {
        fprintf(stderr, "Fucky Wucky Cond");
        exit(1);
    }
    if (pthread_cond_init(&condition_master, NULL) != 0) {
        fprintf(stderr, "Fucky Wucky Cond");
        exit(1);
    }
    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Fucky Wucky Mutex");
        exit(1);
    }

    // create all threads
    workers = calloc(threads, sizeof(ThreadEntry));
    for (int i = 0; i < threads; i++)
    {
        pthread_t threadID;

        workers[i].thread = threadID;
        workers[i].all_requests_handled = 0;
        workers[i].dynamic_requests_handled = 0;
        workers[i].static_requests_handled = 0;
        //workers[i].request_arrival;
        //workers[i].request_work_start;
        workers[i].thread_index = i;

        if (pthread_create(&threadID, NULL, &doRoutine, (void*) &(workers[i].thread_index)) != 0)
        {
            fprintf(stderr, "Fucky Wucky");
            exit(1);
        }
    }

    //main thread: listener
    listenfd = Open_listenfd(port);
    while (true) {
        //printf("Obey your master! there are %d process running \n", requests_running);
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *) &clientaddr, (socklen_t *) &clientlen);
        pthread_mutex_lock(&lock);
        MASTER_LOCKED;
        PRINTF_STRING("max_request_size:");
        PRINTF_INT(max_request_size);
        PRINTF_STRING("requests_running:");
        PRINTF_INT(requests_running);
        PRINTF_STRING("queueSize(wait_queue):");
        PRINTF_INT(queueSize(wait_queue));
        if (max_request_size < requests_running + queueSize(wait_queue))
        {
            fprintf(stderr, "surpassed the max size somehow");
            exit(1);
        }
        else if (max_request_size > requests_running + queueSize(wait_queue))
        {
            QueueElement element = createQueueElement(connfd);
            queueInsert(wait_queue, element);
        }
        else if(max_request_size == requests_running + queueSize(wait_queue))
        {
            overloadQueue(overload_alg, connfd,max_request_size);
        }
        //is sent to a sleeping worker
        pthread_cond_signal(&condition);
        pthread_mutex_unlock(&lock);
    }
}
