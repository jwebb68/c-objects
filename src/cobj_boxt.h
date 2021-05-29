#if !defined(COBJ_BOX_H)
#    define COBJ_BOX_H

// Box, object created and managed on the heap
// and not shared (use RCT for shared objects).
// inherently a Mut.

// forward declares;
typedef struct BoxT_ BoxT;
// typedef struct BoxTMut_ BoxTMut;

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
bool WARN_UNUSED_RESULT BoxT_try_copy(BoxT *const self, BoxT const *const v, Error *err);

// create in-place and take ownership
// one new_* for each new_* on T
bool WARN_UNUSED_RESULT BoxT_try_new_int(BoxT *const self, int v, Error *err);
bool WARN_UNUSED_RESULT BoxT_try_new_from_T(BoxT *const self, T *const v, Error *err);
bool WARN_UNUSED_RESULT BoxT_try_new_copy_T(BoxT *const self, T const *const v, Error *err);
// othe new funcs here..

// access without transferring ownership
T const *BoxT_deref(BoxT const *const self);
T *BoxT_deref_mut(BoxT *const self);

// comparisons
bool BoxT_is_eq(BoxT const *const self, BoxT const *const b);
bool BoxT_is_ne(BoxT const *const self, BoxT const *const b);
bool BoxT_is_lt(BoxT const *const self, BoxT const *const b);
bool BoxT_is_gt(BoxT const *const self, BoxT const *const b);

// create and take ownership of existing heap based object.
// no disown function as that is an implicit destroy.
// what of null?
void BoxT_new_own(BoxT *const self, T *const p);

// can box ever be null?

// hmm, BoxT look like unique_ptr implementation
// but without the release

#endif //! defined(COBJ_BOX_H)
