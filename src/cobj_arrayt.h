#if !defined(COBJ_ARRAYT_H)
#define COBJ_ARRAYT_H

// no structs as intended to work with normal carrays
typedef struct ArrayTIter_ ArrayTIter;
typedef struct ArrayTIterMut_ ArrayTIterMut;

#include "t.h"
#include "cobj_slicet.h"
#include "cobj_defs.h" // WARN_UNUSED_RESULT
#include "cobj_error.h"

#include <stddef.h>  // size_t
#include <stdbool.h>


void ArrayT_destroy_p(T *const arr, T *const end);
void ArrayT_move_p(T *const arr, T *const src, T *const src_end);
bool WARN_UNUSED_RESULT ArrayT_try_copy_p(T *const arr, T const *const src, T const *const src_end, Error *err);

void ArrayT_new_default_p(T *const arr, T *const end);

void ArrayT_destroy_i(T *const arr, size_t len);
void ArrayT_move_i(T *const arr, T *const src, size_t len);
bool WARN_UNUSED_RESULT ArrayT_try_copy_i(T *const arr, T const *const src, size_t len, Error *err);

void ArrayT_new_default_i(T *const arr, size_t len);

void ArrayT_as_slice(T const *const arr, size_t len, SliceT *const s);
void ArrayT_as_slice_mut(T *const arr, size_t len, SliceTMut *const s);

void ArrayT_iter(T const *const arr, size_t len, SliceTIter *const it);
void ArrayT_iter_mut(T *const arr, size_t len, SliceTMutIter *const it);

// void ArrayT_iter(T const *const arr, size_t len, ArrayTIter *const it);
// void ArrayT_iter_mut(T *const arr, size_t len, ArrayTIterMut *const it);

// struct ArrayTIter_ {
//     T const *arr;
//     T const *end;
//     T const *pos;
// };

// void ArrayTIter_destroy(ArrayTIter *const self);
// void ArrayTIter_move(ArrayTIter *const self, ArrayTIter *const src);
// bool WARN_UNUSED_RESULT ArrayTIter_try_copy(T *const arr, T const *const src);
// T const *ArrayTIter_next(ArrayTIter *const self);
// void ArrayTIter_new(ArrayTIter *const self, T const *const arr, size_t len);


// struct ArrayTIterMut_ {
//     T *arr;
//     T *end;
//     T *pos;
// };

// void ArrayTIterMut_destroy(ArrayTIterMut *const self);
// void ArrayTIterMut_move(ArrayTIterMut *const self, ArrayTIterMut *const src);
// bool WARN_UNUSED_RESULT ArrayTIterMut_try_copy(T *const arr, T const *const src);
// T *ArrayTIterMut_next(ArrayTIterMut *const self);
// void ArrayTIterMut_new(ArrayTIterMut *const self, T *const arr, size_t len);

#endif//!defined(COBJ_ARRAYT_H)
