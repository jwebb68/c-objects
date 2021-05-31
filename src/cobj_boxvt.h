#if !defined(COBJ_BOXVT_H)
#    define COBJ_BOXVT_H

/**
 * BoxV<T>: object created and managed on the heap, without sharing.
 *
 * Box contents owned by the box and are not shared (use RCT for shared objects).
 * inherently a Mut; cannot move a const value into it, cannot copy into into a const box.
 *
 * This version is the veneer layer for when using BoxV as the actual implementation.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to BoxT.
 *
 * Warning: some of the  new_ functions may prevent this being a viable pattern.
 * As I've yet to find a solution that is a single func call to BoxV (as the rest of them are).
 */

// forward declares
typedef struct BoxVT_ BoxVT;

// includes
#    include "cobj_boxv.h"
#    include "cobj_error.h"
#    include "t.h"

#    include <stdbool.h>
#    include <stddef.h> // size_t

// defines
struct BoxVT_ {
    BoxV inner;
};

void BoxVT_destroy(BoxVT *const self);
void BoxVT_move(BoxVT *const self, BoxVT *const src);
bool WARN_UNUSED_RESULT BoxVT_try_copy(BoxVT *const self, BoxVT const *const v, Error *const err);

T const *BoxVT_deref(BoxVT const *const self);
T *BoxVT_deref_mut(BoxVT *const self);

// comparisons
bool BoxVT_is_eq(BoxVT const *const self, BoxVT const *const b);
bool BoxVT_is_ne(BoxVT const *const self, BoxVT const *const b);
bool BoxVT_is_lt(BoxVT const *const self, BoxVT const *const b);
bool BoxVT_is_gt(BoxVT const *const self, BoxVT const *const b);
bool BoxVT_is_le(BoxVT const *const self, BoxVT const *const b);
bool BoxVT_is_ge(BoxVT const *const self, BoxVT const *const b);

// does box contain value..
bool BoxVT_contains(BoxVT const *const self, T const *const v);

void BoxVT_new_own(BoxVT *const self, T *const p);

bool WARN_UNUSED_RESULT BoxVT_try_new_from_T(BoxVT *const self, T *const v, Error *const err);
bool WARN_UNUSED_RESULT BoxVT_try_new_copy_T(BoxVT *const self, T const *const v, Error *const err);

// developer needs to add own inplace new functions per type.
// void BoxVT_new_(BoxVT_ *const self, ...);

#endif //! defined(COBJ_BOXVT_H)
