
void handle_read (void* resultv, void* notused) {
    int* result = resultv;
    sum = sum + *result;
}

void handle_read_and_exit (void* resultv, void* notused) {
    handle_read(resultv, NULL);
    printf ("%d\n", sum);
    exit (EXIT_SUCCESS);
}

int main(argc, char **argv) {
    ...
    int result[num_blocks];
    for (int blockno = 0; blockno < num_blocks; blockno++) {
        queue_enqueue (pending_read_queue, &result[blockno], NULL, blockno < num_blocks - 1? handle_read: handle_read_and_exit);
        disk_schedule_read (&result[blockno], blockno);
    ...
}