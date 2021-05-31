#if !defined(COBJ_VECTORT_H)
#    define COBJ_VECTORT_H

/**
 * Vector<T>
 *
 * A container of dynamically set initialised Ts
 *
 * This is the non-heap version, ( well a private heap).
 *
 * The Array owns the set values, so when destroyed, will destroy the Ts set into it.
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// forward declares
typedef struct VectorT_ VectorT;

// includes
#    include "cobj_slicet.h"
#    include "t.h"

#    include <stddef.h> // size_t

// defines
typedef size_t Index;

// maybe vector can take a private heap struct, (if private heap is needed elsewhere)
// TODO: VectorT_malloc/VectorT_free/VectorT_malloc_new/VectorT_free_destroy.

struct VectorT_ {
    T *b;
    T *e;
    T *p;
};

void VectorT_destroy(VectorT *const self);
void VectorT_move(VectorT *const self, VectorT *const src);
void VectorT_new(VectorT *const self, T *const b, T *const e);
void VectorT_clear(VectorT *const self);
bool VectorT_is_empty(VectorT const *const self);
size_t VectorT_len(VectorT const *const self);
size_t VectorT_alloc(VectorT const *const self);
bool WARN_UNUSED_RESULT VectorT_realloc(VectorT *const self, size_t newalloc);
bool WARN_UNUSED_RESULT VectorT_push_back(VectorT *const self, T *const item);
bool WARN_UNUSED_RESULT VectorT_pop_back(VectorT *const self, T *const item);
T *VectorT_get_item_at_mut(VectorT const *const self, Index pos);
T const *VectorT_get_item_at(VectorT const *const self, Index pos);

void VectorT_as_slice(VectorT const *const self, SliceT *const s);
void VectorT_as_slice_mut(VectorT const *const self, SliceTMut *const s);

// iters can iter over contents of vector WITHOUT needing to bounds check
// each access (unlike get_item_at) so faster than for looped index access.
void VectorT_iter(VectorT const *const self, SliceTIter *const it);
void VectorT_iter_mut(VectorT const *const self, SliceTMutIter *const it);

#endif //! defined(COBJ_VECTORT_H)
