#include "t.h"

#include "cobj_defs.h" // UNUSED_ARG
#include "cobj_memory.h" // STRUCTWIPE

#include <stdlib.h> // malloc/free

static void T_wipe(T *const self)
{
    STRUCTWIPE(self);
}

void T_destroy(T *const self)
{
    T_wipe(self);
}

void T_move(T *const self, T *const src)
{
    *self = *src;
    T_wipe(src);
}

bool WARN_UNUSED_RESULT T_try_copy(T *const self, T const *const src, Error *const err)
{
    *self = *src;
    // and/or copy any members in a by-member fashion,
    // mainly those that have special copy semantics and a simple bit-copy won't do.
    UNUSED_ARG(err);
    return false;
    // or
    // return ERROR_RAISE(err, Error_ENOTIMPL);
}

bool T_is_eq(T const *const self, T const *const b)
{
    return self->dummy == b->dummy;
}

bool T_is_ne(T const *const self, T const *const b)
{
    return self->dummy != b->dummy;
}

bool T_is_lt(T const *const self, T const *const b)
{
    return self->dummy < b->dummy;
}

bool T_is_gt(T const *const self, T const *const b)
{
    return self->dummy > b->dummy;
}

void T_new_int(T *const self, int v)
{
    self->dummy = v;
}

void T_new_default(T *const self)
{
    self->dummy = 0;
}

// T *T_malloc(void) {
//     return (T *)malloc(sizeof(T));
// }

// void ArrayT_destroy_p(T *const arr, T *const arr_e) {
//     for (T *it = arr; it != arr_e; ++it) {
//         T_destroy(it);
//     }
// }

// void ArrayT_destroy_i(T *const arr, size_t len) {
//     ArrayT_destroy_p(arr, arr+len);
// }

// void ArrayT_new_default_p(T *const arr, T *const arr_e) {
//     for (T *it = arr; it != arr_e; ++it) {
//         T_new_default(it);
//     }
// }

// void ArrayT_new_default_i(T *const arr, size_t len) {
//     ArrayT_new_default_p(arr, arr+len);
// }
