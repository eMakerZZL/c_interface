#ifndef ARRAYREP_H
#define ARRAYREP_H

#define T Array_T

struct T {
  int length;
  int size;
  char *array;
};

extern void ArrayRep_init(T array, int length, int size, void *ary);

#undef T
#endif /* end of include guard: ARRAYREP_H */
