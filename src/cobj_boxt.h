#if !defined(COBJ_BOX_H)
#define COBJ_BOX_H

// Box, object created and managed on the heap
// and not shared (use RCT for shared objects).

// forward declares;
typedef struct BoxT_ BoxT;
typedef struct BoxTMut_ BoxTMut;

//includes
#include "t.h"
#include "cobj_error.h"

#include <stdbool.h>

// defines
struct BoxT_ {
    T const *elem;
};
void BoxT_destroy(BoxT *const self);
void BoxT_move(BoxT *const self, BoxT *const src);
T const *BoxT_deref(BoxT const *const self);
bool BoxT_is_none(BoxT const *const self);
bool BoxT_is_eq(BoxT const *const self, BoxT const *const b);
bool BoxT_is_lt(BoxT const *const self, BoxT const *const b);
bool BoxT_is_gt(BoxT const *const self, BoxT const *const b);

// create and take ownership of existing heap based object.
// no disown as that is an implicit destroy.
void BoxT_new_own(BoxT *const self, T const *const p);

// create in-place and take ownership
// one new_* for each new_* on T
bool BoxT_try_new_int(BoxT *const self, int v, Error *err);
bool BoxT_try_new_from_T(BoxT *const self, T *const v, Error *err);
bool BoxT_try_new_copy_T(BoxT *const self, T const *const v, Error *err);


struct BoxTMut_ {
    T *elem;
};

void BoxTMut_destroy(BoxTMut *const self);
void BoxTMut_move(BoxTMut *const self, BoxTMut *const src);
T *BoxTMut_deref_mut(BoxTMut const *const self);
T const *BoxTMut_deref(BoxTMut const *const self);
bool BoxTMut_is_none(BoxTMut const *const self);
bool BoxTMut_is_eq(BoxTMut const *const self, BoxTMut const *const b);
bool BoxTMut_is_lt(BoxTMut const *const self, BoxTMut const *const b);
bool BoxTMut_is_gt(BoxTMut const *const self, BoxTMut const *const b);

// create and take ownsership of existing heap obj
// no disown as that is an implicit destroy.
void BoxTMut_new_own(BoxTMut *const self, T *const p);

// create in-place and take ownership
// one new_ for each new_ on T
bool BoxTMut_try_new_int(BoxTMut *const self, int v, Error *const err);

// create and take ownership of existing T
bool BoxTMut_try_new_from_T(BoxTMut *const self, T *const v, Error *const err);

// create and take ownership of a copy of existing T
// direct into dest.
bool BoxTMut_try_new_copy_T(BoxTMut *const self, T const *const v, Error *const err);

// hmm, BoxT and BoxTMut look like unique_ptr implementations
// but without the release


#endif//!defined(COBJ_BOX_H)
