#if !defined(COBJ_SLICEV_H)
#    define COBJ_SLICEV_H

/**
 * cobj_SliceV, cobj_SliceV: The undelying implementation for a non-owned region of initialised Ts.
 *
 * This version is the underlying implementation for all cobj_SliceV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to cobj_SliceT.
 */

typedef struct cobj_SliceV_s cobj_SliceV;
typedef struct cobj_SliceVIter_s cobj_SliceVIter;

typedef struct cobj_SliceVMut_s cobj_SliceVMut;
typedef struct cobj_SliceVMutIter_s cobj_SliceVMutIter;

#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct cobj_SliceV_s {
    uint8_t const *ptr;
    size_t len; // n_elems
};

// slice does not own what it points to, so no need to destroy elems
void cobj_SliceV_wipe(cobj_SliceV *const self);
void cobj_SliceV_destroy_member(cobj_SliceV *const self);
void cobj_SliceV_destroy(cobj_SliceV *const self);

// slice does not own what it points to, so no need to move elems.
void cobj_SliceV_move_member(cobj_SliceV *const self, cobj_SliceV *const src);
void cobj_SliceV_move(cobj_SliceV *const self, cobj_SliceV *const src);

// slice does nto own what it points to, so no need to copy elems
// this is just about copying the slice, not the contents of the slice.
bool WARN_UNUSED_RESULT cobj_SliceV_try_copy(cobj_SliceV *const self,
                                             cobj_SliceV const *const src,
                                             cobj_Error *const err);

// slice holds ptr + len (num elements)
void cobj_SliceV_new(cobj_SliceV *const self, void const *ptr, size_t len);

void const *cobj_SliceV_ptr(cobj_SliceV const *const self);
size_t cobj_SliceV_len(cobj_SliceV const *const self);
bool cobj_SliceV_is_empty(cobj_SliceV const *const self);

// null if not valid pos
void const *cobj_SliceV_try_get(cobj_SliceV const *const self,
                                size_t pos,
                                void const *(*elem_ptr)(void const *const arr, size_t idx));

bool cobj_SliceV_try_subslice(cobj_SliceV const *const self,
                              size_t b,
                              size_t e,
                              cobj_SliceV *const dest,
                              cobj_Error *err,
                              void const *(*elem_ptr)(void const *const arr, size_t idx));

void cobj_SliceV_iter(cobj_SliceV const *const self,
                      cobj_SliceVIter *const it,
                      void const *(*elem_ptr)(void const *const arr, size_t idx));

struct cobj_SliceVIter_s {
    uint8_t const *p;
    uint8_t const *e;
};

void cobj_SliceVIter_wipe(cobj_SliceVIter *const self);
void cobj_SliceVIter_destroy_member(cobj_SliceVIter *const self);
void cobj_SliceVIter_destroy(cobj_SliceVIter *const self);
void cobj_SliceVIter_move_member(cobj_SliceVIter *const self, cobj_SliceVIter *const src);
void cobj_SliceVIter_move(cobj_SliceVIter *const self, cobj_SliceVIter *const src);
bool WARN_UNUSED_RESULT cobj_SliceVIter_try_copy(cobj_SliceVIter *const self,
                                                 cobj_SliceVIter const *const src,
                                                 cobj_Error *const err);
void const *cobj_SliceVIter_next(cobj_SliceVIter *const self, size_t elem_size);
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len
void cobj_SliceVIter_new(cobj_SliceVIter *const self, void const *const b, void const *const e);
// void cobj_SliceVIter_new(cobj_SliceVIter *const self,
//                        void const *const ptr,
//                        size_t len,
//                        void const *(*elem_ptr)(void const *const arr, size_t idx));

struct cobj_SliceVMut_s {
    uint8_t *ptr;
    size_t len;
};

void cobj_SliceVMut_wipe(cobj_SliceVMut *const self);
void cobj_SliceVMut_destroy_member(cobj_SliceVMut *const self);
void cobj_SliceVMut_destroy(cobj_SliceVMut *const self);
void cobj_SliceVMut_move_member(cobj_SliceVMut *const self, cobj_SliceVMut *const src);
void cobj_SliceVMut_move(cobj_SliceVMut *const self, cobj_SliceVMut *const src);
bool WARN_UNUSED_RESULT cobj_SliceVMut_try_copy(cobj_SliceVMut *const self,
                                                cobj_SliceVMut const *const src,
                                                cobj_Error *const err);
