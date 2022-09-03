#if !defined(COBJ_BOX_H)
#    define COBJ_BOX_H

/**
 * Box<T>: object created and managed on the heap, without sharing.
 *
 * Box contents owned by the box and are not shared (use RCT for shared objects).
 * inherently Mutable; cannot move a const value into it, cannot copy into into a const box
 *
 * initialised Box content cannot be NULL, so always contains a value.
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// forward declares;
typedef struct cobj_BoxT_s cobj_BoxT;

// includes
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>

// defines
struct cobj_BoxT_s {
    T *elem;
};

void cobj_BoxT_destroy_member(cobj_BoxT *const self);
void cobj_BoxT_destroy(cobj_BoxT *const self);

void cobj_BoxT_move(cobj_BoxT *const self, cobj_BoxT *const src);

bool WARN_UNUSED_RESULT cobj_BoxT_try_copy_member(cobj_BoxT *const self,
                                                  cobj_BoxT const *const v,
                                                  cobj_Error *const err);
bool WARN_UNUSED_RESULT cobj_BoxT_try_copy(cobj_BoxT *const self,
                                           cobj_BoxT const *const v,
                                           cobj_Error *const err);

// access without transferring ownership
T const *cobj_BoxT_deref(cobj_BoxT const *const self);
T *cobj_BoxT_deref_mut(cobj_BoxT *const self);

// comparisons
bool cobj_BoxT_is_eq(cobj_BoxT const *const self, cobj_BoxT const *const b);
bool cobj_BoxT_is_ne(cobj_BoxT const *const self, cobj_BoxT const *const b);
bool cobj_BoxT_is_lt(cobj_BoxT const *const self, cobj_BoxT const *const b);
bool cobj_BoxT_is_gt(cobj_BoxT const *const self, cobj_BoxT const *const b);
bool cobj_BoxT_is_le(cobj_BoxT const *const self, cobj_BoxT const *const b);
bool cobj_BoxT_is_ge(cobj_BoxT const *const self, cobj_BoxT const *const b);

// does box contain value..
bool cobj_BoxT_contains(cobj_BoxT const *const self, T const *const v);

// create and take ownership of existing heap based object.
// no NULLs
// UB if self init.
// UB if p uninit/NULL.
void cobj_BoxT_own(cobj_BoxT *const self, T *const p);

// stop owning heap based object, user is required to manually free returned object.
// destroys self
// UB if self uninit.
T *cobj_BoxT_disown(cobj_BoxT *const self);

// swap value of self and value of other
// if self is uninit or other is uninit then uninit state is swapped as well.
// if self is init and other is uninit then after self is uninit and other is init.
// if self is uninit and other is init then after self is init and other is uninit.
void cobj_BoxT_swap(cobj_BoxT *const self, cobj_BoxT *const other);

// // cannot do fully create inplace, so do a partial.
// // nasty, as destroy will now expect an initialised block..
// // if partially created, then cannot call destroy, so call partial_free
// // create box and create heap obj for T, but don't initialise..
// // similar to stack create and heap create (mem is alloc'd but not inited)
// // so possibly only call destroy on an initialised one
// T *cobj_BoxT_elem_try_alloc(cobj_BoxT *const self, cobj_Error *const err);
// void cobj_BoxT_elem_free(cobj_BoxT *const self);

// try move T into box.. and own content..
// but as malloc can fail then use the try..
// UB if v uninit.
// UB if self uninit.
// TODO: inplace create of T instead of create then move.
bool WARN_UNUSED_RESULT cobj_BoxT_try_from_T(cobj_BoxT *const self,
                                             T *const v,
                                             cobj_Error *const err);

// move value out of box into v, destroys self (implicit destroy..)
// UB if v init.
// UB if self uninit.
void cobj_BoxT_into_T(cobj_BoxT *const self, T *const v);

// create new cobj_BoxT from copy of T..? maybe, so create new inplace.
// bool WARN_UNUSED_RESULT cobj_BoxT_try_copy_T(cobj_BoxT *const self, T const *const v, Error
// *const err); other new funcs here..

// hmm, cobj_BoxT look like unique_ptr implementation

#endif //! defined(COBJ_BOX_H)
