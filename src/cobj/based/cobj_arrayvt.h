#if !defined(COBJ_ARRAYVT_H)
#    define COBJ_ARRAYVT_H

/**
 * cobj_ArrayV<T>
 *
 * A contiguous block of initialised Ts.
 * Array owns the block, so when destroyed, will destroy the Ts.
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

typedef struct cobj_ArrayVT_s cobj_ArrayVT;

#    include <cobj/based/cobj_arrayv.h>
#    include <cobj/based/cobj_slicevt.h>
#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct cobj_ArrayVT_s {
    cobj_ArrayV inner;
};

void cobj_ArrayVT_destroy_member(cobj_ArrayVT *const self);
void cobj_ArrayVT_destroy(cobj_ArrayVT *const self);

void cobj_ArrayVT_move(cobj_ArrayVT *const self, cobj_ArrayVT *const src);

// copy src into self, but self is uninitialised and so nothing to copy to.
// bool WARN_UNUSED_RESULT cobj_ArrayVT_try_copy(cobj_ArrayVT *const self, cobj_ArrayVT const *const
// src, cobj_Error*err);

void cobj_ArrayVT_default(cobj_ArrayVT *const self, T *const arr, size_t len);

void cobj_ArrayVT_own(cobj_ArrayVT *const self, T *const arr, size_t len);
void cobj_ArrayVT_disown(cobj_ArrayVT *const self, T **const arr, size_t *const len);

void cobj_ArrayVT_as_cobj_SliceT(cobj_ArrayVT const *const self, cobj_SliceVT *const s);
void cobj_ArrayVT_as_cobj_SliceTMut(cobj_ArrayVT *const self, cobj_SliceVTMut *const s);

void cobj_ArrayVT_iter(cobj_ArrayVT const *const self, cobj_SliceVTIter *const it);
void cobj_ArrayVT_iter_mut(cobj_ArrayVT *const self, cobj_SliceVTMutIter *const it);

#endif //! defined(COBJ_ARRAYVT_H)
