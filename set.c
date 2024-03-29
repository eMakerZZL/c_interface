#include "set.h"
#include "arith.h"
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#define T Set_T

struct T {
    int length;
    unsigned int timestamp;
    int (*cmp)(const void* x, const void* y);
    unsigned long (*hash)(const void* x);
    int size;
    struct member {
        struct member* link;
        const void* member;
    } * *buckets;
};

static int cmpatom(const void* x, const void* y)
{
    return x != y;
}

static unsigned long hashatom(const void* x)
{
    return (unsigned long)x >> 2;
}

T Set_new(int hint, int cmp(const void* x, const void* y), unsigned long hash(const void* x))
{
    T set;
    int i;
    static int primes[] = { 509, 509, 1021, 2053, 4093, 8191, 16381, 32771, 65521, INT_MAX };

    assert(hint >= 0);
    for (i = 1; primes[i] < hint; i++) {
    }
    set = (T)malloc(sizeof(*set) + primes[i - 1] * sizeof(set->buckets[0]));
    set->size = primes[i - 1];
    set->cmp = cmp ? cmp : cmpatom;
    set->hash = hash ? hash : hashatom;
    set->buckets = (struct member**)(set + 1);
    for (i = 0; i < set->size; i++)
        set->buckets[i] = NULL;
    set->length = 0;
    set->timestamp = 0;

    return set;
}

void Set_free(T* set)
{
    assert(set && *set);

    if ((*set)->length > 0) {
        int i;

        struct member *p, *q;
        for (i = 0; i < (*set)->size; i++) {
            for (p = (*set)->buckets[i]; p; p = q) {
                q = p->link;
                free(p);
            }
        }
    }

    free(*set);
}

int Set_length(T set)
{
    assert(set);

    return set->length;
}

int Set_member(T set, const void* member)
{
    int i;
    struct member* p;

    assert(set);
    assert(member);

    i = (*set->hash)(member) % set->size;
    for (p = set->buckets[i]; p; p = p->link)
        if ((*set->cmp)(member, p->member) == 0)
            break;

    return p != NULL;
}

void Set_put(T set, const void* member)
{
    int i;
    struct member* p;

    assert(set);
    assert(member);

    i = (*set->hash)(member) % set->size;
    for (p = set->buckets[i]; p; p = p->link)
        if ((*set->cmp)(member, p->member) == 0)
            break;

    if (p == NULL) {
        p = malloc(sizeof(*p));
		p->member = member;
        p->link = set->buckets[i];
        set->buckets[i] = p;
        set->length += 1;
    } else
        p->member = member;

    set->timestamp += 1;
}

void* Set_remove(T set, const void* member)
{
    int i;
    struct member** pp;

    assert(set);
    assert(member);

    set->timestamp += 1;
    i = (*set->hash)(member) % set->size;
    for (pp = &set->buckets[i]; *pp; pp = &(*pp)->link)
        if ((*set->cmp)(member, (*pp)->member) == 0) {
            struct member* p = *pp;
            *pp = p->link;
            member = p->member;
            free(p);
            set->length -= 1;
            return (void*)member;
        }
    return NULL;
}

void Set_map(T set, void apply(const void* member, void* cl), void* cl)
{
    int i;
    unsigned int stamp;
    struct member* p;

    assert(set);
    assert(apply);

    stamp = set->timestamp;
    for (i = 0; i < set->size; i++) {
        for (p = set->buckets[i]; p; p = p->link) {
            apply(p->member, cl);
            assert(set->timestamp == stamp);
        }
    }
}

void** Set_toArray(T set, void* end)
{
    int i, j = 0;
    void** array;
    struct member* p;

    assert(set);
    array = (void*)malloc((set->length + 1) * sizeof(*array));
    for (i = 0; i < set->size; i++)
        for (p = set->buckets[i]; p; p = p->link)
            array[j++] = (void*)p->member;
    array[j] = end;

    return array;
}

