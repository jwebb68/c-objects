#if !defined(COBJ_VECTORV_H)
#    define COBJ_VECTORV_H

/**
 * cobj_VectorV
 *
 * The underlying implementation for a container of dynamically set initialised Ts.
 *
 * This is the non-heap version, ( well a private heap).
 *
 * The Array owns the set values, so when destroyed, will destroy the Ts set into it.
 *
 * This version is the underlying implementation for all cobj_VectorV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to cobj_VectorT.
 */

typedef struct cobj_VectorV_s cobj_VectorV;

#    include <cobj/based/cobj_slicev.h>
#    include <cobj/core/cobj_defs.h> // WARN_RESULT
#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct cobj_VectorV_s {
    uint8_t *ptr;
    size_t alloc;
    size_t len;
};

void cobj_VectorV_destroy_member(cobj_VectorV *const self,
                                 void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                 void (*elem_destroy)(void *const));
void cobj_VectorV_destroy(cobj_VectorV *const self,
                          void *(*elem_ptr_mut)(void *const arr, size_t idx),
                          void (*elem_destroy)(void *const));

void cobj_VectorV_move_member(cobj_VectorV *const self, cobj_VectorV *const src);
void cobj_VectorV_move(cobj_VectorV *const self, cobj_VectorV *const src);

void cobj_VectorV_new(cobj_VectorV *const self, void *const ptr, size_t alloc);
bool cobj_VectorV_is_empty(cobj_VectorV const *const self);
void cobj_VectorV_clear(cobj_VectorV *const self,
                        void *(*elem_ptr_mut)(void *const arr, size_t idx),
                        void (*elem_destroy)(void *const));
size_t cobj_VectorV_len(cobj_VectorV const *const self);
bool WARN_UNUSED_RESULT cobj_VectorV_push_back(cobj_VectorV *const self,
                                               void *const elem,
                                               void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                               void (*elem_move)(void *const, void *const));
bool WARN_UNUSED_RESULT cobj_VectorV_pop_back(cobj_VectorV *const self,
                                              void *const elem,
                                              void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                              void (*elem_move)(void *const, void *const));
void *cobj_VectorV_try_get_mut(cobj_VectorV const *const self,
                               cobj_Index pos,
                               void *(*elem_ptr_mut)(void *const arr, size_t idx));
void const *cobj_VectorV_try_get(cobj_VectorV const *const self,
                                 cobj_Index pos,
                                 void const *(*elem_ptr)(void const *const arr, size_t idx));

void cobj_VectorV_as_cobj_SliceV(cobj_VectorV const *const self, cobj_SliceV *const s);
void cobj_VectorV_as_cobj_SliceVMut(cobj_VectorV const *const self, cobj_SliceVMut *const s);

void cobj_VectorV_iter(cobj_VectorV const *const self,
                       cobj_SliceVIter *const it,
                       void const *(*elem_ptr)(void const *const arr, size_t idx));
void cobj_VectorV_iter_mut(cobj_VectorV const *const self,
                           cobj_SliceVMutIter *const it,
                           void *(*elem_ptr_mut)(void *const arr, size_t idx));

#endif // ! defined(COBJ_VECTORV_H)
