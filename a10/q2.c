uthread_mutex_t mx; 

void countUp() {
  uthread_mutex_lock (mx);
    for (int i=0; i<N; i++)
      cnt += 1;
  uthread_mutex_unlock (mx);
}

void countDown() {
  uthread_mutex_lock (mx);
    for (int i=0; i<N; i++)
      cnt -= 1;
  uthread_mutex_unlock (mx);
}

int main(int arg, char** arv) {
  uthread_init(4);
  mx = uthread_mutex_create();
  ...
}