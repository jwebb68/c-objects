#if !defined(COBJ_T_H)
#define COBJ_T_H

typedef struct T_ T;

#include "cobj_defs.h"  // WARN_RESULT
#include "cobj_error.h"

#include <stdbool.h>
#include <stddef.h> //size_t

struct T_ {
    int dummy;
};

void T_destroy(T *const self);
void T_move(T *const self, T *const src);
bool WARN_RESULT T_try_copy(T *const self, T const *const src, Error *const err);
bool T_is_eq(T const *const self, T const *const b);
bool T_is_lt(T const *const self, T const *const b);
bool T_is_gt(T const *const self, T const *const b);
void T_new_int(T *const self, int v);
void T_new_default(T *const self);

T *T_malloc(void);
// no T_free(T *) as that's just free()
// T_malloc_new ?, no, use BoxT
// T_free_destroy()? no use BoxT

// array of T manip functions
void ArrayT_destroy(T *const arr, size_t len);
void ArrayT_move(T *const arr, T *const src, size_t len);
bool WARN_RESULT ArrayT_try_copy(T *const arr, T const *const src, size_t len, Error *const err);
void ArrayT_new_default(T *const arr, size_t len);
// Slice ?
// Iter/IterMut ?


#endif//defined(COBJ_T_H)
