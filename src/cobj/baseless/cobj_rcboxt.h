#if !defined(COBJ_RCBOXT_H)
#    define COBJ_RCBOXT_H

/**
 * RCBox<T>: object created and managed on the heap, with sharing.
 *
 * RCBox contents are not owned by the box and are shared using simple ref counting.
 * inherently Mutable; cannot move a const value into it, cannot copy into into a const box.
 *
 */
// because it's C there is no template mechanism, so this needs to be repeated for each type
// being treated this way.

// forward declares
typedef struct cobj_RCNodeT_s cobj_RCNodeT;
typedef struct cobj_RCBoxT_s cobj_RCBoxT;

// includes
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>

// defines
struct cobj_RCBoxT_s {
    cobj_RCNodeT *node;
};

void cobj_RCBoxT_destroy_member(cobj_RCBoxT *const self);
void cobj_RCBoxT_destroy(cobj_RCBoxT *const self);

void cobj_RCBoxT_move_member(cobj_RCBoxT *const self, cobj_RCBoxT *const src);
void cobj_RCBoxT_move(cobj_RCBoxT *const self, cobj_RCBoxT *const src);

bool WARN_UNUSED_RESULT cobj_RCBoxT_try_copy(cobj_RCBoxT *const self, cobj_RCBoxT const *const src);

T const *cobj_RCBoxT_deref(cobj_RCBoxT const *const self);
T *cobj_RCBoxT_deref_mut(cobj_RCBoxT *const self);

bool cobj_RCBoxT_is_eq(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b);
bool cobj_RCBoxT_is_lt(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b);
bool cobj_RCBoxT_is_gt(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b);
bool cobj_RCBoxT_is_le(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b);
bool cobj_RCBoxT_is_ge(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b);

bool cobj_RCBoxT_contains(cobj_RCBoxT const *const self, T const *const v);

bool WARN_UNUSED_RESULT cobj_RCBoxT_try_from_T(cobj_RCBoxT *const self,
                                               T *const v,
                                               cobj_Error *const err);

// copy variant: copy direct into dest without intermed storage?
bool WARN_UNUSED_RESULT cobj_RCBoxT_try_copy_T(cobj_RCBoxT *const self,
                                               T const *const v,
                                               cobj_Error *const err);

#    if 0
    // inplace new, but don't know various funcs on T
    bool WARN_UNUSED_RESULT cobj_RCBoxT_try_new(cobj_RCBoxT *const self, int v, cobj_Error *const err);
#    endif

// no own of a ptr: that leads to heap fragmentation
// no own of a value: that's a default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.

#endif //! defined(COBJ_RCBOXT_H)
