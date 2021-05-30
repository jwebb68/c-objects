#if !defined(COBJ_SLICEVT_H)
#    define COBJ_SLICEVT_H

typedef struct SliceVT_ SliceVT;
typedef struct SliceVTIter_ SliceVTIter;

typedef struct SliceVTMut_ SliceVTMut;
typedef struct SliceVTMutIter_ SliceVTMutIter;

#    include "cobj_defs.h" // WARN_UNUSED_RESULT
#    include "cobj_error.h"
#    include "cobj_slicev.h"
#    include "t.h"

#    include <stdbool.h>
#    include <stddef.h> // size_t

/**
 * Slice
 * A contiguous range of T.
 * Shared, Does not own what it points to, merely tracks the range
 * It's the Developers responsibility to ensure contents and array remain valid for it's lifetime.
 */

struct SliceVT_ {
    SliceV inner;
};

void SliceVT_destroy(SliceVT *const self);
void SliceVT_move(SliceVT *const self, SliceVT *const src);
bool WARN_UNUSED_RESULT SliceVT_try_copy(SliceVT *self, SliceVT const *const src, Error *err);
void SliceVT_new(SliceVT *const self, T const *const b, T const *const e);

size_t SliceVT_len(SliceVT const *const self);
bool SliceVT_is_empty(SliceVT const *const self);

// null if not valid pos
T const *SliceVT_get_item_at(SliceVT const *const self, size_t pos);

bool SliceVT_try_subslice(SliceVT const *const self,
                          size_t b,
                          size_t e,
                          SliceVT *const dest,
                          Error *err);

void SliceVT_iter(SliceVT const *const self, SliceVTIter *const it);

#    define m_SliceVT_destroy(T, self) SliceV_destroy((self))
#    define m_SliceVT_move(T, self, src) SliceV_move((self), (src))
#    define m_SliceVT_try_copy(T, self, src, err) SliceV_try_copy((self), (src), (err))
#    define m_SliceVT_new(T, self, b, e) SliceV_new((self), (b), (e))
#    define m_SliceVT_len(T, self) SliceV_len((self), sizeof(T))
#    define m_SliceVT_is_empty(T, self) SliceV_is_empty((self))
#    define m_SliceVT_get_item_at(T, self, pos) SliceV_get_item_at((self), (pos), sizeof(T))
#    define m_SliceVT_try_subslice(T, self, b, e, dest, err) \
        SliceV_try_subslice((self), (b), (e), (dest), (err), sizeof(T))
#    define m_SliceVT_iter(T, self, it) SliceV_iter((self), (it))

struct SliceVTIter_ {
    SliceVIter inner;
};

void SliceVTIter_destroy(SliceVTIter *const self);
void SliceVTIter_move(SliceVTIter *const self, SliceVTIter *const src);
bool WARN_UNUSED_RESULT SliceVTIter_try_copy(SliceVTIter *const self, SliceVTIter const *const src);
T const *SliceVTIter_next(SliceVTIter *const self);
void SliceVTIter_new(SliceVTIter *const self, T const *const b, T const *const e);

#    define m_SliceVIter_destroy(T, self) SliceVIter_destroy((self))
#    define m_SliceVIter_move(T, self, src) SliceVIter_move((self), (src))
#    define m_SliceVIter_try_copy(T, self, src, err) SliceVIter_try_copy((self), (src), (err))
#    define m_SliceVIter_next(T, self) SliceVIter_next((self), sizeof(T))
#    define m_SliceVIter_new(T, self, b, e) SliceVIter_new((self), (b), (e))

struct SliceVTMut_ {
    SliceVMut inner;
};

void SliceVTMut_destroy(SliceVTMut *const self);
void SliceVTMut_move(SliceVTMut *const self, SliceVTMut *const src);
bool WARN_UNUSED_RESULT SliceVTMut_try_copy(SliceVTMut *self,
                                            SliceVTMut const *const src,
                                            Error *err);
void SliceVTMut_new(SliceVTMut *const self, T *const b, T *const e);

size_t SliceVTMut_len(SliceVTMut const *const self);
bool SliceVTMut_is_empty(SliceVTMut const *const self);

