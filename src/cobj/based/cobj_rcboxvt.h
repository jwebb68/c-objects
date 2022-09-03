#if !defined(COBJ_RCBOXVT_H)
#    define COBJ_RCBOXVT_H

/**
 * RCBoxV<T>: object created and managed on the heap, with sharing.
 *
 * Box contents are not owned by the box and are shared.
 * Inherently Mutable; cannot move a const value into it, cannot copy into into a const box
 *
 * This version is the veneer layer for when using RCBoxV as the actual implementation.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to BoxT.
 *
 * Warning: some of the  new_ functions may prevent this being a viable pattern.
 * As I've yet to find a solution that is a single func call to RCBoxV (as the rest of them are).
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// forward declares
typedef struct RCBoxVT_ RCBoxVT;

// includes
#    include <cobj/based/cobj_rcboxv.h>
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>

// defines
struct RCBoxVT_ {
    RCBoxV inner;
};

void RCBoxVT_destroy(RCBoxVT *const self);
void RCBoxVT_move(RCBoxVT *const self, RCBoxVT *const src);
bool WARN_UNUSED_RESULT RCBoxVT_try_copy(RCBoxVT *const self,
                                         RCBoxVT const *const src,
                                         Error *const err);

T const *RCBoxVT_deref(RCBoxVT const *const self);
T *RCBoxVT_deref_mut(RCBoxVT *const self);

bool RCBoxVT_is_eq(RCBoxVT const *const self, RCBoxVT const *const b);
bool RCBoxVT_is_ne(RCBoxVT const *const self, RCBoxVT const *const b);
bool RCBoxVT_is_lt(RCBoxVT const *const self, RCBoxVT const *const b);
bool RCBoxVT_is_gt(RCBoxVT const *const self, RCBoxVT const *const b);
bool RCBoxVT_is_le(RCBoxVT const *const self, RCBoxVT const *const b);
bool RCBoxVT_is_ge(RCBoxVT const *const self, RCBoxVT const *const b);
bool RCBoxVT_contains(RCBoxVT const *const self, T const *const b);

bool WARN_UNUSED_RESULT RCBoxVT_try_from_T(RCBoxVT *const self, T *const v, Error *const err);

bool WARN_UNUSED_RESULT RCBoxVT_try_copy_T(RCBoxVT *const self, T const *const v, Error *const err);

// bool WARN_UNUSED_RESULT RCBoxVT_try_new(RCBoxVT *const self, int v, Error *const err);

// no own of a ptr: that leads to heap fragmentation
// no own of value: that's a default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.

#endif //! defined(COBJ_RCBOXVT_H)
