#if !defined(COBJ_BOX_H)
#    define COBJ_BOX_H

/**
 * Box<T>: object created and managed on the heap, without sharing.
 *
 * Box contents owned by the box and are not shared (use RCT for shared objects).
 * inherently Mutable; cannot move a const value into it, cannot copy into into a const box
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// forward declares;
typedef struct BoxT_ BoxT;

// includes
#    include "cobj_error.h"
#    include "t.h"

#    include <stdbool.h>

// defines
struct BoxT_ {
    T *elem;
};

void BoxT_destroy(BoxT *const self);

void BoxT_move(BoxT *const self, BoxT *const src);
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
// no disown function as that is an implicit destroy.
// what of null?
void BoxT_new_own(BoxT *const self, T *const p);

// create in-place and take ownership
// one new_* for each new_* on T
// developer needs to add own inplace new functions per type.
bool WARN_UNUSED_RESULT BoxT_try_new_int(BoxT *const self, int v, Error *const err);

bool WARN_UNUSED_RESULT BoxT_try_new_from_T(BoxT *const self, T *const v, Error *const err);
bool WARN_UNUSED_RESULT BoxT_try_new_copy_T(BoxT *const self, T const *const v, Error *const err);
// othe new funcs here..

// hmm, BoxT look like unique_ptr implementation
// but without the release

#endif //! defined(COBJ_BOX_H)
