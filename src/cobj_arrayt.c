#include "cobj_arrayt.h"

#include "cobj_memory.h" //memwipe

#include <string.h> // memmove

void ArrayT_destroy_p(T *const arr, T *const end)
{
    for (T *it = arr; it != end; ++it) {
        T_destroy(it);
    }
}

void ArrayT_destroy_i(T *const arr, size_t len)
{
    ArrayT_destroy_p(arr, arr + len);
}

void ArrayT_move(T *const arr, T *const src, size_t len)
{
    memmove(arr, src, len * sizeof(T));
    memwipe(src, len * sizeof(T));
}

bool WARN_UNUSED_RESULT ArrayT_try_copy(T *const arr, T const *const src, size_t len, Error *err)
{
    bool ok = true;
    T *d_it = arr;
    for (T const *s_it = src; s_it != (src + len); ++d_it, ++s_it) {
        ok = T_try_copy(d_it, s_it, err);
        if (!ok) {
            break;
        }
    }
    if (!ok) {
        ArrayT_destroy_p(arr, d_it);
    }
    return ok;
}

void ArrayT_new_default(T *const arr, size_t len)
{
    for (T *it = arr; it != (arr + len); ++it) {
        T_new_default(it);
    }
}

void ArrayT_as_slice(T const *const arr, size_t len, SliceT *const s)
{
    SliceT_new(s, arr, len);
}

void ArrayT_as_slice_mut(T *const arr, size_t len, SliceTMut *const s)
{
    SliceTMut_new(s, arr, len);
}

void ArrayT_iter(T const *const arr, size_t len, SliceTIter *const it)
{
    SliceTIter_new(it, arr, arr + len);
}

void ArrayT_iter_mut(T *const arr, size_t len, SliceTMutIter *const it)
{
    SliceTMutIter_new(it, arr, arr + len);
}

// void ArrayTIter_destroy(ArrayTIter *const self) {
//     STRUCTWIPE(self);
// }

// void ArrayTIter_move(ArrayTIter *const self, ArrayTIter *const src) {
//     *self = *src;
//     STRUCTWIPE(src);
// }

// bool WARN_UNUSED_RESULT ArrayTIter_try_copy(T *const self, T const *const src) {
//     *self = *src;
//     return true;
// }

// T const *ArrayTIter_next(ArrayTIter *const self) {
//     T const * p = self->pos + 1;
//     if (p >= self->end) { return NULL; }
//     self->pos = p;
//     return p;
// }

// void ArrayTIter_new(ArrayTIter *const self, T const *const arr, size_t len) {
//     self->arr = arr;
//     self->end = arr + len;
//     self->pos = arr;
// }

// void ArrayTIterMut_destroy(ArrayTIterMut *const self) {
//     STRUCTWIPE(self);
// }

// void ArrayTIterMut_move(ArrayTIterMut *const self, ArrayTIterMut *const src) {
//     *self = *src;
//     STRUCTWIPE(src);
// }

// bool WARN_UNUSED_RESULT ArrayTIterMut_try_copy(T *const self, T const *const src) {
//     *self = *src;
//     return true;
// }

// T *ArrayTIterMut_next(ArrayTIterMut *const self) {
//     T * p = self->pos + 1;
//     if (p >= self->end) { return NULL; }
//     self->pos = p;
//     return p;
// }

// void ArrayTIterMut_new(ArrayTIterMut *const self, T *const arr, size_t len) {
//     self->arr = arr;
//     self->end = arr + len;
//     self->pos = arr;
// }