static T copy(T t, int hint)
{
    T set;

    assert(t);
    set = Set_new(hint, t->cmp, t->hash);
    int i;
    struct member* q;
    for (i = 0; i < t->size; i++) {
        for (q = t->buckets[i]; q; q = q->link) {
            struct member* p;
            const void* member = q->member;
            int i = (*set->hash)(member) % set->size;
            p = (struct member*)malloc(sizeof(*p));
            p->member = member;
            p->link = set->buckets[i];
            set->buckets[i] = p;
            set->length += 1;
        }
    }
    return set;
}

T Set_union(T s, T t)
{
    if (s == NULL) {
        assert(t);
        return copy(t, t->size);
    } else if (t == NULL) {
        assert(s);
        return copy(s, s->size);
    } else {
        T set = copy(s, Arith_max(s->size, t->size));
        assert(s->cmp == t->cmp && s->hash == t->hash);
        int i;
        struct member* q;
        for (i = 0; i < t->size; i++) {
            for (q = t->buckets[i]; q; q = q->link)
                Set_put(set, q->member);
        }
        return set;
    }
}

T Set_inter(T s, T t)
{
    if (s == NULL) {
        assert(t);
        return Set_new(t->size, t->cmp, t->hash);
    } else if (t == NULL) {
        assert(s);
        return Set_new(s->size, s->cmp, s->hash);
    } else if (s->length > t->length) {
        Set_inter(t, s);
    } else {
        T set = Set_new(Arith_min(s->size, t->size), s->cmp, s->hash);
        int i;
        struct member* q;
        for (i = 0; i < t->size; i++) {
            for (q = t->buckets[i]; q; q = q->link) {
                if (Set_member(s, q->member)) {
                    struct member* p;
                    const void* member = q->member;
                    int i = (*set->hash)(member) % set->size;
                    p = (struct member*)malloc(sizeof(*p));
                    p->member = member;
                    p->link = set->buckets[i];
                    set->buckets[i] = p;
                    set->length += 1;
                }
            }
        }
        return set;
    }
}

T Set_minus(T s, T t)
{
    if (t == NULL) {
        assert(s);
        return Set_new(s->size, s->cmp, s->hash);
    } else if (s == NULL) {
        return copy(t, t->size);
    } else {
        T set = Set_new(Arith_min(s->size, t->size), s->cmp, s->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);
        int i;
        struct member* q;
        for (i = 0; i < t->size; i++)
            for (q = t->buckets[i]; q; q = q->link) {
                if (!Set_member(s, q->member)) {
                    struct member* p;
                    const void* member = q->member;
                    int i = (*set->hash)(member) % set->size;
                    p = (struct member*)malloc(sizeof(*p));
                    p->member = member;
                    p->link = set->buckets[i];
                    set->buckets[i] = p;
                    set->length += 1;
                }
            }
        return set;
    }
}

T Set_diff(T s, T t)
{
    if (s == NULL) {
        assert(t);
        return copy(t, t->size);
    } else if (t == NULL) {
        return copy(s, s->size);
    } else {
        T set = Set_new(Arith_min(s->size, t->size), s->cmp, s->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);
        int i;
        struct member* q;
        for (i = 0; i < t->size; i++)
            for (q = t->buckets[i]; q; q = q->link) {
                if (!Set_member(s, q->member)) {
                    struct member* p;
                    const void* member = q->member;
                    int i = (*set->hash)(member) % set->size;
                    p = (struct member*)malloc(sizeof(*p));
                    p->member = member;
                    p->link = set->buckets[i];
                    set->buckets[i] = p;
                    set->length += 1;
                }
            }
        {
            T u = t;
            t = s;
            s = u;
        }
        for (i = 0; i < t->size; i++)
            for (q = t->buckets[i]; q; q = q->link) {
                if (!Set_member(s, q->member)) {
                    struct member* p;
                    const void* member = q->member;
                    int i = (*set->hash)(member) % set->size;
                    p = (struct member*)malloc(sizeof(*p));
                    p->member = member;
                    p->link = set->buckets[i];
                    set->buckets[i] = p;
                    set->length += 1;
                }
            }
        return set;
    }
}
