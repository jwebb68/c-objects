#if !defined(COBJ_SLICEVT_H)
#    define COBJ_SLICEVT_H

/**
 * Slice<T>, Slice<TMut>: A non-owned region of initialised Ts.
 *
 * Shared, Does not own what it points to, merely tracks the range
 * It's the Developers responsibility to ensure contents and array remain valid for it's lifetime.
 */

typedef struct cobj_SliceVT_s cobj_SliceVT;
typedef struct cobj_SliceVTIter_s cobj_SliceVTIter;

typedef struct cobj_SliceVTMut_s cobj_SliceVTMut;
typedef struct cobj_SliceVTMutIter_s cobj_SliceVTMutIter;

#    include <cobj/based/cobj_slicev.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct cobj_SliceVT_s {
    cobj_SliceV inner;
};

void cobj_SliceVT_destroy_member(cobj_SliceVT *const self);
void cobj_SliceVT_destroy(cobj_SliceVT *const self);

void cobj_SliceVT_move_member(cobj_SliceVT *const self, cobj_SliceVT *const src);
void cobj_SliceVT_move(cobj_SliceVT *const self, cobj_SliceVT *const src);

bool WARN_UNUSED_RESULT cobj_SliceVT_try_copy(cobj_SliceVT *const self,
                                              cobj_SliceVT const *const src,
                                              cobj_Error *const err);

// void cobj_SliceVT_new(cobj_SliceVT *const self, T const *const b, T const *const e);
void cobj_SliceVT_new(cobj_SliceVT *const self, T const *const ptr, size_t len);

T const *cobj_SliceVT_ptr(cobj_SliceVT const *const self);
size_t cobj_SliceVT_len(cobj_SliceVT const *const self);
bool cobj_SliceVT_is_empty(cobj_SliceVT const *const self);

// null if not valid pos
T const *cobj_SliceVT_try_get(cobj_SliceVT const *const self, size_t pos);

bool cobj_SliceVT_try_subslice(cobj_SliceVT const *const self,
                               size_t b,
                               size_t e,
                               cobj_SliceVT *const dest,
                               cobj_Error *const err);

void cobj_SliceVT_iter(cobj_SliceVT const *const self, cobj_SliceVTIter *const it);
cobj_SliceVTIter cobj_SliceVT_iter2(cobj_SliceVT const *const self);

struct cobj_SliceVTIter_s {
    cobj_SliceVIter inner;
};

void cobj_SliceVTIter_destroy_member(cobj_SliceVTIter *const self);
void cobj_SliceVTIter_destroy(cobj_SliceVTIter *const self);

void cobj_SliceVTIter_move_member(cobj_SliceVTIter *const self, cobj_SliceVTIter *const src);
void cobj_SliceVTIter_move(cobj_SliceVTIter *const self, cobj_SliceVTIter *const src);

bool WARN_UNUSED_RESULT cobj_SliceVTIter_try_copy(cobj_SliceVTIter *const self,
                                                  cobj_SliceVTIter const *const src,
                                                  cobj_Error *const err);
T const *cobj_SliceVTIter_next(cobj_SliceVTIter *const self);
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len
void cobj_SliceVTIter_new(cobj_SliceVTIter *const self, T const *const b, T const *const e);

struct cobj_SliceVTMut_s {
    cobj_SliceVMut inner;
};

void cobj_SliceVTMut_destroy_member(cobj_SliceVTMut *const self);
void cobj_SliceVTMut_destroy(cobj_SliceVTMut *const self);

void cobj_SliceVTMut_move_member(cobj_SliceVTMut *const self, cobj_SliceVTMut *const src);
void cobj_SliceVTMut_move(cobj_SliceVTMut *const self, cobj_SliceVTMut *const src);

bool WARN_UNUSED_RESULT cobj_SliceVTMut_try_copy(cobj_SliceVTMut *const self,
                                                 cobj_SliceVTMut const *const src,
                                                 cobj_Error *const err);

void cobj_SliceVTMut_new(cobj_SliceVTMut *const self, T *const ptr, size_t len);

T *cobj_SliceVTMut_ptr(cobj_SliceVTMut const *const self);
size_t cobj_SliceVTMut_len(cobj_SliceVTMut const *const self);
bool cobj_SliceVTMut_is_empty(cobj_SliceVTMut const *const self);

// null if not valid pos
T *cobj_SliceVTMut_try_get(cobj_SliceVTMut const *const self, size_t pos);

bool cobj_SliceVTMut_try_subslice(cobj_SliceVTMut const *const self,
                                  size_t b,
                                  size_t e,
                                  cobj_SliceVT *const dest,
                                  cobj_Error *const err);
bool cobj_SliceVTMut_try_subslice_mut(cobj_SliceVTMut const *const self,
                                      size_t b,
                                      size_t e,
                                      cobj_SliceVTMut *const dest,
                                      cobj_Error *const err);

void cobj_SliceVTMut_as_slice(cobj_SliceVTMut const *const self, cobj_SliceVT *const s);

bool WARN_UNUSED_RESULT cobj_SliceVTMut_try_move_from(cobj_SliceVTMut const *const self,
                                                      cobj_SliceVTMut const *const src,
                                                      cobj_Error *const err);
bool WARN_UNUSED_RESULT cobj_SliceVTMut_try_copy_from(cobj_SliceVTMut const *const self,
                                                      cobj_SliceVT const *const src,
                                                      cobj_Error *const err);

void cobj_SliceVTMut_iter(cobj_SliceVTMut const *const self, cobj_SliceVTIter *const it);
cobj_SliceVTIter cobj_SliceVTMut_iter2(cobj_SliceVTMut const *const self);
void cobj_SliceVTMut_iter_mut(cobj_SliceVTMut const *const self, cobj_SliceVTMutIter *const it);
cobj_SliceVTMutIter cobj_SliceVTMut_iter_mut2(cobj_SliceVTMut const *const self);

struct cobj_SliceVTMutIter_s {
    cobj_SliceVMutIter inner;
};

void cobj_SliceVTMutIter_destroy_member(cobj_SliceVTMutIter *const self);
void cobj_SliceVTMutIter_destroy(cobj_SliceVTMutIter *const self);

void cobj_SliceVTMutIter_move_member(cobj_SliceVTMutIter *const self,
                                     cobj_SliceVTMutIter *const src);
void cobj_SliceVTMutIter_move(cobj_SliceVTMutIter *const self, cobj_SliceVTMutIter *const src);

bool WARN_UNUSED_RESULT cobj_SliceVTMutIter_try_copy(cobj_SliceVTMutIter *const arr,
                                                     cobj_SliceVTMutIter const *const src,
                                                     cobj_Error *const err);
T *cobj_SliceVTMutIter_next(cobj_SliceVTMutIter *const self);

void cobj_SliceVTMutIter_new(cobj_SliceVTMutIter *const self, T *const b, T *const e);

#endif //! defined(COBJ_SLICEVT_H)
