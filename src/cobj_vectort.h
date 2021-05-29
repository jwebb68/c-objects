#if !defined(COBJ_VECTORT_H)
#define COBJ_VECTORT_H

/**
 * A Vector 'pattern' for C
*/

//forward declares
typedef struct VectorT_ VectorT;
typedef struct VectorTIter_ VectorTIter;
typedef struct VectorTIterMut_ VectorTIterMut;

// includes
#include "t.h"
#include "cobj_slicet.h"

#include <stddef.h> // size_t

// defines
typedef size_t Index;

struct VectorT_ {
    T *arr;
    T *arr_end;
    T *arr_pos;
};

void VectorT_destroy(VectorT *const self);
void VectorT_move(VectorT *const self, VectorT *const src);
void VectorT_new(VectorT *const self, T *const arr, size_t len);
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

void VectorT_iter(VectorT const *const self, SliceTIter *const it);
void VectorT_iter_mut(VectorT *const self, SliceTMutIter *const it);

// void VectorT_iter(VectorT const *const self, VectorTIter *const it);
// void VectorT_iter_mut(VectorT *const self, VectorTIterMut *const it);

// maybe vector can take a private heap struct, (if private heap is needed elsewhere)
// TODO: VectorT_malloc/VectorT_free/VectorT_malloc_new/VectorT_free_destroy.

// iters can iter over contents of vector WITHOUT needing to bounds check
// so faster.

// struct VectorTIterMut_ {
//     VectorT *vec;
//     T *it;
// };
// void VectorTIterMut_destroy(VectorTIterMut *const self);
// void VectorTIterMut_move(VectorTIterMut *const self, VectorTIterMut *const src);
// // initialises iter, sets start at first item
// void VectorTIterMut_new(VectorTIterMut *const self, VectorT *const vec);
// // moves to next item, returns ptr to item or NULL if no more items.
// T * WARN_UNUSED_RESULT VectorTIterMut_next(VectorTIterMut *const self);
// // annoyingly, it's 2 compares per call + 1 in the caller in the for-loop
// // 3 ifs..

// struct VectorTIter_ {
//     VectorT const *vec;
//     T const *it;
// };
// void VectorTIter_destroy(VectorTIter *const self);
// void VectorTIter_move(VectorTIter *const self, VectorTIter *const src);
// // initialises iter, sets start at first item
// // does not return first item.
// void VectorTIter_new(VectorTIter *const self, VectorT const *const vec);
// // moves to next item, returns ptr to item or NULL if no more items.
// T const * WARN_UNUSED_RESULT VectorTIter_next(VectorTIter *const self);
// // annoyingly, it's 2 compares per call + 1 in the caller in the for-loop
// // 3 ifs..


#endif//!defined(COBJ_VECTORT_H)
