#if !defined(COBJ_SLICET_H)
#    define COBJ_SLICET_H

/**
 * Slice<T>, Slice<TMut>: A non-owned region of initialised Ts.
 *
 * Shared, Does not own what it points to, merely tracks the range
 * It's the Developers responsibility to ensure contents and array remain valid for it's lifetime.
 */

typedef struct cobj_SliceT_s cobj_SliceT;
typedef struct cobj_SliceTIter_s cobj_SliceTIter;

typedef struct cobj_SliceTMut_s cobj_SliceTMut;
typedef struct cobj_SliceTMutIter_s cobj_SliceTMutIter;

#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct cobj_SliceT_s {
    T const *ptr;
    size_t len;
};

void cobj_SliceT_destroy_member(cobj_SliceT *const self);
void cobj_SliceT_destroy(cobj_SliceT *const self);

void cobj_SliceT_move_member(cobj_SliceT *const self, cobj_SliceT *const src);
void cobj_SliceT_move(cobj_SliceT *const self, cobj_SliceT *const src);

bool WARN_UNUSED_RESULT cobj_SliceT_try_copy(cobj_SliceT *self,
                                             cobj_SliceT const *const src,
                                             cobj_Error *err);

// void cobj_SliceT_new(cobj_SliceT *const self, T const *const b, T const *const e);
void cobj_SliceT_new(cobj_SliceT *const self, T const *const ptr, size_t len);

T const *cobj_SliceT_ptr(cobj_SliceT const *const self);
size_t cobj_SliceT_len(cobj_SliceT const *const self);
bool cobj_SliceT_is_empty(cobj_SliceT const *const self);

// null if not valid pos
T const *cobj_SliceT_try_get(cobj_SliceT const *const self, size_t pos);

bool cobj_SliceT_try_subslice(cobj_SliceT const *const self,
                              size_t b,
                              size_t e,
                              cobj_SliceT *const dest,
                              cobj_Error *const err);

void cobj_SliceT_iter(cobj_SliceT const *const self, cobj_SliceTIter *const it);
cobj_SliceTIter cobj_SliceT_iter2(cobj_SliceT const *const self);

struct cobj_SliceTIter_s {
    T const *p;
    T const *e;
};

void cobj_SliceTIter_destroy_member(cobj_SliceTIter *const self);
void cobj_SliceTIter_destroy(cobj_SliceTIter *const self);

void cobj_SliceTIter_move_membere(cobj_SliceTIter *const self, cobj_SliceTIter *const src);
void cobj_SliceTIter_move(cobj_SliceTIter *const self, cobj_SliceTIter *const src);

bool WARN_UNUSED_RESULT cobj_SliceTIter_try_copy(cobj_SliceTIter *const self,
                                                 cobj_SliceTIter const *const src);
T const *cobj_SliceTIter_next(cobj_SliceTIter *const self);
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len
void cobj_SliceTIter_new(cobj_SliceTIter *const self, T const *const b, T const *const e);

struct cobj_SliceTMut_s {
    T *ptr;
    size_t len;
};

void cobj_SliceTMut_destroy_member(cobj_SliceTMut *const self);
void cobj_SliceTMut_destroy(cobj_SliceTMut *const self);

void cobj_SliceTMut_move_member(cobj_SliceTMut *const self, cobj_SliceTMut *const src);
void cobj_SliceTMut_move(cobj_SliceTMut *const self, cobj_SliceTMut *const src);

bool WARN_UNUSED_RESULT cobj_SliceTMut_try_copy(cobj_SliceTMut *self,
                                                cobj_SliceTMut const *const src,
                                                cobj_Error *err);

void cobj_SliceTMut_new(cobj_SliceTMut *const self, T *const ptr, size_t len);

T *cobj_SliceTMut_ptr(cobj_SliceTMut const *const self);
size_t cobj_SliceTMut_len(cobj_SliceTMut const *const self);
bool cobj_SliceTMut_is_empty(cobj_SliceTMut const *const self);

// null if not valid pos
T *cobj_SliceTMut_try_get(cobj_SliceTMut const *const self, size_t pos);

bool cobj_SliceTMut_try_subslice(cobj_SliceTMut const *const self,
                                 size_t b,
                                 size_t e,
                                 cobj_SliceT *const dest,
                                 cobj_Error *const err);
bool cobj_SliceTMut_try_subslice_mut(cobj_SliceTMut const *const self,
                                     size_t b,
                                     size_t e,
                                     cobj_SliceTMut *const dest,
                                     cobj_Error *const err);

void cobj_SliceTMut_as_slice(cobj_SliceTMut const *const self, cobj_SliceT *const s);

bool WARN_UNUSED_RESULT cobj_SliceTMut_try_move_from(cobj_SliceTMut *const self,
                                                     cobj_SliceTMut const *const src,
                                                     cobj_Error *const err);
bool WARN_UNUSED_RESULT cobj_SliceTMut_try_copy_from(cobj_SliceTMut *const self,
                                                     cobj_SliceT const *const src,
                                                     cobj_Error *const err);

void cobj_SliceTMut_iter(cobj_SliceTMut const *const self, cobj_SliceTIter *const it);
cobj_SliceTIter cobj_SliceTMut_iter2(cobj_SliceTMut const *const self);
void cobj_SliceTMut_iter_mut(cobj_SliceTMut const *const self, cobj_SliceTMutIter *const it);
cobj_SliceTMutIter cobj_SliceTMut_iter_mut2(cobj_SliceTMut const *const self);

struct cobj_SliceTMutIter_s {
    T *p;
    T *e;
};

void cobj_SliceTMutIter_destroy_member(cobj_SliceTMutIter *const self);
void cobj_SliceTMutIter_destroy(cobj_SliceTMutIter *const self);

void cobj_SliceTMutIter_move_member(cobj_SliceTMutIter *const self, cobj_SliceTMutIter *const src);
void cobj_SliceTMutIter_move(cobj_SliceTMutIter *const self, cobj_SliceTMutIter *const src);

bool WARN_UNUSED_RESULT cobj_SliceTMutIter_try_copy(cobj_SliceTMutIter *const arr,
                                                    cobj_SliceTMutIter const *const src);
T *cobj_SliceTMutIter_next(cobj_SliceTMutIter *const self);

void cobj_SliceTMutIter_new(cobj_SliceTMutIter *const self, T *const b, T *const e);

#endif //! defined(COBJ_SLICET_H)
