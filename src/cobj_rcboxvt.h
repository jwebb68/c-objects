#if !defined(COBJ_RCBOXVT_H)
#    define COBJ_RCBOXVT_H

// forward declares
typedef struct RCBoxVT_ RCBoxVT;

// includes
#    include "cobj_error.h"
#    include "cobj_rcboxv.h"
#    include "t.h"

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

// bool WARN_UNUSED_RESULT RCBoxVT_try_new_int(RCBoxVT *const self, int v, Error *const err);
bool WARN_UNUSED_RESULT RCBoxVT_try_new_from_T(RCBoxVT *const self, T *const v, Error *const err);

// // new_copy variant? copy direct into dest without intermed storage?
// bool WARN_UNUSED_RESULT RCBoxVT_try_new_copy_T(RCBoxVT *const self, T const *const v, Error
// *const err);

bool RCBoxVT_is_eq(RCBoxVT const *const self, RCBoxVT const *const b);
bool RCBoxVT_is_lt(RCBoxVT const *const self, RCBoxVT const *const b);
bool RCBoxVT_is_gt(RCBoxVT const *const self, RCBoxVT const *const b);

// no own of a ptr: that leads to heap fragmentation
// no own of value: that's a new_default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.

#endif //! defined(COBJ_RCBOXVT_H)
