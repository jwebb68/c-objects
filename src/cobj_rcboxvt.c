#include "cobj_rcboxvt.h"

#include "cobj_error.h"
#include "cobj_memory.h" // STRUCTWIPE

static void RCBoxVT_destroy_elem(void *const elem)
{
    T_destroy((T *)elem);
}

static bool RCBoxVT_try_copy_elem(void *const elem, void const *const src, Error *const err)
{
    return T_try_copy((T *)elem, (T const *)src, err);
}

// static void RCBoxVT_wipe(RCBoxVT *const self) {
//     STRUCTWIPE(self);
// }

void RCBoxVT_destroy(RCBoxVT *const self)
{
    RCBoxV_destroy(&self->inner, RCBoxVT_destroy_elem);
}

void RCBoxVT_move(RCBoxVT *const self, RCBoxVT *const src)
{
    RCBoxV_move(&self->inner, &src->inner);
    // anything else to do here?
}

bool RCBoxVT_try_copy(RCBoxVT *const self, RCBoxVT const *const src, Error *const err)
{
    return RCBoxV_try_copy(&self->inner, &src->inner, err);
}

T const *RCBoxVT_deref(RCBoxVT const *const self)
{
    return (T const *)RCBoxV_deref(&self->inner);
}

T *RCBoxVT_deref_mut(RCBoxVT *const self)
{
    return RCBoxV_deref_mut(&self->inner);
}

// bool WARN_UNUSED_RESULT RCBoxT_try_new_int(RCBoxT *const self, int v, Error *const err) {
//     RCNodeT *p = RCNodeT_malloc();
//     if (p == NULL) {
//         return ERROR_RAISE(err, Error_ENOMEM);
//     }

//     RCNodeT_new_int(p, v);
//     RCNodeT_grab(p);
//     self->node = p;
//     return true;
// }

bool WARN_UNUSED_RESULT RCBoxVT_try_new_from_T(RCBoxVT *const self, T *const v, Error *const err)
{
    return RCBoxV_try_new_from(&self->inner, v, err, sizeof(*v), RCBoxVT_try_copy_elem);
}

// // new_copy variant? copy direct into dest without intermed storage?
// bool WARN_UNUSED_RESULT RCBoxT_try_new_copy_T(RCBoxT *const self, T const *const v, Error *const
// err) {
//     RCNodeT *p = RCNodeT_malloc();
//     if (p == NULL) {
//         return ERROR_RAISE(err, Error_ENOMEM);
//     }

//     bool ok;
//     ok = T_try_copy(RCNodeT_deref_mut(p), v, err);
//     if (!ok) {
//         // no destroy as node is not initialised and destroy wants it
//         // initialised.
//         free(p);
//         return false;
//     }

//     RCNodeT_grab(p);
//     self->node = p;
//     return true;
// }

bool RCBoxVT_is_eq(RCBoxVT const *const self, RCBoxVT const *const b)
{
    return T_is_eq(RCBoxVT_deref(self), RCBoxVT_deref(b));
}

bool RCBoxVT_is_lt(RCBoxVT const *const self, RCBoxVT const *const b)
{
    return T_is_lt(RCBoxVT_deref(self), RCBoxVT_deref(b));
}

bool RCBoxVT_is_gt(RCBoxVT const *const self, RCBoxVT const *const b)
{
    return T_is_gt(RCBoxVT_deref(self), RCBoxVT_deref(b));
}

// no own of ptr: that leads to heap fragmentation
// no own of value: that's a new_default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.
