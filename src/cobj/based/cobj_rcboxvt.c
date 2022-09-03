#include <cobj/based/cobj_rcboxvt.h>
#include <cobj/core/cobj_error.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE

static void RCBoxVT_destroy_elem(void *const elem)
{
    T_destroy((T *)elem);
}

static bool RCBoxVT_try_copy_elem(void *const elem, void const *const src, Error *const err)
{
    return T_try_copy((T *)elem, (T const *)src, err);
}

static void RCBoxVT_move_elem(void *const elem, void *const src)
{
    T_move((T *)elem, (T *)src);
}

static void RCBoxVT_wipe(RCBoxVT *const self)
{
    STRUCTWIPE(self);
}

void RCBoxVT_destroy_member(RCBoxVT *const self)
{
    RCBoxV_destroy_member(&self->inner, RCBoxVT_destroy_elem);
}

void RCBoxVT_destroy(RCBoxVT *const self)
{
    RCBoxVT_destroy_member(self);
    RCBoxVT_wipe(self);
}

void RCBoxVT_move_member(RCBoxVT *const self, RCBoxVT *const src)
{
    RCBoxV_move_member(&self->inner, &src->inner);
}
void RCBoxVT_move(RCBoxVT *const self, RCBoxVT *const src)
{
    RCBoxVT_move_member(self, src);
    RCBoxVT_wipe(self);
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

// Can I boil this down to a single RCBoxV func?
// it's the T_new_* func sig that's the issue..
// bool WARN_UNUSED_RESULT RCBoxVT_try_new(RCBoxVT *const self, int v, Error *const err) {
//     RCNodeV *p = RCNodeV_malloc(sizeof(T));
//     if (p == NULL) {
//         return ERROR_RAISE(err, Error_ENOMEM);
//     }

//     T_new(&p->value, v);
//     RCNodeV_grab(p);
//     self->inner.node = p;
//     return true;
// }

bool WARN_UNUSED_RESULT RCBoxVT_try_from_T(RCBoxVT *const self, T *const v, Error *const err)
{
    return RCBoxV_try_from_V(&self->inner, v, err, sizeof(*v), RCBoxVT_move_elem);
}

bool WARN_UNUSED_RESULT RCBoxVT_try_copy_T(RCBoxVT *const self, T const *const v, Error *const err)
{
    return RCBoxV_try_copy_V(&self->inner, v, err, sizeof(*v), RCBoxVT_try_copy_elem);
}

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

bool RCBoxVT_is_le(RCBoxVT const *const self, RCBoxVT const *const b)
{
    return T_is_le(RCBoxVT_deref(self), RCBoxVT_deref(b));
}

bool RCBoxVT_is_ge(RCBoxVT const *const self, RCBoxVT const *const b)
{
    return T_is_ge(RCBoxVT_deref(self), RCBoxVT_deref(b));
}

bool RCBoxVT_contains(RCBoxVT const *const self, T const *const v)
{
    return T_is_eq(RCBoxVT_deref(self), v);
}

// no own of ptr: that leads to heap fragmentation
// no own of value: that's a new_default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.