// null if not valid pos
T *SliceVTMut_get_item_at(SliceVTMut const *const self, size_t pos);

bool SliceVTMut_try_subslice(SliceVTMut const *const self,
                             size_t b,
                             size_t e,
                             SliceVT *const dest,
                             Error *err);
bool SliceVTMut_try_subslice_mut(SliceVTMut const *const self,
                                 size_t b,
                                 size_t e,
                                 SliceVTMut *const dest,
                                 Error *err);

void SliceVTMut_as_slice(SliceVTMut const *const self, SliceVT *const s);

bool WARN_UNUSED_RESULT SliceVTMut_try_move_from(SliceVTMut *const self,
                                                 SliceVTMut const *const src,
                                                 Error *err);
bool WARN_UNUSED_RESULT SliceVTMut_try_copy_from(SliceVTMut *const self,
                                                 SliceVT const *const src,
                                                 Error *err);

void SliceVTMut_iter(SliceVTMut const *const self, SliceVTIter *const it);
void SliceVTMut_iter_mut(SliceVTMut const *const self, SliceVTMutIter *const it);

#    define m_SliceVTMut_destroy(T, self) SliceVMut_destroy((self))
#    define m_SliceVTMut_move(T, self, src) SliceVMut_move((self), (src))
#    define m_SliceVTMut_try_copy(T, self, src, err) SliceVMut_try_copy((self), (src), (err))
#    define m_SliceVTMut_new(T, self, b, e) SliceVMut_new((self), (b), (e))
#    define m_SliceVTMut_len(T, self) SliceVMut_len((self), sizeof(T))
#    define m_SliceVTMut_is_empty(T, self) SliceVMut_is_empty((self))
#    define m_SliceVTMut_get_item_at(T, self, pos) SliceVMut_get_item_at((self), (pos), sizeof(T))
#    define m_SliceVTMut_try_subslice(T, self, b, e, dest, err) \
        SliceVMut_try_subslice((self), (b), (e), (dest), (err), sizeof(T))
#    define m_SliceVTMut_try_subslice_mut(T, self, b, e, dest, err) \
        SliceVMut_try_subslice_mut((self), (b), (e), (dest), (err), sizeof(T))
#    define m_SliceVTMut_try_move_from(T, self, src, err) \
        SliceVMut_try_move_from((self), (src), (err), sizeof(T))
#    define m_SliceVTMut_try_copy_from(T, self, src, err) \
        SliceVMut_try_copy_from((self), (src), (err), sizeof(T), ##T_try_copy_from, ##T_destroy)
#    define m_SliceVTMut_as_slice(T, self, s) SliceVMut_as_slice((self), (s))
#    define m_SliceVTMut_as_slice_mut(T, self, s) SliceVMut_as_slice_mut((self), (s))
#    define m_SliceVTMut_iter(T, self, it) SliceVMut_iter((self), (it))
#    define m_SliceVTMut_iter_mut(T, self, it) SliceVMut_iter_mut((self), (it))

struct SliceVTMutIter_ {
    SliceVMutIter inner;
};

void SliceVTMutIter_destroy(SliceVTMutIter *const self);
void SliceVTMutIter_move(SliceVTMutIter *const self, SliceVTMutIter *const src);
bool WARN_UNUSED_RESULT SliceVTMutIter_try_copy(SliceVTMutIter *const arr,
                                                SliceVTMutIter const *const src,
                                                Error *const err);
T *SliceVTMutIter_next(SliceVTMutIter *const self);
void SliceVTMutIter_new(SliceVTMutIter *const self, T *const b, T *const e);

#    define m_SliceVMutIter_destroy(T, self) SliceVMutIter_destroy((self))
#    define m_SliceVMutIter_move(T, self, src) SliceVMutIter_move((self), (src))
#    define m_SliceVMutIter_try_copy(T, self, src, err) SliceVMutIter_try_copy((self), (src), (err))
#    define m_SliceVMutIter_next(T, self) SliceVMutIter_next((self), sizeof(T))
#    define m_SliceVMutIter_new(T, self, b, e) SliceVMutIter_new((self), (b), (e))

#endif //! defined(COBJ_SLICEVT_H)
