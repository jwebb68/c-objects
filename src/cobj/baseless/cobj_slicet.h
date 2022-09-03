#if !defined(COBJ_SLICET_H)
#    define COBJ_SLICET_H

/**
 * Slice<T>, Slice<TMut>: A non-owned region of initialised Ts.
 *
 * Shared, Does not own what it points to, merely tracks the range
 * It's the Developers responsibility to ensure contents and array remain valid for it's lifetime.
 */

typedef struct SliceT_ SliceT;
typedef struct SliceTIter_ SliceTIter;

typedef struct SliceTMut_ SliceTMut;
typedef struct SliceTMutIter_ SliceTMutIter;

#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct SliceT_ {
    T const *ptr;
    size_t len;
};

void SliceT_destroy_member(SliceT *const self);
void SliceT_destroy(SliceT *const self);

void SliceT_move_member(SliceT *const self, SliceT *const src);
void SliceT_move(SliceT *const self, SliceT *const src);

bool WARN_UNUSED_RESULT SliceT_try_copy(SliceT *self, SliceT const *const src, Error *err);

// void SliceT_new(SliceT *const self, T const *const b, T const *const e);
void SliceT_new(SliceT *const self, T const *const ptr, size_t len);

T const *SliceT_ptr(SliceT const *const self);
size_t SliceT_len(SliceT const *const self);
bool SliceT_is_empty(SliceT const *const self);

// null if not valid pos
T const *SliceT_try_get(SliceT const *const self, size_t pos);

bool SliceT_try_subslice(SliceT const *const self,
                         size_t b,
                         size_t e,
                         SliceT *const dest,
                         Error *const err);

void SliceT_iter(SliceT const *const self, SliceTIter *const it);
SliceTIter SliceT_iter2(SliceT const *const self);

struct SliceTIter_ {
    T const *p;
    T const *e;
};

void SliceTIter_destroy_member(SliceTIter *const self);
void SliceTIter_destroy(SliceTIter *const self);

void SliceTIter_move_membere(SliceTIter *const self, SliceTIter *const src);
void SliceTIter_move(SliceTIter *const self, SliceTIter *const src);

bool WARN_UNUSED_RESULT SliceTIter_try_copy(SliceTIter *const self, SliceTIter const *const src);
T const *SliceTIter_next(SliceTIter *const self);
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len
void SliceTIter_new(SliceTIter *const self, T const *const b, T const *const e);

struct SliceTMut_ {
    T *ptr;
    size_t len;
};

void SliceTMut_destroy_member(SliceTMut *const self);
void SliceTMut_destroy(SliceTMut *const self);

void SliceTMut_move_member(SliceTMut *const self, SliceTMut *const src);
void SliceTMut_move(SliceTMut *const self, SliceTMut *const src);

bool WARN_UNUSED_RESULT SliceTMut_try_copy(SliceTMut *self, SliceTMut const *const src, Error *err);

void SliceTMut_new(SliceTMut *const self, T *const ptr, size_t len);

T *SliceTMut_ptr(SliceTMut const *const self);
size_t SliceTMut_len(SliceTMut const *const self);
bool SliceTMut_is_empty(SliceTMut const *const self);

// null if not valid pos
T *SliceTMut_try_get(SliceTMut const *const self, size_t pos);

bool SliceTMut_try_subslice(SliceTMut const *const self,
                            size_t b,
                            size_t e,
                            SliceT *const dest,
                            Error *const err);
bool SliceTMut_try_subslice_mut(SliceTMut const *const self,
                                size_t b,
                                size_t e,
                                SliceTMut *const dest,
                                Error *const err);

void SliceTMut_as_slice(SliceTMut const *const self, SliceT *const s);

bool WARN_UNUSED_RESULT SliceTMut_try_move_from(SliceTMut *const self,
                                                SliceTMut const *const src,
                                                Error *const err);
bool WARN_UNUSED_RESULT SliceTMut_try_copy_from(SliceTMut *const self,
                                                SliceT const *const src,
                                                Error *const err);

void SliceTMut_iter(SliceTMut const *const self, SliceTIter *const it);
SliceTIter SliceTMut_iter2(SliceTMut const *const self);
void SliceTMut_iter_mut(SliceTMut const *const self, SliceTMutIter *const it);
SliceTMutIter SliceTMut_iter_mut2(SliceTMut const *const self);

struct SliceTMutIter_ {
    T *p;
    T *e;
};

void SliceTMutIter_destroy_member(SliceTMutIter *const self);
void SliceTMutIter_destroy(SliceTMutIter *const self);

void SliceTMutIter_move_member(SliceTMutIter *const self, SliceTMutIter *const src);
void SliceTMutIter_move(SliceTMutIter *const self, SliceTMutIter *const src);

bool WARN_UNUSED_RESULT SliceTMutIter_try_copy(SliceTMutIter *const arr,
                                               SliceTMutIter const *const src);
T *SliceTMutIter_next(SliceTMutIter *const self);

void SliceTMutIter_new(SliceTMutIter *const self, T *const b, T *const e);

#endif //! defined(COBJ_SLICET_H)
