#if !defined(COBJ_CARRAYT_H)
#    define COBJ_CARRAYT_H

/**
 * CArray<T>
 *
 * A contiguous block of initialised Ts, as per C..
 * Array owns the block, so when destroyed, will destroy the Ts.
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// no structs as intended to work with normal carrays

#    include "cobj_defs.h" // WARN_UNUSED_RESULT
#    include "cobj_error.h"
#    include "cobj_slicet.h"
#    include "t.h"

#    include <stdbool.h>
#    include <stddef.h> // size_t

T const *CArrayT_try_get(T const *const arr, size_t len, size_t pos);
void CArrayT_as_SliceT(T const *const arr, size_t len, SliceT *const s);
void CArrayT_iter(T const *const arr, size_t len, SliceTIter *const it);

// ===========================================================================

/**
 * destroy an array of T, where all members are initialised.
 *
 * @param arr the start of the array.
 * @param len the number of items in the array.
 *
 * @warning UB if len > 0 and arr to arr+len is not readable.
 * @warning UB if len > 0 and arr to arr+len is not writable.
 */
void CArrayTMut_destroy_member(T *const arr, size_t len);
void CArrayTMut_destroy(T *const arr, size_t len);

void CArrayTMut_move_member(T *const arr, size_t len, T *const src);
void CArrayTMut_move(T *const arr, size_t len, T *const src);

// copy from initialised [src,len) to uninitialised [arr,len)
bool WARN_UNUSED_RESULT CArrayTMut_try_copy(T *const arr,
                                            size_t len,
                                            T const *const src,
                                            Error *err);

bool WARN_UNUSED_RESULT CArrayTMut_try_fill(T *const arr, size_t len, T const *const v, Error *err);

void CArrayTMut_default(T *const arr, size_t len);
// should this be: fill_with()?
//void CArrayTMut_new(T *const arr, size_t len, void (*elem_init)(T *const elem));

void CArrayTMut_erase_member(T *const arr, size_t b, size_t len);

void CArrayTMut_erase(T *const arr, size_t b, size_t len);

T *CArrayTMut_try_get(T *const arr, size_t len, size_t pos);

void CArrayTMut_as_SliceT(T *const arr, size_t len, SliceT *const s);

void CArrayTMut_as_SliceTMut(T *const arr, size_t len, SliceTMut *const s);

void CArrayTMut_iter(T *const arr, size_t len, SliceTIter *const it);
void CArrayTMut_iter_mut(T *const arr, size_t len, SliceTMutIter *const it);

#    if 0
    T *CArrayT_malloc(size_t len);
    T *CArrayT_realloc(T *const prev, size_t len);
    void CArrayT_free(T *const arr);

#    endif

#endif // ! defined(COBJ_CARRAYT_H)
