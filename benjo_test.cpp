//
// Created by alonb on 13/06/2022.
//
#include <queue>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "Queue.h"
#include <assert.h>

typedef enum {
    INSERT,
    REMOVE,
    SIZE,
    IS_EMPTY,
    IS_FULL,
    FUNC_ENUM_SIZE
} Function;

#ifdef __cplusplus
}
#endif

#define MAX_SIZE 10
#define ITERATIONS 100
#define MODULO 100

static int queue_size;

using namespace std;


extern "C" {
    void do_insert(queue<QueueElement> std_q, Queue my_q);
    void do_remove(queue<QueueElement> std_q, Queue my_q);
    void do_size(queue<QueueElement> std_q, Queue my_q);
    void do_is_empty(queue<QueueElement> std_q, Queue my_q);
    void do_is_full(queue<QueueElement> std_q, Queue my_q);
}


int main(int argc, char *argv[]) {
    queue <QueueElement> std_q;
        queue_size = 0;
        Queue my_q = createQueueElement(MAX_SIZE);
            for ( int i = 0; i < ITERATIONS ; ++ i ) {
            Function func = rand() % FUNC_ENUM_SIZE;
            switch(func){
                case INSERT:
                    do_insert(std_q, my_q);
                    break;
                case REMOVE:
                    do_remove(std_q, my_q);
                    break;
                case SIZE:
                    do_size(std_q, my_q);
                    break;
                case IS_EMPTY:
                    do_is_empty(std_q, my_q);
                    break;
                case IS_FULL:
                    do_is_full(std_q, my_q);
                    break;
                default:
                    break;
            }
        }
    }
    /*switch ( func ) {
    case INSERT : {
    if ( queue_size > MAX_SIZE )
    break ;
    int rnd = rand() % MODULO;
    QueueElement element = createQueueElement(rnd);
    assert (queueInsert(my_q, element) == QUEUE_SUCCESS ) ;
    extern "C++" {
    try {
    std_q . push(element);
    }
    catch ( ... ) {
    extern "C" { assert ( 0 ) ; }
    }*/


extern "C" {
    void do_insert(queue<QueueElement> std_q, Queue my_q) {
        if (queue_size == MAX_SIZE)
            return;
        int rnd = rand() % MODULO;
        QueueElement element = createQueueElement(rnd);
        assert(queueInsert(my_q, element) == QUEUE_SUCCESS);
        std_q.push(element);
    }

void do_remove(queue <QueueElement> std_q, Queue my_q) {
    if (queue_size == 0)
        return;
    QueueElement my_res;
    QueueElement std_res;
    assert(queueRemove(my_q, my_res) == QUEUE_SUCCESS);
    std_res = std_q.front();
    std_q.pop();
    assert(my_res->connfd == std_res->cnnfd);
    queue_size--;
}

    void do_size(queue<QueueElement> std_q, Queue my_q){
        int my_size = queueSize(my_q);
        asert(my_size >= 0);
        int std_size = std_q.size();
        assert(my_size == queue_size);
        assert(std_size == queue_size);
    }

    void do_is_empty(queue<QueueElement> std_q, Queue my_q){
        bool my_empty = queueIsEmpty(my_q);
        bool std_empty = std_q.size()==0;
        assert(my_empty == std_empty);
    }

    void do_is_full(queue<QueueElement> std_q, Queue my_q){
        bool my_full = queueIsFull(my_q);
        bool std_full = std_q.size()==MAX_SIZE;
        assert(my_full == std_full);
    }

}
