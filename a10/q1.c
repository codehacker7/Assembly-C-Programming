void* p1(void* v) {
  randomStall();
  uthread_join (t0, NULL); 
  printf("one\n");
  return NULL;
}

void* p2(void* v) {
  randomStall();
  uthread_join (t1, NULL); 
  printf("two\n");
  return NULL;}

int main(int arg, char** arv) {
  ...
  uthread_join (t2, NULL); 
  printf("three\n");
  printf("------\n");
}