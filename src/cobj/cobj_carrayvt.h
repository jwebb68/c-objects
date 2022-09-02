#if !defined(COBJ_CARRAYVT_H)
#    define COBJ_CARRAYVT_H

/**
 * CArrayV<T>
 *
 * A contiguous block of initialised Ts.
 * Array owns the block, so when destroyed, will destroy the Ts.
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// no structs as intended to work with normal carrays

#    include "cobj_carrayv.h"
#    include "cobj_defs.h" // WARN_UNUSED_RESULT
#    include "cobj_error.h"
#    include "cobj_slicet.h"
#    include "t.h"

#    include <stdbool.h>
#    include <stddef.h> // size_t

void CArrayVT_destroy_member(T *const arr, size_t len);
void CArrayVT_destroy(T *const arr, size_t len);

void CArrayVT_move_member(T *const arr, size_t len, T *const src);
void CArrayVT_move(T *const arr, size_t len, T *const src);

bool WARN_UNUSED_RESULT CArrayVT_try_copy(T *const arr, size_t len, T const *const src, Error *err);

bool WARN_UNUSED_RESULT CArrayVT_try_fill(T *const arr, size_t len, T const *const v, Error *err);

void CArrayVT_default(T *const arr, size_t len);
#    if 0
    void CArrayVT_default(T *const b, T *const e);
#    endif

T *CArrayVT_get_mut(T *const arr, size_t idx);
T const *CArrayVT_get(T const *const arr, size_t idx);

void CArrayVT_as_SliceT(T const *const arr, size_t len, SliceT *const s);
void CArrayVT_as_SliceTMut(T *const arr, size_t len, SliceTMut *const s);

void CArrayVT_iter(T const *const arr, size_t len, SliceTIter *const it);
void CArrayVT_iter_mut(T *const arr, size_t len, SliceTMutIter *const it);

#endif //! defined(COBJ_CARRAYVT_H)
