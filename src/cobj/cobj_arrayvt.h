#if !defined(COBJ_ARRAYVT_H)
#    define COBJ_ARRAYVT_H

/**
 * ArrayV<T>
 *
 * A contiguous block of initialised Ts.
 * Array owns the block, so when destroyed, will destroy the Ts.
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

typedef struct ArrayVT_s ArrayVT;

#    include "cobj_arrayv.h"
#    include "cobj_defs.h" // WARN_UNUSED_RESULT
#    include "cobj_error.h"
#    include "cobj_slicet.h"
#    include "t.h"

#    include <stdbool.h>
#    include <stddef.h> // size_t

struct ArrayVT_s {
    ArrayV inner;
};

void ArrayVT_destroy_member(ArrayVT *const self);
void ArrayVT_destroy(ArrayVT *const self);

void ArrayVT_move(ArrayVT *const self, ArrayVT *const src);

// copy src into self, but self is uninitialised and so nothing to copy to.
// bool WARN_UNUSED_RESULT ArrayVT_try_copy(ArrayVT *const self, ArrayVT const *const src, Error
// *err);

void ArrayVT_default(ArrayVT *const self, T *const arr, size_t len);

void ArrayVT_own(ArrayVT *const self, T *const arr, size_t len);
void ArrayVT_disown(ArrayVT *const self, T **const arr, size_t *const len);

void ArrayVT_as_SliceT(ArrayVT const *const self, SliceT *const s);
void ArrayVT_as_SliceTMut(ArrayVT *const self, SliceTMut *const s);

void ArrayVT_iter(ArrayVT const *const self, SliceTIter *const it);
void ArrayVT_iter_mut(ArrayVT *const self, SliceTMutIter *const it);

#endif //! defined(COBJ_ARRAYT_H)
