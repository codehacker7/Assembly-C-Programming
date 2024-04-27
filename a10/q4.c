
#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

uthread_t t0, t1, t2;

uthread_mutex_t mutex;
uthread_cond_t cond_t1, cond_t2;

int t1_executed = 0;
int t2_executed = 0;


void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void* p0(void* v) {
  randomStall();
  uthread_mutex_lock(mutex);
  printf("zero\n");
  t1_executed = 1;
  uthread_cond_signal(cond_t2);
  uthread_mutex_unlock(mutex);
  return NULL;
}

void* p1(void* v) {
  randomStall();
  uthread_mutex_lock(mutex);
  while (!t1_executed) {
    uthread_cond_wait(cond_t1);
  }
  printf("one\n");
  t2_executed = 1;

  uthread_cond_signal(cond_t2);
  uthread_mutex_unlock(mutex);
  return NULL;
}

void* p2(void* v) {
  randomStall();
  uthread_mutex_lock(mutex);
   while (!t2_executed) {
    uthread_cond_wait(cond_t2);
  }
  printf("two\n");
  uthread_mutex_unlock(mutex);
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  mutex = uthread_mutex_create();
  cond_t1 = uthread_cond_create(mutex);
  cond_t2 = uthread_cond_create(mutex);
  t0 = uthread_create(p0, NULL);
  t1 = uthread_create(p1, NULL);
  t2 = uthread_create(p2, NULL);
  randomStall();
  uthread_join (t0, NULL);
  uthread_join (t1, NULL);
  uthread_join (t2, NULL);
  printf("three\n");
  printf("------\n");
  uthread_mutex_destroy(mutex);
  uthread_cond_destroy(cond_t1);
  uthread_cond_destroy(cond_t2);
}