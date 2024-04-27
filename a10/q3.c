uthread_mutex_t mx;
uthread_cond_t  go;
int             arrivals;

void waitForAllOtherThreads() {
  uthread_mutex_lock (mx);
    arrivals += 1;
    if (arrivals < NUM_THREADS)
      uthread_cond_wait (go);
    else 
      uthread_cond_broadcast (go);
  uthread_mutex_unlock (mx);
}

int main(int arg, char** arv) {
  uthread_init(4);
  mx = uthread_mutex_create();
  go = uthread_cond_create (mx);
  ...
}