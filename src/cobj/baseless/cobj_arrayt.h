#if !defined(COBJ_cobj_ArrayT_H)
#    define COBJ_cobj_ArrayT_H

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

typedef struct cobj_ArrayT_s cobj_ArrayT;
typedef struct cobj_ArrayTMut_s cobj_ArrayTMut;

#    include <cobj/baseless/cobj_slicet.h>
#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct cobj_ArrayT_s {
    T const *ptr;
    size_t len;
};

// const array does not own the referred
void cobj_ArrayT_destroy(cobj_ArrayT *const self);
void cobj_ArrayT_destroy_member(cobj_ArrayT *const self);

// can move the obj, ownership of the referred moved. referred not moved.
void cobj_ArrayT_move(cobj_ArrayT *const self, cobj_ArrayT *const src);
void cobj_ArrayT_move_member(cobj_ArrayT *const self, cobj_ArrayT *const src);

// copying a const array?
// requires memory to copy to (if copying content).
// cannot have 2+ array structs referring to same content.
// so no copy of const array.
// copy const array into const array?

void cobj_ArrayT_own(cobj_ArrayT *const self, T const *const p, size_t len);
void cobj_ArrayT_disown(cobj_ArrayT *const self, T const **const p, size_t *const len);

void cobj_ArrayT_swap(cobj_ArrayT *const self, cobj_ArrayT *const other);

T const *cobj_ArrayT_ptr(cobj_ArrayT const *const self);

size_t cobj_ArrayT_len(cobj_ArrayT const *const self);

// checked get..
T const *cobj_ArrayT_try_get(cobj_ArrayT const *const self, cobj_Index pos);

// slices don't own referred i.e shared.
void cobj_ArrayT_as_cobj_SliceT(cobj_ArrayT const *const self, cobj_SliceT *const s);

// iters don't own referred i.e shared.
void cobj_ArrayT_iter(cobj_ArrayT const *const self, cobj_SliceTIter *const it);

// transfer of ownership?
void cobj_ArrayT_from_cobj_ArrayTMut(cobj_ArrayT *const self, cobj_ArrayTMut *const src);

// ============================================================================

// Does array own the referred?
// If so then it destroys the referred on destroy.
// then how can a mutable be cast to non-mutable?
struct cobj_ArrayTMut_s {
    T *ptr;
    size_t len;
};

void cobj_ArrayTMut_destroy_member(cobj_ArrayTMut *const self);
void cobj_ArrayTMut_destroy(cobj_ArrayTMut *const self);

void cobj_ArrayTMut_move(cobj_ArrayTMut *const self, cobj_ArrayTMut *const src);

// content copy.
// [in]self uninitialised, [out]self initialised.
// cannot do, copy requires location to write to.. and unintialised self does not hold this,
// bool WARN_UNUSED_RESULT cobj_ArrayTMut_try_copy_from(cobj_ArrayTMut *const self,
//                                            cobj_ArrayT const *const src,
//                                            cobj_Error *err);
// bool WARN_UNUSED_RESULT cobj_ArrayTMut_try_copy_from_mut(cobj_ArrayTMut *const self,
//                                                cobj_ArrayTMut const *const src,
//                                                cobj_Error *err);

// array i
// own an already initialised carray
void cobj_ArrayTMut_own(cobj_ArrayTMut *const self, T *const arr, size_t len);
// disown the owned carray, carray no longer managed, self uninitialised.
void cobj_ArrayTMut_disown(cobj_ArrayTMut *const self, T **const arr, size_t *const len);

void cobj_ArrayTMut_swap(cobj_ArrayTMut *const self, cobj_ArrayTMut *const other);

// own an uninitialised carray and initialise with the init fn.
void cobj_ArrayTMut_default(cobj_ArrayTMut *const self, T *const arr, size_t len);

// void cobj_ArrayTMut_fill_with(cobj_ArrayTMut *const self, T *const arr, size_t len, void
// (*elem_init)(T *const self));

T *cobj_ArrayTMut_ptr(cobj_ArrayTMut const *const self);
size_t cobj_ArrayTMut_len(cobj_ArrayTMut const *const self);

// cast to const without copying?
// what owns the referred?
// what happens when cobj_ArrayT is destroyed..?
// array can be mutated underneath arr..
// cast or convert?
void cobj_ArrayTMut_into_cobj_ArrayT(cobj_ArrayTMut *const self, cobj_ArrayT *const arr);
// void cobj_ArrayTMut_as_cobj_ArrayT(cobj_ArrayTMut const *const self, cobj_ArrayT *const arr);

// slices never own the referred.
void cobj_ArrayTMut_as_cobj_SliceT(cobj_ArrayTMut const *const self, cobj_SliceT *const s);
void cobj_ArrayTMut_as_cobj_SliceTMut(cobj_ArrayTMut const *const self, cobj_SliceTMut *const s);

// iters never own the referred.
void cobj_ArrayTMut_iter(cobj_ArrayTMut const *const self, cobj_SliceTIter *const it);
void cobj_ArrayTMut_iter_mut(cobj_ArrayTMut const *const self, cobj_SliceTMutIter *const it);

#endif //! defined(COBJ_cobj_ArrayT_H)
