#if !defined(COBJ_VECTORV_H)
#    define COBJ_VECTORV_H

/**
 * VectorV
 *
 * The underlying implementation for a container of dynamically set initialised Ts.
 *
 * This is the non-heap version, ( well a private heap).
 *
 * The Array owns the set values, so when destroyed, will destroy the Ts set into it.
 *
 * This version is the underlying implementation for all VectorV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to VectorT.
 */

typedef struct VectorV_ VectorV;

#    include <cobj/based/cobj_slicev.h>
#    include <cobj/core/cobj_defs.h> // WARN_RESULT
#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct VectorV_ {
    uint8_t *ptr;
    size_t alloc;
    size_t len;
};

void VectorV_destroy_member(VectorV *const self,
                            void *(*elem_ptr_mut)(void *const arr, size_t idx),
                            void (*elem_destroy)(void *const));
void VectorV_destroy(VectorV *const self,
                     void *(*elem_ptr_mut)(void *const arr, size_t idx),
                     void (*elem_destroy)(void *const));

void VectorV_move_member(VectorV *const self, VectorV *const src);
void VectorV_move(VectorV *const self, VectorV *const src);

void VectorV_new(VectorV *const self, void *const ptr, size_t alloc);
bool VectorV_is_empty(VectorV const *const self);
void VectorV_clear(VectorV *const self,
                   void *(*elem_ptr_mut)(void *const arr, size_t idx),
                   void (*elem_destroy)(void *const));
size_t VectorV_len(VectorV const *const self);
bool WARN_UNUSED_RESULT VectorV_push_back(VectorV *const self,
                                          void *const elem,
                                          void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                          void (*elem_move)(void *const, void *const));
bool WARN_UNUSED_RESULT VectorV_pop_back(VectorV *const self,
                                         void *const elem,
                                         void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                         void (*elem_move)(void *const, void *const));
void *VectorV_try_get_mut(VectorV const *const self,
                          cobj_Index pos,
                          void *(*elem_ptr_mut)(void *const arr, size_t idx));
void const *VectorV_try_get(VectorV const *const self,
                            cobj_Index pos,
                            void const *(*elem_ptr)(void const *const arr, size_t idx));

void VectorV_as_SliceV(VectorV const *const self, SliceV *const s);
void VectorV_as_SliceVMut(VectorV const *const self, SliceVMut *const s);

void VectorV_iter(VectorV const *const self,
                  SliceVIter *const it,
                  void const *(*elem_ptr)(void const *const arr, size_t idx));
void VectorV_iter_mut(VectorV const *const self,
                      SliceVMutIter *const it,
                      void *(*elem_ptr_mut)(void *const arr, size_t idx));

#endif // ! defined(COBJ_VECTORV_H)