void cobj_SliceVMut_new(cobj_SliceVMut *const self, void *const ptr, size_t len);

void *cobj_SliceVMut_ptr(cobj_SliceVMut const *const self);
size_t cobj_SliceVMut_len(cobj_SliceVMut const *const self);
bool cobj_SliceVMut_is_empty(cobj_SliceVMut const *const self);

// null if not valid pos
void *cobj_SliceVMut_try_get(cobj_SliceVMut const *const self,
                             size_t pos,
                             void *(*elem_ptr_mut)(void *const arr, size_t idx));

bool cobj_SliceVMut_try_subslice(cobj_SliceVMut const *const self,
                                 size_t b,
                                 size_t e,
                                 cobj_SliceV *const dest,
                                 cobj_Error *err,
                                 void *(*elem_ptr_mut)(void *const arr, size_t idx));
bool cobj_SliceVMut_try_subslice_mut(cobj_SliceVMut const *const self,
                                     size_t b,
                                     size_t e,
                                     cobj_SliceVMut *const dest,
                                     cobj_Error *err,
                                     void *(*elem_ptr_mut)(void *const arr, size_t idx));

void cobj_SliceVMut_as_cobj_SliceV(cobj_SliceVMut const *const self, cobj_SliceV *const s);

bool WARN_UNUSED_RESULT cobj_SliceVMut_try_move_from(cobj_SliceVMut const *const self,
                                                     cobj_SliceVMut const *const src,
                                                     cobj_Error *const err,
                                                     void *(*elem_ptr_mut)(void *const arr,
                                                                           size_t idx));

bool WARN_UNUSED_RESULT
cobj_SliceVMut_try_copy_from(cobj_SliceVMut const *const self,
                             cobj_SliceV const *const src,
                             cobj_Error *const err,
                             void const *(*elem_ptr)(void const *const arr, size_t idx),
                             void *(*elem_ptr_mut)(void *const arr, size_t idx),
                             bool (*elem_try_copy)(void *, void const *, cobj_Error *),
                             void (*elem_destroy)(void *));

void cobj_SliceVMut_iter(cobj_SliceVMut const *const self,
                         cobj_SliceVIter *const it,
                         void *(*elem_ptr_mut)(void *const arr, size_t idx));
void cobj_SliceVMut_iter_mut(cobj_SliceVMut const *const self,
                             cobj_SliceVMutIter *const it,
                             void *(*elem_ptr_mut)(void *const arr, size_t idx));

struct cobj_SliceVMutIter_s {
    uint8_t *p;
    uint8_t *e;
};

void cobj_SliceVMutIter_wipe(cobj_SliceVMutIter *const self);
void cobj_SliceVMutIter_destroy_member(cobj_SliceVMutIter *const self);
void cobj_SliceVMutIter_destroy(cobj_SliceVMutIter *const self);
void cobj_SliceVMutIter_move_member(cobj_SliceVMutIter *const self, cobj_SliceVMutIter *const src);
void cobj_SliceVMutIter_move(cobj_SliceVMutIter *const self, cobj_SliceVMutIter *const src);
bool WARN_UNUSED_RESULT cobj_SliceVMutIter_try_copy(cobj_SliceVMutIter *const arr,
                                                    cobj_SliceVMutIter const *const src,
                                                    cobj_Error *const err);
void *cobj_SliceVMutIter_next(cobj_SliceVMutIter *const self, size_t elem_size);
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len
void cobj_SliceVMutIter_new(cobj_SliceVMutIter *const self, void *const b, void *const e);
// void cobj_SliceVMutIter_new(cobj_SliceVMutIter *const self,
//                           void *const ptr,
//                           size_t len,
//                           void *(*elem_ptr_mut)(void *const arr, size_t idx));

#endif //! defined(COBJ_SLICEV_H)
