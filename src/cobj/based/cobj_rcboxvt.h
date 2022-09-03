#if !defined(COBJ_RCBOXVT_H)
#    define COBJ_RCBOXVT_H

/**
 * cobj_RCBoxV<T>: object created and managed on the heap, with sharing.
 *
 * Box contents are not owned by the box and are shared.
 * Inherently Mutable; cannot move a const value into it, cannot copy into into a const box
 *
 * This version is the veneer layer for when using cobj_RCBoxV as the actual implementation.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to cobj_BoxT.
 *
 * Warning: some of the  new_ functions may prevent this being a viable pattern.
 * As I've yet to find a solution that is a single func call to cobj_RCBoxV (as the rest of them
 * are).
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// forward declares
typedef struct cobj_RCBoxVT_s cobj_RCBoxVT;

// includes
#    include <cobj/based/cobj_rcboxv.h>
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>

// defines
struct cobj_RCBoxVT_s {
    cobj_RCBoxV inner;
};

void cobj_RCBoxVT_destroy(cobj_RCBoxVT *const self);
void cobj_RCBoxVT_move(cobj_RCBoxVT *const self, cobj_RCBoxVT *const src);
bool WARN_UNUSED_RESULT cobj_RCBoxVT_try_copy(cobj_RCBoxVT *const self,
                                              cobj_RCBoxVT const *const src,
                                              cobj_Error *const err);

T const *cobj_RCBoxVT_deref(cobj_RCBoxVT const *const self);
T *cobj_RCBoxVT_deref_mut(cobj_RCBoxVT *const self);

bool cobj_RCBoxVT_is_eq(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b);
bool cobj_RCBoxVT_is_ne(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b);
bool cobj_RCBoxVT_is_lt(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b);
bool cobj_RCBoxVT_is_gt(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b);
bool cobj_RCBoxVT_is_le(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b);
bool cobj_RCBoxVT_is_ge(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b);
bool cobj_RCBoxVT_contains(cobj_RCBoxVT const *const self, T const *const b);

bool WARN_UNUSED_RESULT cobj_RCBoxVT_try_from_T(cobj_RCBoxVT *const self,
                                                T *const v,
                                                cobj_Error *const err);

bool WARN_UNUSED_RESULT cobj_RCBoxVT_try_copy_T(cobj_RCBoxVT *const self,
                                                T const *const v,
                                                cobj_Error *const err);

// bool WARN_UNUSED_RESULT cobj_RCBoxVT_try_new(cobj_RCBoxVT *const self, int v, cobj_Error *const
// err);

// no own of a ptr: that leads to heap fragmentation
// no own of value: that's a default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.

#endif //! defined(COBJ_RCBOXVT_H)
