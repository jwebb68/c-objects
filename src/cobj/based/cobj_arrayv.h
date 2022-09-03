#if !defined(COBJ_ARRAYV_H)
#    define COBJ_ARRAYV_H

/**
 * ArrayV
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

typedef struct ArrayV_s ArrayV;

#    include <cobj/based/cobj_slicev.h>
#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct ArrayV_s {
    uint8_t *ptr;
    size_t len; // number of elems.
};

// array owns what it points to, so destroying self destroys pointed to.
void ArrayV_destroy_member(ArrayV *const self,
                           void *(*elem_ptr_mut)(void *const arr, size_t idx),
                           void (*elem_destroy_member)(void *const elem));
void ArrayV_destroy(ArrayV *const self,
                    void *(*elem_ptr_mut)(void *const arr, size_t idx),
                    void (*elem_destroy)(void *const elem));

// is this moving array struct or array contents..?
// struct not contents.
void ArrayV_move(ArrayV *const self, ArrayV *const src);

// is this copying array struct or array contents..?
// contents are not shared, so contents and struct.
// but self must be uninitialised, so cannot do..
// bool WARN_UNUSED_RESULT ArrayV_try_copy(ArrayV *const self,
//                                         ArrayV const *const src,
//                                         Error *const err,
//                                         void const *(*elem_ptr)(void const *const arr, size_t
//                                         idx), void *(*elem_ptr_mut)(void *const arr, size_t idx),
//                                         bool(elem_try_copy)(void *const dest,
//                                                             void const *const src,
//                                                             Error *const err),
//                                         void (*elem_destroy)(void *const elem));

void ArrayV_own(ArrayV *const self, void *const ptr, size_t len);
void ArrayV_disown(ArrayV *const self, void **const ptr, size_t *const len);

void ArrayV_default(ArrayV *const self,
                    void *const arr,
                    size_t len,
                    void *(*elem_ptr_mut)(void *const arr, size_t idx),
                    void (*elem_default)(void *const elem));

void ArrayV_as_SliceV(ArrayV const *const self, SliceV *const s);
void ArrayV_as_SliceVMut(ArrayV *const self, SliceVMut *const s);

void ArrayV_iter(ArrayV const *const self,
                 SliceVIter *const it,
                 void const *(*elem_ptr)(void const *const arr, size_t idx));
void ArrayV_iter_mut(ArrayV *const self,
                     SliceVMutIter *const it,
                     void *(*elem_ptr_mut)(void *const arr, size_t idx));

#endif // defined(COBJ_ARRAYV_H)
