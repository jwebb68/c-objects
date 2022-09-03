#if !defined(COBJ_RCBOXV_H)
#    define COBJ_RCBOXV_H

/**
 * RCBoxV: object created and managed on the heap, with sharing.
 *
 * RCBox contents are not owned by the box and are shared.
 * Inherently Mutable; cannot move a const value into it, cannot copy into into a const box.
 *
 * This version is the underlying implementation for all RCBoxV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to BoxT.
 */

// forward declares
typedef struct RCNodeV_ RCNodeV;
typedef struct RCBoxV_ RCBoxV;

// includes
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>

// defines
struct RCBoxV_ {
    RCNodeV *node;
};

void RCBoxV_destroy_member(RCBoxV *const self, void (*elem_destroy)(void *const));
void RCBoxV_destroy(RCBoxV *const self, void (*elem_destroy)(void *const));

void RCBoxV_move_member(RCBoxV *const self, RCBoxV *const src);
void RCBoxV_move(RCBoxV *const self, RCBoxV *const src);

bool RCBoxV_try_copy(RCBoxV *const self, RCBoxV const *const src, Error *const err);

void const *RCBoxV_deref(RCBoxV const *const self);
void *RCBoxV_deref_mut(RCBoxV *const self);

bool WARN_UNUSED_RESULT RCBoxV_try_from_V(RCBoxV *const self,
                                          void *const v,
                                          Error *const err,
                                          size_t elem_size,
                                          void (*elem_move)(void *const self, void *const src));

bool WARN_UNUSED_RESULT RCBoxV_try_copy_V(RCBoxV *const self,
                                          void const *const v,
                                          Error *const err,
                                          size_t elem_size,
                                          bool (*elem_try_copy)(void *const elem,
                                                                void const *const src,
                                                                Error *const err));

// no own of a ptr: that leads to heap fragmentation
// no own of value: that's a default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.

#endif //! defined(COBJ_RCBOXV_H)
