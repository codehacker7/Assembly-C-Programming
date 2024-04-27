#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

#include "stdbool.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create(agent->mutex);
  agent->match   = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke   = uthread_cond_create(agent->mutex);
  return agent;
}

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

// # of threads waiting for a signal. Used to ensure that the agent
// only signals once all other threads are ready.
int num_active_threads = 0;

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

int availableMaterials;
uthread_cond_t tobaccoMatch, paperTobacco, matchPaper;

// wakes appropriate smoker based on combo of materials
void wakeupSmoker(int mats) {
  switch (mats) {
    case TOBACCO + MATCH:
      uthread_cond_signal(tobaccoMatch);
      break;
    case PAPER + TOBACCO:
      uthread_cond_signal(paperTobacco);
      break;    
    case PAPER + MATCH:
      uthread_cond_signal(matchPaper);
      break;
    default:
      return;
  }
  availableMaterials = 0; // reset availableMaterials to 0
}


// == HANDLERS ==


// handler for when [tobacco] is available
void* handlerTobacco(void* agent) {
  struct Agent* a = (struct Agent*) agent;
  uthread_mutex_lock(a->mutex);

  while(true) {
    // wait for signal that [tobacco] is available
    uthread_cond_wait(a->tobacco);
    // add [tobacco] to available materials
    availableMaterials = availableMaterials + TOBACCO;
    // attempt to wake up appropriate smoker
    wakeupSmoker(availableMaterials);
  }

  uthread_mutex_unlock(a->mutex);
}


void* handlerPaper(void* agent) {
  struct Agent* a = (struct Agent*) agent;
  uthread_mutex_lock(a->mutex);

  while(true) {
    uthread_cond_wait(a->paper);
    availableMaterials = availableMaterials + PAPER;
    wakeupSmoker(availableMaterials);
  }

  uthread_mutex_unlock(a->mutex);
}

void* handlerMatches(void* agent) {
  struct Agent* a = (struct Agent*) agent;
  uthread_mutex_lock(a->mutex);

  while(true) {
    uthread_cond_wait(a->match);
    availableMaterials = availableMaterials + MATCH;
    wakeupSmoker(availableMaterials);
  }

  uthread_mutex_unlock(a->mutex);
}


// == SMOKERS ==


// smoker who has [tobacco]
void* smokerTobacco(void* agent) {
  struct Agent* a = (struct Agent*) agent;
  uthread_mutex_lock(a->mutex);

  while(true) {
    // wait for the condition to smoke
    uthread_cond_wait(matchPaper);
    // signal that smoking is occurring
    uthread_cond_signal(a->smoke);
    // increment the count for [tobacco]
    smoke_count[TOBACCO]++;
  }

  uthread_mutex_unlock(a->mutex);
}


void* smokerPaper(void* agent) {
  struct Agent* a = (struct Agent*) agent;
  uthread_mutex_lock(a->mutex);

  while(true) {
    uthread_cond_wait(tobaccoMatch);
    uthread_cond_signal(a->smoke);
    smoke_count[PAPER]++;
  }

  uthread_mutex_unlock(a->mutex);
}

void* smokerMatches(void* agent) {
  struct Agent* a = (struct Agent*) agent;
  uthread_mutex_lock(a->mutex);

  while(true) {
    uthread_cond_wait(paperTobacco);
    uthread_cond_signal(a->smoke);
    smoke_count[MATCH]++;
  }

  uthread_mutex_unlock(a->mutex);
}


/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can modify it if you like, but be sure that all it does
 * is choose 2 random resources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  srandom(time(NULL));
  
  uthread_mutex_lock (a->mutex);
  // Wait until all other threads are waiting for a signal
  while (num_active_threads < 3)
    uthread_cond_wait (a->smoke);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 6;
    switch(r) {
    case 0:
      signal_count[TOBACCO]++;
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    case 1:
      signal_count[PAPER]++;
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 2:
      signal_count[MATCH]++;
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 3:
      signal_count[TOBACCO]++;
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 4:
      signal_count[PAPER]++;
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 5:
      signal_count[MATCH]++;
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    }
    VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
    uthread_cond_wait (a->smoke);
  }
  
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main(int argc, char** argv) {
  struct Agent* a = createAgent();
  uthread_t agent_thread;

  uthread_init(5);

  // create condition variables for each combination of resources
  matchPaper = uthread_cond_create(a->mutex);
  paperTobacco = uthread_cond_create(a->mutex);
  tobaccoMatch = uthread_cond_create(a->mutex);

  // create smoker threads
  uthread_create(smokerTobacco, a);
  uthread_create(smokerMatches, a);
  uthread_create(smokerPaper, a);

  // create handler threads
  uthread_create(handlerTobacco, a);
  uthread_create(handlerMatches, a);
  uthread_create(handlerPaper, a);
  
  num_active_threads = num_active_threads + 3;

  agent_thread = uthread_create(agent, a);
  uthread_join(agent_thread, NULL);

  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);

  printf("Match (Signal): %d\n", signal_count[MATCH]);
  printf("Match (Smoke): %d\n", smoke_count[MATCH]);
  printf("Paper (Signal): %d\n", signal_count[PAPER]);
  printf("Paper (Smoke): %d\n", smoke_count[PAPER]);
  printf("Tobacco (Signal): %d\n", signal_count[TOBACCO]);
  printf("Tobacco (Smoke): %d\n", smoke_count[TOBACCO]);


 return 0;
}