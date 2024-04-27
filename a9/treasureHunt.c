void interrupt_service_routine() {
    void *val;
    void *arg;
    void (*callback)(void*, void*);
    queue_dequeue(pending_read_queue, &val, &arg, &callback);
    callback(val, arg);
}

void doNextRead(void *resultv, void *countv) {
    int *resultp = resultv;
    int *countp = countv;
    if(*countp == 0) {
        done = 1;
    } else {
        queue_enqueue(pending_read_queue, resultp, countp, handleOtherReads);
        disk_schedule_read(resultp, *resultp);
    }
}

void handleOtherReads (void *resultv, void *countv) {
    int *countp = countv;
    (*countp)--;
    doNextRead(resultv, countv);
}

void handleFirstRead (void *resultv, void *countv) {
    int *resultp = resultv;
    int *countp = countv;
    /* set count for future reads */
    *countp = *resultp;
    /* pass pointers onwards (all pointers refer to main's result & count vars */
    doNextRead(resultv, countv);
}

int main (int argc, char** argv) {
    ...
    int result, count;
    queue_enqueue(pending_read_queue, &result, &count, handleFirstRead);
    disk_schedule_read(&result, starting_block_number);
    ...
}