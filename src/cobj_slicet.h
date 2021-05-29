#if !defined(COBJ_SLICET_H)
#define COBJ_SLICET_H

typedef struct SliceT_ SliceT;
typedef struct SliceTIter_ SliceTIter;

typedef struct SliceTMut_ SliceTMut;
typedef struct SliceTMutIter_ SliceTMutIter;


#include "t.h"
#include "cobj_error.h"
#include "cobj_defs.h" // WARN_UNUSED_RESULT

#include <stdbool.h>
#include <stddef.h> // size_t

/**
 * Slice
 * A contiguous range of T.
 * Shared, Does not own what it points to, merely tracks the range
 * It's the Developers responsibility to ensure contents and array remain valid for it's lifetime.
 */

struct SliceT_ {
    T const *arr;
    T const *arr_end;
};

void SliceT_destroy(SliceT *const self);
void SliceT_move(SliceT *const self, SliceT *const src);
bool WARN_UNUSED_RESULT SliceT_try_copy(SliceT *self, SliceT const *const src, Error *err);
void SliceT_new(SliceT *const self, T const *arr, size_t len);

T const *SliceT_ptr(SliceT const *const self);
size_t SliceT_len(SliceT const *const self);
bool SliceT_is_empty(SliceT const *const self);

// null if not valid pos
T const *SliceT_get_item_at(SliceT const *const self, size_t pos);

bool SliceT_try_subslice(SliceT const *const self, size_t b, size_t e, SliceT *const dest, Error *err);

// cannot move const data src into const data dest
// bool WARN_UNUSED_RESULT SliceT_try_move_from(SliceT *const self, SliceT *const src, Error *err);
// cannot copy into a const array
//bool WARN_UNUSED_RESULT SliceT_try_copy_from(SliceT *const self, SliceT const *const src, Error *err);

void SliceT_iter(SliceT const *const self, SliceTIter *const it);


struct SliceTIter_ {
    T const *p;
    T const *e;
};

void SliceTIter_destroy(SliceTIter *const self);
void SliceTIter_move(SliceTIter *const self, SliceTIter *const src);
bool WARN_UNUSED_RESULT SliceTIter_try_copy(SliceTIter *const self, SliceTIter const *const src);
T const *SliceTIter_next(SliceTIter *const self);
void SliceTIter_new(SliceTIter *const self, T const *const b, T const *const e);




struct SliceTMut_ {
    T *arr;
    T *arr_end;
};

void SliceTMut_destroy(SliceTMut *const self);
void SliceTMut_move(SliceTMut *const self, SliceTMut *const src);
bool WARN_UNUSED_RESULT SliceTMut_try_copy(SliceTMut *self, SliceTMut const *const src, Error *err);
void SliceTMut_new(SliceTMut *const self, T *arr, size_t len);

size_t SliceTMut_len(SliceTMut const *const self);
bool SliceTMut_is_empty(SliceTMut const *const self);

// null if not valid pos
T *SliceTMut_get_item_at(SliceTMut const *const self, size_t pos);

bool SliceTMut_try_subslice(SliceTMut const *const self, size_t b, size_t e, SliceTMut *const dest, Error *err);

void SliceTMut_as_slice(SliceTMut const *const self, SliceT *const s);

bool WARN_UNUSED_RESULT SliceTMut_try_move_from(SliceTMut *const self, SliceTMut const *const src, Error *err);
bool WARN_UNUSED_RESULT SliceTMut_try_copy_from(SliceTMut *const self, SliceT const *const src, Error *err);

void SliceTMut_iter(SliceTMut const *const self, SliceTIter *const it);
void SliceTMut_iter_mut(SliceTMut const *const self, SliceTMutIter *const it);


struct SliceTMutIter_ {
    T *p;
    T *e;
};

void SliceTMutIter_destroy(SliceTMutIter *const self);
void SliceTMutIter_move(SliceTMutIter *const self, SliceTMutIter *const src);
bool WARN_UNUSED_RESULT SliceTMutIter_try_copy(SliceTMutIter *const arr, SliceTMutIter const *const src);
T *SliceTMutIter_next(SliceTMutIter *const self);
void SliceTMutIter_new(SliceTMutIter *const self, T * const b, T * const e);



#endif//!defined(COBJ_SLICET_H)
