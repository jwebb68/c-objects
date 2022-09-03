#if !defined(COBJ_SLICEVT_H)
#    define COBJ_SLICEVT_H

/**
 * Slice<T>, Slice<TMut>: A non-owned region of initialised Ts.
 *
 * Shared, Does not own what it points to, merely tracks the range
 * It's the Developers responsibility to ensure contents and array remain valid for it's lifetime.
 */

typedef struct SliceVT_ SliceVT;
typedef struct SliceVTIter_ SliceVTIter;

typedef struct SliceVTMut_ SliceVTMut;
typedef struct SliceVTMutIter_ SliceVTMutIter;

#    include <cobj/based/cobj_slicev.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct SliceVT_ {
    SliceV inner;
};

void SliceVT_destroy_member(SliceVT *const self);
void SliceVT_destroy(SliceVT *const self);

void SliceVT_move_member(SliceVT *const self, SliceVT *const src);
void SliceVT_move(SliceVT *const self, SliceVT *const src);

bool WARN_UNUSED_RESULT SliceVT_try_copy(SliceVT *const self,
                                         SliceVT const *const src,
                                         Error *const err);

// void SliceVT_new(SliceVT *const self, T const *const b, T const *const e);
void SliceVT_new(SliceVT *const self, T const *const ptr, size_t len);

T const *SliceVT_ptr(SliceVT const *const self);
size_t SliceVT_len(SliceVT const *const self);
bool SliceVT_is_empty(SliceVT const *const self);

// null if not valid pos
T const *SliceVT_try_get(SliceVT const *const self, size_t pos);

bool SliceVT_try_subslice(SliceVT const *const self,
                          size_t b,
                          size_t e,
                          SliceVT *const dest,
                          Error *const err);

void SliceVT_iter(SliceVT const *const self, SliceVTIter *const it);
SliceVTIter SliceVT_iter2(SliceVT const *const self);

struct SliceVTIter_ {
    SliceVIter inner;
};

void SliceVTIter_destroy_member(SliceVTIter *const self);
void SliceVTIter_destroy(SliceVTIter *const self);

void SliceVTIter_move_member(SliceVTIter *const self, SliceVTIter *const src);
void SliceVTIter_move(SliceVTIter *const self, SliceVTIter *const src);

bool WARN_UNUSED_RESULT SliceVTIter_try_copy(SliceVTIter *const self,
                                             SliceVTIter const *const src,
                                             Error *const err);
T const *SliceVTIter_next(SliceVTIter *const self);
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len
void SliceVTIter_new(SliceVTIter *const self, T const *const b, T const *const e);

struct SliceVTMut_ {
    SliceVMut inner;
};

void SliceVTMut_destroy_member(SliceVTMut *const self);
void SliceVTMut_destroy(SliceVTMut *const self);

void SliceVTMut_move_member(SliceVTMut *const self, SliceVTMut *const src);
void SliceVTMut_move(SliceVTMut *const self, SliceVTMut *const src);

bool WARN_UNUSED_RESULT SliceVTMut_try_copy(SliceVTMut *const self,
                                            SliceVTMut const *const src,
                                            Error *const err);

void SliceVTMut_new(SliceVTMut *const self, T *const ptr, size_t len);

T *SliceVTMut_ptr(SliceVTMut const *const self);
size_t SliceVTMut_len(SliceVTMut const *const self);
bool SliceVTMut_is_empty(SliceVTMut const *const self);

// null if not valid pos
T *SliceVTMut_try_get(SliceVTMut const *const self, size_t pos);

bool SliceVTMut_try_subslice(SliceVTMut const *const self,
                             size_t b,
                             size_t e,
                             SliceVT *const dest,
                             Error *const err);
bool SliceVTMut_try_subslice_mut(SliceVTMut const *const self,
                                 size_t b,
                                 size_t e,
                                 SliceVTMut *const dest,
                                 Error *const err);

void SliceVTMut_as_slice(SliceVTMut const *const self, SliceVT *const s);

bool WARN_UNUSED_RESULT SliceVTMut_try_move_from(SliceVTMut const *const self,
                                                 SliceVTMut const *const src,
                                                 Error *const err);
bool WARN_UNUSED_RESULT SliceVTMut_try_copy_from(SliceVTMut const *const self,
                                                 SliceVT const *const src,
                                                 Error *const err);

void SliceVTMut_iter(SliceVTMut const *const self, SliceVTIter *const it);
SliceVTIter SliceVTMut_iter2(SliceVTMut const *const self);
void SliceVTMut_iter_mut(SliceVTMut const *const self, SliceVTMutIter *const it);
SliceVTMutIter SliceVTMut_iter_mut2(SliceVTMut const *const self);

struct SliceVTMutIter_ {
    SliceVMutIter inner;
};

void SliceVTMutIter_destroy_member(SliceVTMutIter *const self);
void SliceVTMutIter_destroy(SliceVTMutIter *const self);

void SliceVTMutIter_move_member(SliceVTMutIter *const self, SliceVTMutIter *const src);
void SliceVTMutIter_move(SliceVTMutIter *const self, SliceVTMutIter *const src);

bool WARN_UNUSED_RESULT SliceVTMutIter_try_copy(SliceVTMutIter *const arr,
                                                SliceVTMutIter const *const src,
                                                Error *const err);
T *SliceVTMutIter_next(SliceVTMutIter *const self);

void SliceVTMutIter_new(SliceVTMutIter *const self, T *const b, T *const e);

#endif //! defined(COBJ_SLICEVT_H)
