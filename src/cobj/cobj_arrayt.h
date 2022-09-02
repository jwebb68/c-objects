#if !defined(COBJ_ARRAYT_H)
#    define COBJ_ARRAYT_H

/**
 * Array<T>
 *
 * A contiguous block of initialised Ts.
 * Array owns the block, so when destroyed, will destroy the Ts.
 */
// Two types needed:
// 1. inline, but this puts the num-elems in the type
// 2. out-of-line, but needs the array already created in C.
// This is is the out-of-line version.

// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

typedef struct ArrayT_s ArrayT;
typedef struct ArrayTMut_s ArrayTMut;

#    include "cobj_defs.h" // WARN_UNUSED_RESULT
#    include "cobj_error.h"
#    include "cobj_slicet.h"
#    include "t.h"

#    include <stdbool.h>
#    include <stddef.h> // size_t

struct ArrayT_s {
    T const *ptr;
    size_t len;
};

// const array does not own the referred
void ArrayT_destroy(ArrayT *const self);
void ArrayT_destroy_member(ArrayT *const self);

// can move the obj, ownership of the referred moved. referred not moved.
void ArrayT_move(ArrayT *const self, ArrayT *const src);
void ArrayT_move_member(ArrayT *const self, ArrayT *const src);

// copying a const array?
// requires memory to copy to (if copying content).
// cannot have 2+ array structs referring to same content.
// so no copy of const array.
// copy const array into const array?

void ArrayT_own(ArrayT *const self, T const *const p, size_t len);
void ArrayT_disown(ArrayT *const self, T const **const p, size_t *const len);

void ArrayT_swap(ArrayT *const self, ArrayT *const other);

T const *ArrayT_ptr(ArrayT const *const self);

size_t ArrayT_len(ArrayT const *const self);

// checked get..
T const *ArrayT_try_get(ArrayT const *const self, Index pos);

// slices don't own referred i.e shared.
void ArrayT_as_SliceT(ArrayT const *const self, SliceT *const s);

// iters don't own referred i.e shared.
void ArrayT_iter(ArrayT const *const self, SliceTIter *const it);

// transfer of ownership?
void ArrayT_from_ArrayTMut(ArrayT *const self, ArrayTMut *const src);

// ============================================================================

// Does array own the referred?
// If so then it destroys the referred on destroy.
// then how can a mutable be cast to non-mutable?
struct ArrayTMut_s {
    T *ptr;
    size_t len;
};

void ArrayTMut_destroy_member(ArrayTMut *const self);
void ArrayTMut_destroy(ArrayTMut *const self);

void ArrayTMut_move(ArrayTMut *const self, ArrayTMut *const src);

// content copy.
// [in]self uninitialised, [out]self initialised.
// cannot do, copy requires location to write to.. and unintialised self does not hold this,
// bool WARN_UNUSED_RESULT ArrayTMut_try_copy_from(ArrayTMut *const self,
//                                            ArrayT const *const src,
//                                            Error *err);
// bool WARN_UNUSED_RESULT ArrayTMut_try_copy_from_mut(ArrayTMut *const self,
//                                                ArrayTMut const *const src,
//                                                Error *err);

// array i
// own an already initialised carray
void ArrayTMut_own(ArrayTMut *const self, T *const arr, size_t len);
// disown the owned carray, carray no longer managed, self uninitialised.
void ArrayTMut_disown(ArrayTMut *const self, T **const arr, size_t *const len);

void ArrayTMut_swap(ArrayTMut *const self, ArrayTMut *const other);

// own an uninitialised carray and initialise with the init fn.
void ArrayTMut_default(ArrayTMut *const self, T *const arr, size_t len);

//void ArrayTMut_fill_with(ArrayTMut *const self, T *const arr, size_t len, void (*elem_init)(T *const self));

T *ArrayTMut_ptr(ArrayTMut const *const self);
size_t ArrayTMut_len(ArrayTMut const *const self);

// cast to const without copying?
// what owns the referred?
// what happens when ArrayT is destroyed..?
// array can be mutated underneath arr..
// cast or convert?
void ArrayTMut_into_ArrayT(ArrayTMut *const self, ArrayT *const arr);
// void ArrayTMut_as_ArrayT(ArrayTMut const *const self, ArrayT *const arr);

// slices never own the referred.
void ArrayTMut_as_SliceT(ArrayTMut const *const self, SliceT *const s);
void ArrayTMut_as_SliceTMut(ArrayTMut const *const self, SliceTMut *const s);

// iters never own the referred.
void ArrayTMut_iter(ArrayTMut const *const self, SliceTIter *const it);
void ArrayTMut_iter_mut(ArrayTMut const *const self, SliceTMutIter *const it);

#endif //! defined(COBJ_ARRAYT_H)
