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

#    include <cobj/baseless/cobj_slicet.h>
#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

T const *cobj_CArrayT_try_get(T const *const arr, size_t len, size_t pos);
void cobj_CArrayT_as_cobj_SliceT(T const *const arr, size_t len, cobj_SliceT *const s);
void cobj_CArrayT_iter(T const *const arr, size_t len, cobj_SliceTIter *const it);

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
void cobj_CArrayTMut_destroy_member(T *const arr, size_t len);
void cobj_CArrayTMut_destroy(T *const arr, size_t len);

void cobj_CArrayTMut_move_member(T *const arr, size_t len, T *const src);
void cobj_CArrayTMut_move(T *const arr, size_t len, T *const src);

// copy from initialised [src,len) to uninitialised [arr,len)
bool WARN_UNUSED_RESULT cobj_CArrayTMut_try_copy(T *const arr,
                                                 size_t len,
                                                 T const *const src,
                                                 cobj_Error *err);

bool WARN_UNUSED_RESULT cobj_CArrayTMut_try_fill(T *const arr,
                                                 size_t len,
                                                 T const *const v,
                                                 cobj_Error *err);

void cobj_CArrayTMut_default(T *const arr, size_t len);
// should this be: fill_with()?
// void cobj_CArrayTMut_new(T *const arr, size_t len, void (*elem_init)(T *const elem));

void cobj_CArrayTMut_erase_member(T *const arr, size_t b, size_t len);

void cobj_CArrayTMut_erase(T *const arr, size_t b, size_t len);

T *cobj_CArrayTMut_try_get(T *const arr, size_t len, size_t pos);

void cobj_CArrayTMut_as_cobj_SliceT(T *const arr, size_t len, cobj_SliceT *const s);

void cobj_CArrayTMut_as_cobj_SliceTMut(T *const arr, size_t len, cobj_SliceTMut *const s);

void cobj_CArrayTMut_iter(T *const arr, size_t len, cobj_SliceTIter *const it);
void cobj_CArrayTMut_iter_mut(T *const arr, size_t len, cobj_SliceTMutIter *const it);

#    if 0
    T *cobj_CArrayT_malloc(size_t len);
    T *cobj_CArrayT_realloc(T *const prev, size_t len);
    void cobj_CArrayT_free(T *const arr);

#    endif

#endif // ! defined(COBJ_CARRAYT_H)
