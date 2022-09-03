#if !defined(COBJ_SLICEV_H)
#    define COBJ_SLICEV_H

/**
 * SliceV, SliceV: The undelying implementation for a non-owned region of initialised Ts.
 *
 * This version is the underlying implementation for all SliceV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to SliceT.
 */

typedef struct SliceV_ SliceV;
typedef struct SliceVIter_ SliceVIter;

typedef struct SliceVMut_ SliceVMut;
typedef struct SliceVMutIter_ SliceVMutIter;

#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct SliceV_ {
    uint8_t const *ptr;
    size_t len; // n_elems
};

// slice does not own what it points to, so no need to destroy elems
void SliceV_wipe(SliceV *const self);
void SliceV_destroy_member(SliceV *const self);
void SliceV_destroy(SliceV *const self);

// slice does not own what it points to, so no need to move elems.
void SliceV_move_member(SliceV *const self, SliceV *const src);
void SliceV_move(SliceV *const self, SliceV *const src);

// slice does nto own what it points to, so no need to copy elems
// this is just about copying the slice, not the contents of the slice.
bool WARN_UNUSED_RESULT SliceV_try_copy(SliceV *const self,
                                        SliceV const *const src,
                                        Error *const err);

// slice holds ptr + len (num elements)
void SliceV_new(SliceV *const self, void const *ptr, size_t len);

void const *SliceV_ptr(SliceV const *const self);
size_t SliceV_len(SliceV const *const self);
bool SliceV_is_empty(SliceV const *const self);

// null if not valid pos
void const *SliceV_try_get(SliceV const *const self,
                           size_t pos,
                           void const *(*elem_ptr)(void const *const arr, size_t idx));

bool SliceV_try_subslice(SliceV const *const self,
                         size_t b,
                         size_t e,
                         SliceV *const dest,
                         Error *err,
                         void const *(*elem_ptr)(void const *const arr, size_t idx));

void SliceV_iter(SliceV const *const self,
                 SliceVIter *const it,
                 void const *(*elem_ptr)(void const *const arr, size_t idx));

struct SliceVIter_ {
    uint8_t const *p;
    uint8_t const *e;
};

void SliceVIter_wipe(SliceVIter *const self);
void SliceVIter_destroy_member(SliceVIter *const self);
void SliceVIter_destroy(SliceVIter *const self);
void SliceVIter_move_member(SliceVIter *const self, SliceVIter *const src);
void SliceVIter_move(SliceVIter *const self, SliceVIter *const src);
bool WARN_UNUSED_RESULT SliceVIter_try_copy(SliceVIter *const self,
                                            SliceVIter const *const src,
                                            Error *const err);
void const *SliceVIter_next(SliceVIter *const self, size_t elem_size);
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len
void SliceVIter_new(SliceVIter *const self, void const *const b, void const *const e);
// void SliceVIter_new(SliceVIter *const self,
//                        void const *const ptr,
//                        size_t len,
//                        void const *(*elem_ptr)(void const *const arr, size_t idx));

struct SliceVMut_ {
    uint8_t *ptr;
    size_t len;
};

void SliceVMut_wipe(SliceVMut *const self);
void SliceVMut_destroy_member(SliceVMut *const self);
void SliceVMut_destroy(SliceVMut *const self);
void SliceVMut_move_member(SliceVMut *const self, SliceVMut *const src);
void SliceVMut_move(SliceVMut *const self, SliceVMut *const src);
bool WARN_UNUSED_RESULT SliceVMut_try_copy(SliceVMut *const self,
                                           SliceVMut const *const src,
                                           Error *const err);
void SliceVMut_new(SliceVMut *const self, void *const ptr, size_t len);

void *SliceVMut_ptr(SliceVMut const *const self);
size_t SliceVMut_len(SliceVMut const *const self);
bool SliceVMut_is_empty(SliceVMut const *const self);

// null if not valid pos
void *SliceVMut_try_get(SliceVMut const *const self,
                        size_t pos,
                        void *(*elem_ptr_mut)(void *const arr, size_t idx));

bool SliceVMut_try_subslice(SliceVMut const *const self,
                            size_t b,
                            size_t e,
                            SliceV *const dest,
                            Error *err,
                            void *(*elem_ptr_mut)(void *const arr, size_t idx));
bool SliceVMut_try_subslice_mut(SliceVMut const *const self,
                                size_t b,
                                size_t e,
                                SliceVMut *const dest,
                                Error *err,
                                void *(*elem_ptr_mut)(void *const arr, size_t idx));

void SliceVMut_as_SliceV(SliceVMut const *const self, SliceV *const s);

bool WARN_UNUSED_RESULT SliceVMut_try_move_from(SliceVMut const *const self,
                                                SliceVMut const *const src,
                                                Error *const err,
                                                void *(*elem_ptr_mut)(void *const arr, size_t idx));

bool WARN_UNUSED_RESULT
SliceVMut_try_copy_from(SliceVMut const *const self,
                        SliceV const *const src,
                        Error *const err,
                        void const *(*elem_ptr)(void const *const arr, size_t idx),
                        void *(*elem_ptr_mut)(void *const arr, size_t idx),
                        bool (*elem_try_copy)(void *, void const *, Error *),
                        void (*elem_destroy)(void *));

void SliceVMut_iter(SliceVMut const *const self,
                    SliceVIter *const it,
                    void *(*elem_ptr_mut)(void *const arr, size_t idx));
void SliceVMut_iter_mut(SliceVMut const *const self,
                        SliceVMutIter *const it,
                        void *(*elem_ptr_mut)(void *const arr, size_t idx));

struct SliceVMutIter_ {
    uint8_t *p;
    uint8_t *e;
};

void SliceVMutIter_wipe(SliceVMutIter *const self);
void SliceVMutIter_destroy_member(SliceVMutIter *const self);
void SliceVMutIter_destroy(SliceVMutIter *const self);
void SliceVMutIter_move_member(SliceVMutIter *const self, SliceVMutIter *const src);
void SliceVMutIter_move(SliceVMutIter *const self, SliceVMutIter *const src);
bool WARN_UNUSED_RESULT SliceVMutIter_try_copy(SliceVMutIter *const arr,
                                               SliceVMutIter const *const src,
                                               Error *const err);
void *SliceVMutIter_next(SliceVMutIter *const self, size_t elem_size);
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len
void SliceVMutIter_new(SliceVMutIter *const self, void *const b, void *const e);
// void SliceVMutIter_new(SliceVMutIter *const self,
//                           void *const ptr,
//                           size_t len,
//                           void *(*elem_ptr_mut)(void *const arr, size_t idx));

#endif //! defined(COBJ_SLICEV_H)
