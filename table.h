#ifndef TABLE_H
#define TABLE_H

#define T Table_T

typedef struct T *T;

extern T Table_new(int hint, int cmp(const void *x, const void *y),
                   unsigned int hash(const void *key));
extern void Table_free(T *table);
extern int Table_length(T table);
extern void *Table_put(T table, const void *key, const void *value);
extern void *Table_get(T table, const void *key);
extern void *Table_remove(T table, const void *key);
extern void Table_map(T table,
                      void apply(const void *key, void **value, void *cl),
                      void *cl);
extern void **Table_toArray(T table, void *end);

#undef T
#endif /* end of include guard: TABLE_H */
