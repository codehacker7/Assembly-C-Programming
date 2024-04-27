void interrupt_service_routine () {
    void* tid;
    queue_dequeue   (pending_read_queue, &tid, NULL, NULL);
    uthread_unblock (tid);
}

void* read_block (void* blocknov) {
    int* blockno = blocknov;
    int result;

    disk_schedule_read (&result, *blockno);
    queue_enqueue      (pending_read_queue, uthread_self(), NULL, NULL);
    uthread_block      ();
    sum = sum + result;

    return NULL;
}

int main (int argc, char** argv) {
    ...
    // Initialize
    uthread_init (1);
    disk_start (interrupt_service_routine);
    pending_read_queue = queue_create();

    // Sum Blocks
    uthread_t threads  [num_blocks];
    int       blocknos [num_blocks];
    for (int blockno = 0; blockno < num_blocks; blockno++) {
        blocknos [blockno] = blockno;
        threads [blockno] = uthread_create (read_block, & blocknos [blockno]);
    }
    for (int blockno = 0; blockno < num_blocks; blockno++)
        uthread_join (threads [blockno], NULL);
    printf ("%d\n", sum);
}  