#if !defined(COBJ_RCBOXV_H)
#    define COBJ_RCBOXV_H

/**
 * cobj_RCBoxV: object created and managed on the heap, with sharing.
 *
 * RCBox contents are not owned by the box and are shared.
 * Inherently Mutable; cannot move a const value into it, cannot copy into into a const box.
 *
 * This version is the underlying implementation for all cobj_RCBoxV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to cobj_BoxT.
 */

// forward declares
typedef struct cobj_RCNodeV_s cobj_RCNodeV;
typedef struct cobj_RCBoxV_s cobj_RCBoxV;

// includes
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>

// defines
struct cobj_RCBoxV_s {
    cobj_RCNodeV *node;
};

void cobj_RCBoxV_destroy_member(cobj_RCBoxV *const self, void (*elem_destroy)(void *const));
void cobj_RCBoxV_destroy(cobj_RCBoxV *const self, void (*elem_destroy)(void *const));

void cobj_RCBoxV_move_member(cobj_RCBoxV *const self, cobj_RCBoxV *const src);
void cobj_RCBoxV_move(cobj_RCBoxV *const self, cobj_RCBoxV *const src);

bool cobj_RCBoxV_try_copy(cobj_RCBoxV *const self,
                          cobj_RCBoxV const *const src,
                          cobj_Error *const err);

void const *cobj_RCBoxV_deref(cobj_RCBoxV const *const self);
void *cobj_RCBoxV_deref_mut(cobj_RCBoxV *const self);

bool WARN_UNUSED_RESULT cobj_RCBoxV_try_from_V(cobj_RCBoxV *const self,
                                               void *const v,
                                               cobj_Error *const err,
                                               size_t elem_size,
                                               void (*elem_move)(void *const self,
                                                                 void *const src));

bool WARN_UNUSED_RESULT cobj_RCBoxV_try_copy_V(cobj_RCBoxV *const self,
                                               void const *const v,
                                               cobj_Error *const err,
                                               size_t elem_size,
                                               bool (*elem_try_copy)(void *const elem,
                                                                     void const *const src,
                                                                     cobj_Error *const err));

// no own of a ptr: that leads to heap fragmentation
// no own of value: that's a default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.

#endif //! defined(COBJ_RCBOXV_H)
