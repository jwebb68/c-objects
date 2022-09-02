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
typedef struct BoxT_s BoxT;

// includes
#    include "cobj_error.h"
#    include "t.h"

#    include <stdbool.h>

// defines
struct BoxT_s {
    T *elem;
};

void BoxT_destroy_member(BoxT *const self);
void BoxT_destroy(BoxT *const self);

void BoxT_move(BoxT *const self, BoxT *const src);

bool WARN_UNUSED_RESULT BoxT_try_copy_member(BoxT *const self,
                                             BoxT const *const v,
                                             Error *const err);
bool WARN_UNUSED_RESULT BoxT_try_copy(BoxT *const self, BoxT const *const v, Error *const err);

// access without transferring ownership
T const *BoxT_deref(BoxT const *const self);
T *BoxT_deref_mut(BoxT *const self);

// comparisons
bool BoxT_is_eq(BoxT const *const self, BoxT const *const b);
bool BoxT_is_ne(BoxT const *const self, BoxT const *const b);
bool BoxT_is_lt(BoxT const *const self, BoxT const *const b);
bool BoxT_is_gt(BoxT const *const self, BoxT const *const b);
bool BoxT_is_le(BoxT const *const self, BoxT const *const b);
bool BoxT_is_ge(BoxT const *const self, BoxT const *const b);

// does box contain value..
bool BoxT_contains(BoxT const *const self, T const *const v);

// create and take ownership of existing heap based object.
// no NULLs
// UB if self init.
// UB if p uninit/NULL.
void BoxT_own(BoxT *const self, T *const p);

// stop owning heap based object, user is required to manually free returned object.
// destroys self
// UB if self uninit.
T *BoxT_disown(BoxT *const self);

// swap value of self and value of other
// if self is uninit or other is uninit then uninit state is swapped as well.
// if self is init and other is uninit then after self is uninit and other is init.
// if self is uninit and other is init then after self is init and other is uninit.
void BoxT_swap(BoxT *const self, BoxT *const other);

// // cannot do fully create inplace, so do a partial.
// // nasty, as destroy will now expect an initialised block..
// // if partially created, then cannot call destroy, so call partial_free
// // create box and create heap obj for T, but don't initialise..
// // similar to stack create and heap create (mem is alloc'd but not inited)
// // so possibly only call destroy on an initialised one
// T *BoxT_elem_try_alloc(BoxT *const self, Error *const err);
// void BoxT_elem_free(BoxT *const self);

// try move T into box.. and own content..
// but as malloc can fail then use the try..
// UB if v uninit.
// UB if self uninit.
// TODO: inplace create of T instead of create then move.
bool WARN_UNUSED_RESULT BoxT_try_from_T(BoxT *const self, T *const v, Error *const err);

// move value out of box into v, destroys self (implicit destroy..)
// UB if v init.
// UB if self uninit.
void BoxT_into_T(BoxT *const self, T *const v);

// create new boxt from copy of T..? maybe, so create new inplace.
// bool WARN_UNUSED_RESULT BoxT_try_copy_T(BoxT *const self, T const *const v, Error *const err);
// other new funcs here..

// hmm, BoxT look like unique_ptr implementation

#endif //! defined(COBJ_BOX_H)
