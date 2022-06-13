#include "segel.h"
#include "request.h"
#include <string.h>
#include "Queue.h"




 typedef struct _threadEntry {
    pthread_t thread;
    int stat_data;
    void* request;
} ThreadEntry;

//
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too
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
pthread_cond_t condition;
pthread_mutex_t lock;
int requests_running;
Queue wait_queue;

void* doRoutine(void* request)
{
    while(1) {
        pthread_mutex_lock(&lock);
        while (queueIsEmpty(wait_queue))//change queue isn't empty
        {
            pthread_cond_wait(&condition, &lock);
        }
        QueueElement elem;
        queueRemove(wait_queue, &elem);
        //out of the queue begin work on the request. Bonjur ve la tanur
        requests_running++;
        requestHandle(elem.connfd);

        Close(elem.connfd);
        destroyQueueElement(&elem);
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char *argv[])
{
    enum schedAlg overload_alg;
    struct sockaddr_in clientaddr;
    int listenfd, connfd, port, threads, max_request_size, clientlen;
    requests_running = 0;

    getargs(&port, &threads, &max_request_size, &overload_alg, argc, argv);
    wait_queue = queueCreate(max_request_size - threads);
    if(pthread_cond_init(&condition, NULL)!= 0)
    {
        fprintf(stderr,"Fucky Wucky Cond");
        exit(1);
    }
    if(pthread_mutex_init(&lock, NULL) != 0)
    {
        fprintf(stderr,"Fucky Wucky Mutex");
        exit(1);
    }

    ThreadEntry* workers = calloc(threads, sizeof(ThreadEntry));
    for(int i=0; i < threads; i++)
    {
        pthread_t threadID;
        if(pthread_create(&threadID, NULL, &doRoutine,NULL) != 0)
        {
            fprintf(stderr,"Fucky Wucky");
            exit(1);
        }
        ThreadEntry entry;
        entry.thread = threadID;
        entry.stat_data = 0; //change this
        entry.request = NULL;
        workers[i] = entry;
    }




    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        if(max_request_size < requests_running + queueSize(wait_queue))
        {
            //something went terribly wrong if you entered this
            exit(1);
        }
    else if(max_request_size > requests_running + queueSize(wait_queue))//add queue size
    {
        connfd = Accept(listenfd, (SA *) &clientaddr, (socklen_t *) &clientlen);
        pthread_mutex_lock(&lock);
        QueueElement*  element =  createQueueElement(connfd);
        queueInsert(wait_queue,element);
    }
    else
    {
        pthread_mutex_lock(&lock);
        //overload according to the enum
    }
    pthread_mutex_unlock(&lock);

    }

}



    


 
