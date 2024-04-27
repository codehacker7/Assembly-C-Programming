#include <stdlib.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"
#include "threadpool.h"

// struct for single task in threadpool 
typedef struct task {
    void (*function)(tpool_t, void*);  // func ptr to task function
    void *arg;                         // arg to be passed to task function 
    struct task *next;                 // ptr to next task in queue
} task_t;


struct tpool {
    uthread_mutex_t mutex;
    uthread_cond_t task_cond;
    uthread_cond_t done_cond;
    unsigned int thread_count;
    unsigned int active_threads;
    task_t *task_queue_head;
    task_t *task_queue_tail;
    uthread_t *threads; // array to keep track of worker thread IDs, just in case!
    int stop;
};

/**
 * Base procedure of every worker thread.  Calls available tasks
 * or blocks until a task becomes available.
 */
void *worker_thread(void *pool_v) {
    tpool_t pool = (tpool_t)pool_v;
    while (1) {
        uthread_mutex_lock(pool->mutex);

        // wait for task to be available or for stop single
        while (!pool->stop && pool->task_queue_head == NULL) {
            uthread_cond_wait(pool->task_cond);
        }

        // break out of loop if threadpool is stopping and no tasks are left
        if (pool->stop && pool->task_queue_head == NULL) {
            uthread_mutex_unlock(pool->mutex);
            break;
        }

        // take the next task from queue
        task_t *task = pool->task_queue_head;
        pool->task_queue_head = task->next;
        if (pool->task_queue_head == NULL) {
            pool->task_queue_tail = NULL;
        }

        pool->active_threads++;
        uthread_mutex_unlock(pool->mutex);

        // exectute task
        task->function(pool, task->arg);

        free(task);

        // update pool state after completing task
        uthread_mutex_lock(pool->mutex);
        pool->active_threads--;
        if (!pool->stop && pool->active_threads == 0 && pool->task_queue_head == NULL) {
            uthread_cond_signal(pool->done_cond);
        }
        uthread_mutex_unlock(pool->mutex);
    }
    return NULL;
}


/**
 * Create a new thread pool with max_threads thread-count limit.
 */
tpool_t tpool_create(unsigned int max_threads) {
    tpool_t pool = malloc(sizeof(struct tpool));
    if (!pool) return NULL; // error handling just in case :)!

    // intialize synch primitives
    pool->mutex = uthread_mutex_create();
    pool->task_cond = uthread_cond_create(pool->mutex);
    pool->done_cond = uthread_cond_create(pool->mutex);

    // error handling just in case :)!
    if (!pool->mutex || !pool->task_cond || !pool->done_cond) {
        free(pool);
        return NULL;
    }

    // init threadpool structure
    pool->thread_count = max_threads;
    pool->active_threads = 0;
    pool->task_queue_head = NULL;
    pool->task_queue_tail = NULL;
    pool->stop = 0;

    // allocating memory for worker threads
    pool->threads = malloc(sizeof(uthread_t) * max_threads);
    // error handling just in case :)!
    if (!pool->threads) {
        free(pool);
        return NULL;
    }

    // create worker threads
    for (unsigned int i = 0; i < pool->thread_count; i++) {
        pool->threads[i] = uthread_create(worker_thread, pool);
    }

    return pool;
}

/**
 * Sechedule task f(arg) to be executed.
 */
void tpool_schedule_task(tpool_t pool, void (*f)(tpool_t, void *), void *arg) {
    task_t *new_task = malloc(sizeof(task_t));
    new_task->function = f;
    new_task->arg = arg;
    new_task->next = NULL;

    uthread_mutex_lock(pool->mutex);

    // dont schedule new tasks if pool is stopping
    if (pool->stop) {
        free(new_task);
        uthread_mutex_unlock(pool->mutex);
        return;
    }

    // add new task to task queue
    if (pool->task_queue_head == NULL) {
        pool->task_queue_head = new_task;
        pool->task_queue_tail = new_task;
    } else {
        pool->task_queue_tail->next = new_task;
        pool->task_queue_tail = new_task;
    }

    uthread_cond_signal(pool->task_cond); // signal worker thread that new task is available
    uthread_mutex_unlock(pool->mutex);
}

/**
 * Wait (by blocking) until all tasks have completed and thread pool is thus idle
 */
void tpool_join(tpool_t pool) {
    uthread_mutex_lock(pool->mutex);
    pool->stop = 1; // signal that no more tasks should be scheduled
    uthread_cond_broadcast(pool->task_cond); // wake up worker threads

    uthread_mutex_unlock(pool->mutex);

    // wait for worker threads to finish
    for (unsigned int i = 0; i < pool->thread_count; i++) {
        uthread_join(pool->threads[i], NULL);
    }


    // cleanup threadpool 
    uthread_mutex_destroy(pool->mutex);
    uthread_cond_destroy(pool->task_cond);
    uthread_cond_destroy(pool->done_cond);
    free(pool->threads);
    free(pool);
}

