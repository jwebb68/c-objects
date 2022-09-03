#if !defined(COBJ_CARRAYVT_H)
#    define COBJ_CARRAYVT_H

/**
 * cobj_CArrayV<T>
 *
 * A contiguous block of initialised Ts.
 * Array owns the block, so when destroyed, will destroy the Ts.
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// no structs as intended to work with normal carrays

#    include <cobj/based/cobj_carrayv.h>
#    include <cobj/based/cobj_slicevt.h>
#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

void cobj_CArrayVT_destroy_member(T *const arr, size_t len);
void cobj_CArrayVT_destroy(T *const arr, size_t len);

void cobj_CArrayVT_move_member(T *const arr, size_t len, T *const src);
void cobj_CArrayVT_move(T *const arr, size_t len, T *const src);

bool WARN_UNUSED_RESULT cobj_CArrayVT_try_copy(T *const arr,
                                               size_t len,
                                               T const *const src,
                                               cobj_Error *err);

bool WARN_UNUSED_RESULT cobj_CArrayVT_try_fill(T *const arr,
                                               size_t len,
                                               T const *const v,
                                               cobj_Error *err);

void cobj_CArrayVT_default(T *const arr, size_t len);
#    if 0
    void cobj_CArrayVT_default(T *const b, T *const e);
#    endif

T *cobj_CArrayVT_get_mut(T *const arr, size_t idx);
T const *cobj_CArrayVT_get(T const *const arr, size_t idx);

void cobj_CArrayVT_as_cobj_SliceVT(T const *const arr, size_t len, cobj_SliceVT *const s);
void cobj_CArrayVT_as_cobj_SliceVTMut(T *const arr, size_t len, cobj_SliceVTMut *const s);

void cobj_CArrayVT_iter(T const *const arr, size_t len, cobj_SliceVTIter *const it);
void cobj_CArrayVT_iter_mut(T *const arr, size_t len, cobj_SliceVTMutIter *const it);

#endif //! defined(COBJ_CARRAYVT_H)
