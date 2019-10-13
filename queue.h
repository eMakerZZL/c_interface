#ifndef QUEUE_H
#define QUEUE_H 

#define CREATE_QUEUE( QUEUE_TYPE, SUFFIX, QUEUE_SIZE)        \
                                                             \
static QUEUE_TYPE queue##SUFFIX[QUEUE_SIZE + 1];             \
static int front##SUFFIX = 0;                                \
static int rear##SUFFIX = 0;                                 \
                                                             \
int queue_is_empty##SUFFIX(void) {                           \
    return front##SUFFIX == rear##SUFFIX;                    \
}                                                            \
                                                             \
int queue_is_full##SUFFIX(void) {                            \
    return (rear##SUFFIX + 1) % QUEUE_SIZE == front##SUFFIX; \
}                                                            \
                                                             \
void enqueue##SUFFIX(QUEUE_TYPE val) {                       \
    assert(!queue_is_full##SUFFIX());                        \
                                                             \
    rear##SUFFIX = (rear##SUFFIX + 1) % QUEUE_SIZE;          \
    queue##SUFFIX[rear##SUFFIX] = val;                       \
                                                             \
    return;                                                  \
}                                                            \
                                                             \
void dequeue##SUFFIX(void) {                                 \
    assert(!queue_is_empty##SUFFIX());                       \
                                                             \
    front##SUFFIX = (front##SUFFIX + 1) % QUEUE_SIZE;        \
                                                             \
    return;                                                  \
}                                                            \
                                                             \
QUEUE_TYPE first##SUFFIX(void) {                             \
    assert(!queue_is_empty##SUFFIX());                       \
                                                             \
    return queue##SUFFIX[front##SUFFIX];                     \
}

#endif /* end of include guard: QUEUE_H */
