#if !defined(COBJ_RCBOXT_H)
#    define COBJ_RCBOXT_H

// forward declares
typedef struct RCNodeT_ RCNodeT;
typedef struct RCBoxT_ RCBoxT;

// includes
#    include "cobj_error.h"
#    include "t.h"

#    include <stdbool.h>

// defines
struct RCBoxT_ {
    RCNodeT *node;
};

void RCBoxT_destroy(RCBoxT *const self);
void RCBoxT_move(RCBoxT *const self, RCBoxT *const src);
bool WARN_UNUSED_RESULT RCBoxT_try_copy(RCBoxT *const self, RCBoxT const *const src);

T const *RCBoxT_deref(RCBoxT const *const self);
T *RCBoxT_deref_mut(RCBoxT *const self);

bool WARN_UNUSED_RESULT RCBoxT_try_new_int(RCBoxT *const self, int v, Error *const err);
bool WARN_UNUSED_RESULT RCBoxT_try_new_from_T(RCBoxT *const self, T *const v, Error *const err);

// new_copy variant? copy direct into dest without intermed storage?
bool WARN_UNUSED_RESULT RCBoxT_try_new_copy_T(RCBoxT *const self,
                                              T const *const v,
                                              Error *const err);

bool RCBoxT_is_eq(RCBoxT const *const self, RCBoxT const *const b);
bool RCBoxT_is_lt(RCBoxT const *const self, RCBoxT const *const b);
bool RCBoxT_is_gt(RCBoxT const *const self, RCBoxT const *const b);
bool RCBoxT_is_le(RCBoxT const *const self, RCBoxT const *const b);
bool RCBoxT_is_ge(RCBoxT const *const self, RCBoxT const *const b);
bool RCBoxT_contains(RCBoxT const *const self, T const *const v);

// no own of a ptr: that leads to heap fragmentation
// no own of a value: that's a new_default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.

#endif //! defined(COBJ_RCBOXT_H)
