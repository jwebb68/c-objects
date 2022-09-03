#if !defined(COBJ_CARRAYV_H)
#    define COBJ_CARRAYV_H

/**
 * CArrayV
 *
 * The underlying implementation for a container of initialised Ts.
 *
 * This is the non-heap version, ( well a private heap).
 *
 * The Array owns the set values, so when destroyed, will destroy the Ts set into it.
 *
 * This version is the underlying implementation for all ArrayV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to ArrayT.
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// no structs as intended to work with normal carrays

#    include <cobj/based/cobj_slicev.h>
#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

void CArrayV_destroy_member(void *const arr,
                            size_t len,
                            void *(*elem_ptr_mut)(void *const arr, size_t idx),
                            void (*elem_destroy)(void *const elem));
void CArrayV_destroy(void *const arr,
                     size_t len,
                     void *(*elem_ptr_mut)(void *const arr, size_t idx),
                     void (*elem_destroy)(void *const elem));

void CArrayV_move_member(void *const arr,
                         size_t len,
                         void *const src,
                         void *(*elem_ptr_mut)(void *const arr, size_t idx));
void CArrayV_move(void *const arr,
                  size_t len,
                  void *const src,
                  void *(*elem_ptr_mut)(void *const arr, size_t idx));

bool WARN_UNUSED_RESULT CArrayV_try_copy(void *const arr,
                                         size_t len,
                                         void const *const src,
                                         Error *const err,
                                         void const *(*elem_ptr)(void const *const arr, size_t idx),
                                         void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                         bool(try_elem_copy)(void *const dest,
                                                             void const *const src,
                                                             Error *const err),
                                         void (*elem_destroy)(void *const elem));

bool WARN_UNUSED_RESULT CArrayV_try_fill(void *const arr,
                                         size_t len,
                                         void const *const v,
                                         Error *const err,
                                         void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                         bool(try_elem_copy)(void *const dest,
                                                             void const *const src,
                                                             Error *const err),
                                         void (*elem_destroy)(void *const elem));

void CArrayV_default(void *const arr,
                     size_t len,
                     void *(*elem_ptr)(void *arr, size_t idx),
                     void (*elem_default)(void *const elem));

#    if 0
void CArrayV_default(void *const b,
                     void *const e,
                     size_t elem_size,
                     void (*elem_default)(void *const elem));
#    endif

void CArrayV_erase_member(void *const arr,
                          size_t b,
                          size_t len,
                          void *(*elem_ptr_mut)(void *const arr, size_t idx),
                          void (*elem_destroy)(void *const elem));
void CArrayV_erase(void *const arr,
                   size_t b,
                   size_t len,
                   void *(*elem_ptr_mut)(void *const arr, size_t idx),
                   void (*elem_destroy)(void *const elem));

void CArrayV_as_SliceV(void const *const arr, size_t len, SliceV *const s);
void CArrayV_as_SliceVMut(void *const arr, size_t len, SliceVMut *const s);

void CArrayV_iter(void const *const arr,
                  size_t len,
                  SliceVIter *const it,
                  void const *(*elem_ptr)(void const *arr, size_t idx));
void CArrayV_iter_mut(void *const arr,
                      size_t len,
                      SliceVMutIter *const it,
                      void *(*elem_ptr_mut)(void *arr, size_t idx));

#endif // defined(COBJ_CARRAYV_H)
