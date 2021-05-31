#if !defined(COBJ_RCBOXV_H)
#    define COBJ_RCBOXV_H

// forward declares
typedef struct RCNodeV_ RCNodeV;
typedef struct RCBoxV_ RCBoxV;

// includes
#    include "cobj_error.h"
#    include "t.h"

#    include <stdbool.h>

// defines
struct RCBoxV_ {
    RCNodeV *node;
};

void RCBoxV_destroy(RCBoxV *const self, void (*elem_destroy)(void *const));
void RCBoxV_move(RCBoxV *const self, RCBoxV *const src);
bool RCBoxV_try_copy(RCBoxV *const self, RCBoxV const *const src, Error *const err);

void const *RCBoxV_deref(RCBoxV const *const self);
void *RCBoxV_deref_mut(RCBoxV *const self);

bool WARN_UNUSED_RESULT RCBoxV_try_new_from(RCBoxV *const self,
                                            void *const v,
                                            Error *const err,
                                            size_t elem_size,
                                            void (*elem_move)(void *const self, void *const src));

bool WARN_UNUSED_RESULT RCBoxV_try_new_copy(RCBoxV *const self,
                                            T const *const v,
                                            Error *const err,
                                            size_t elem_size,
                                            bool (*elem_try_copy)(void *const elem,
                                                                  void const *const src,
                                                                  Error *const err));

// // new_copy variant? copy direct into dest without intermed storage?
// bool WARN_UNUSED_RESULT RCBoxV_try_new_copy_T(RCBoxV *const self, T const *const v, Error *const
// err);

// bool RCBoxV_is_eq(RCBoxV const *const self, RCBoxV const *const b);
// bool RCBoxV_is_lt(RCBoxV const *const self, RCBoxV const *const b);
// bool RCBoxV_is_gt(RCBoxV const *const self, RCBoxV const *const b);

// no own of a ptr: that leads to heap fragmentation
// no own of value: that's a new_default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.

#endif //! defined(COBJ_RCBOXV_H)
