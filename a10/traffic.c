
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"
#include <limits.h> 

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

#define MAX_OCCUPANCY  3
#define NUM_ITERATIONS 100
#define NUM_CARS       20

// These times determine the number of times yield is called when in
// the street, or when waiting before crossing again.
#define CROSSING_TIME             NUM_CARS
#define WAIT_TIME_BETWEEN_CROSSES NUM_CARS

/**
 * You might find these declarations useful.
 */
enum Direction {EAST = 0, WEST = 1};
const static enum Direction oppositeEnd [] = {WEST, EAST};

// struct to represent the street being crossed by cars
struct Street {
  uthread_mutex_t mutex;
  uthread_cond_t eastBound, westBound;
  int eastCount, westCount; 
  int carsInStreet;
  enum Direction currentDirection;
  int fairnessThreshold;
} Street;

// init the street variables and synch tools
void initializeStreet(void) {
  Street.mutex = uthread_mutex_create();
  Street.eastBound = uthread_cond_create(Street.mutex);
  Street.westBound = uthread_cond_create(Street.mutex);
  Street.eastCount = Street.westCount = 0;
  Street.carsInStreet = 0;
  Street.currentDirection = EAST; 
  Street.fairnessThreshold = 10; 
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int             entryTicker;                                          
int             waitingHistogram [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogramLock;
int             occupancyHistogram [2] [MAX_OCCUPANCY + 1];

// record waiting times in histogram
void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogramLock);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogramLock);
}

// enter street ensuring conditions like direction and max occupancy
void enterStreet(enum Direction g) {
  uthread_mutex_lock(Street.mutex);
  int waitStart = entryTicker;
  
  // increment count based on direction
  if (g == EAST) Street.eastCount++;
  else Street.westCount++;
  
  // wait until cond are right (street capacity + directionality)
  while (Street.carsInStreet == MAX_OCCUPANCY || 
         (Street.carsInStreet > 0 && Street.currentDirection != g) || 
         (Street.carsInStreet == 0 && Street.eastCount > 0 && Street.westCount > 0 && Street.currentDirection == oppositeEnd[g])) {
    if ((g == EAST && Street.westCount > Street.eastCount + Street.fairnessThreshold) || 
        (g == WEST && Street.eastCount > Street.westCount + Street.fairnessThreshold)) {
      break;
    }
    
    if (g == EAST) uthread_cond_wait(Street.eastBound);
    else uthread_cond_wait(Street.westBound);
  }

  // assert conditions are met before entering the street
  assert(Street.carsInStreet < MAX_OCCUPANCY); 
  assert(Street.carsInStreet == 0 || Street.currentDirection == g); 

  Street.carsInStreet++;
  Street.currentDirection = g;
  entryTicker++;

  // decrease count of waiting cars
  if (g == EAST) Street.eastCount--;
  else Street.westCount--;
  
  // record occupancy in histogram
  occupancyHistogram[g][Street.carsInStreet]++;
  
  int waitEnd = entryTicker;
  recordWaitingTime(waitEnd - waitStart - 1); 
  
  uthread_mutex_unlock(Street.mutex);
}

// logic for a car to leave the street
void leaveStreet(void) {
  uthread_mutex_lock(Street.mutex);
  
  assert(Street.carsInStreet > 0);

  Street.carsInStreet--;
  // signal or broadcast depending on conditions
  if (Street.carsInStreet == 0) {
    if (Street.eastCount > Street.westCount || (Street.currentDirection == WEST && Street.eastCount > 0)) {
      uthread_cond_broadcast(Street.eastBound);
    } else if (Street.westCount > 0) {
      uthread_cond_broadcast(Street.westBound);
    }
  } else {
    if (Street.currentDirection == EAST) uthread_cond_signal(Street.eastBound);
    else uthread_cond_signal(Street.westBound);
  }
  
  uthread_mutex_unlock(Street.mutex);
}

// represent behaviour of car
void* car(void* arg) {
  enum Direction direction = rand() % 2; // randomly assigns direction
  for (int i = 0; i < NUM_ITERATIONS; i++) { // each car repeats cycle for set # of iterations
    enterStreet(direction); // attempts to enter street; wait if condition not met
    for (int j = 0; j < CROSSING_TIME; j++) uthread_yield(); // simulates time tkaen to cross street
    leaveStreet(); 
    for (int k = 0; k < WAIT_TIME_BETWEEN_CROSSES; k++) uthread_yield(); // wait before crossing again
  }
  return NULL;
}

int main (int argc, char** argv) {

  uthread_init(8);

  waitingHistogramLock = uthread_mutex_create();
  
  initializeStreet();
  uthread_t pt [NUM_CARS];

  for (int i = 0; i < NUM_CARS; i++) {
    pt[i] = uthread_create(car, NULL);
  }

  for (int i = 0; i < NUM_CARS; i++) {
    uthread_join(pt[i], NULL);
  }
  
  printf ("Times with 1 car  going east: %d\n", occupancyHistogram [EAST] [1]);
  printf ("Times with 2 cars going east: %d\n", occupancyHistogram [EAST] [2]);
  printf ("Times with 3 cars going east: %d\n", occupancyHistogram [EAST] [3]);
  printf ("Times with 1 car  going west: %d\n", occupancyHistogram [WEST] [1]);
  printf ("Times with 2 cars going west: %d\n", occupancyHistogram [WEST] [2]);
  printf ("Times with 3 cars going west: %d\n", occupancyHistogram [WEST] [3]);
  
  printf ("Waiting Histogram\n");
  for (int i=0; i < WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Cars waited for           %4d car%s to enter: %4d time(s)\n",
	      i, i==1 ? " " : "s", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Cars waited for more than %4d cars to enter: %4d time(s)\n",
	    WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}