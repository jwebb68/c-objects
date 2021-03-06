#if !defined(COBJ_ARRAYT_H)
#    define COBJ_ARRAYT_H

/**
 * Array<T>
 *
 * A contiguous block of initialised Ts.
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

// void ArrayT_destroy_p(T *const arr, T *const end);
// void ArrayT_move_p(T *const arr, T *const src, T *const src_end);
// bool WARN_UNUSED_RESULT ArrayT_try_copy_p(T *const arr,
//                                           T const *const src,
//                                           T const *const src_end,
//                                           Error *err);

// void ArrayT_new_default_p(T *const arr, T *const end);

void ArrayT_destroy(T *const arr, size_t len);
void ArrayT_move(T *const arr, T *const src, size_t len);
bool WARN_UNUSED_RESULT ArrayT_try_copy(T *const arr, T const *const src, size_t len, Error *err);

void ArrayT_new_default(T *const arr, size_t len);

void ArrayT_as_slice(T const *const arr, size_t len, SliceT *const s);
void ArrayT_as_slice_mut(T *const arr, size_t len, SliceTMut *const s);

void ArrayT_iter(T const *const arr, size_t len, SliceTIter *const it);
void ArrayT_iter_mut(T *const arr, size_t len, SliceTMutIter *const it);

#endif //! defined(COBJ_ARRAYT_H)
