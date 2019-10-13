#ifndef STACK_H
#define STACK_H

#include <assert.h>

#define CREATE_STACK(STACK_TYPE, SUFFIX, STACK_SIZE) \
                                                     \
static STACK_TYPE stack##SUFFIX[STACK_SIZE];         \
static int top_element##SUFFIX = -1;                 \
                                                     \
int is_empty##SUFFIX(void) {                         \
    return top_element##SUFFIX == -1;                \
}                                                    \
                                                     \
int is_full##SUFFIX(void) {                          \
    return top_element##SUFFIX == STACK_SIZE - 1;    \
}                                                    \
                                                     \
void push##SUFFIX(STACK_TYPE val) {                  \
  assert(!is_full##SUFFIX());                        \
  stack##SUFFIX[++top_element##SUFFIX] = val;        \
}                                                    \
                                                     \
void pop##SUFFIX(void) {                             \
  assert(!is_empty##SUFFIX());                       \
  top_element##SUFFIX -= 1;                          \
}                                                    \
                                                     \
STACK_TYPE top##SUFFIX(void) {                       \
  assert(!is_empty##SUFFIX());                       \
  return stack##SUFFIX[top_element##SUFFIX];         \
}

#endif /* end of include guard: STACK_H */
