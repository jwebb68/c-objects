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
typedef struct cobj_VectorT_s cobj_VectorT;

// includes
#    include <cobj/baseless/cobj_slicet.h>
#    include <cobj/t.h>
#    include <stddef.h> // size_t

// defines
typedef size_t cobj_Index;

// maybe vector can take a private heap struct, (if private heap is needed elsewhere)
// TODO: cobj_VectorT_malloc/cobj_VectorT_free/cobj_VectorT_malloc_new/cobj_VectorT_free_destroy.

struct cobj_VectorT_s {
    T *ptr;
    size_t alloc;
    size_t len;
};

void cobj_VectorT_destroy_member(cobj_VectorT *const self);
void cobj_VectorT_destroy(cobj_VectorT *const self);
void cobj_VectorT_move_member(cobj_VectorT *const self, cobj_VectorT *const src);
void cobj_VectorT_move(cobj_VectorT *const self, cobj_VectorT *const src);
void cobj_VectorT_new(cobj_VectorT *const self, T *const ptr, size_t alloc);
void cobj_VectorT_clear(cobj_VectorT *const self);
bool cobj_VectorT_is_empty(cobj_VectorT const *const self);
size_t cobj_VectorT_len(cobj_VectorT const *const self);
size_t cobj_VectorT_alloc(cobj_VectorT const *const self);
bool WARN_UNUSED_RESULT cobj_VectorT_realloc(cobj_VectorT *const self, size_t newalloc);
bool WARN_UNUSED_RESULT cobj_VectorT_push_back(cobj_VectorT *const self, T *const item);
bool WARN_UNUSED_RESULT cobj_VectorT_pop_back(cobj_VectorT *const self, T *const item);
T *cobj_VectorT_try_get_mut(cobj_VectorT const *const self, cobj_Index pos);
T const *cobj_VectorT_try_get(cobj_VectorT const *const self, cobj_Index pos);

void cobj_VectorT_as_cobj_SliceT(cobj_VectorT const *const self, cobj_SliceT *const s);
void cobj_VectorT_as_cobj_SliceTMut(cobj_VectorT const *const self, cobj_SliceTMut *const s);

// iters can iter over contents of vector WITHOUT needing to bounds check
// each access (unlike get_item_at) so faster than for looped cobj_Index access.
void cobj_VectorT_iter(cobj_VectorT const *const self, cobj_SliceTIter *const it);
void cobj_VectorT_iter_mut(cobj_VectorT const *const self, cobj_SliceTMutIter *const it);

#endif //! defined(COBJ_VECTORT_H)
