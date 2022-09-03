#include <cobj/based/cobj_rcboxvt.h>
#include <cobj/core/cobj_error.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE

static void cobj_RCBoxVT_destroy_elem(void *const elem)
{
    T_destroy((T *)elem);
}

static bool
cobj_RCBoxVT_try_copy_elem(void *const elem, void const *const src, cobj_Error *const err)
{
    return T_try_copy((T *)elem, (T const *)src, err);
}

static void cobj_RCBoxVT_move_elem(void *const elem, void *const src)
{
    T_move((T *)elem, (T *)src);
}

static void cobj_RCBoxVT_wipe(cobj_RCBoxVT *const self)
{
    STRUCTWIPE(self);
}

void cobj_RCBoxVT_destroy_member(cobj_RCBoxVT *const self)
{
    cobj_RCBoxV_destroy_member(&self->inner, cobj_RCBoxVT_destroy_elem);
}

void cobj_RCBoxVT_destroy(cobj_RCBoxVT *const self)
{
    cobj_RCBoxVT_destroy_member(self);
    cobj_RCBoxVT_wipe(self);
}

void cobj_RCBoxVT_move_member(cobj_RCBoxVT *const self, cobj_RCBoxVT *const src)
{
    cobj_RCBoxV_move_member(&self->inner, &src->inner);
}
void cobj_RCBoxVT_move(cobj_RCBoxVT *const self, cobj_RCBoxVT *const src)
{
    cobj_RCBoxVT_move_member(self, src);
    cobj_RCBoxVT_wipe(self);
}

bool cobj_RCBoxVT_try_copy(cobj_RCBoxVT *const self,
                           cobj_RCBoxVT const *const src,
                           cobj_Error *const err)
{
    return cobj_RCBoxV_try_copy(&self->inner, &src->inner, err);
}

T const *cobj_RCBoxVT_deref(cobj_RCBoxVT const *const self)
{
    return (T const *)cobj_RCBoxV_deref(&self->inner);
}

T *cobj_RCBoxVT_deref_mut(cobj_RCBoxVT *const self)
{
    return cobj_RCBoxV_deref_mut(&self->inner);
}

// Can I boil this down to a single cobj_RCBoxV func?
// it's the T_new_* func sig that's the issue..
// bool WARN_UNUSED_RESULT cobj_RCBoxVT_try_new(cobj_RCBoxVT *const self, int v, cobj_Error *const
// err) {
//     cobj_RCNodeV *p = cobj_RCNodeV_malloc(sizeof(T));
//     if (p == NULL) {
//         return COBJ_ERROR_RAISE(err, cobj_ErrorCode_ENOMEM);
//     }

//     T_new(&p->value, v);
//     cobj_RCNodeV_grab(p);
//     self->inner.node = p;
//     return true;
// }

bool WARN_UNUSED_RESULT cobj_RCBoxVT_try_from_T(cobj_RCBoxVT *const self,
                                                T *const v,
                                                cobj_Error *const err)
{
    return cobj_RCBoxV_try_from_V(&self->inner, v, err, sizeof(*v), cobj_RCBoxVT_move_elem);
}

bool WARN_UNUSED_RESULT cobj_RCBoxVT_try_copy_T(cobj_RCBoxVT *const self,
                                                T const *const v,
                                                cobj_Error *const err)
{
    return cobj_RCBoxV_try_copy_V(&self->inner, v, err, sizeof(*v), cobj_RCBoxVT_try_copy_elem);
}

bool cobj_RCBoxVT_is_eq(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b)
{
    return T_is_eq(cobj_RCBoxVT_deref(self), cobj_RCBoxVT_deref(b));
}

bool cobj_RCBoxVT_is_lt(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b)
{
    return T_is_lt(cobj_RCBoxVT_deref(self), cobj_RCBoxVT_deref(b));
}

bool cobj_RCBoxVT_is_gt(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b)
{
    return T_is_gt(cobj_RCBoxVT_deref(self), cobj_RCBoxVT_deref(b));
}

bool cobj_RCBoxVT_is_le(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b)
{
    return T_is_le(cobj_RCBoxVT_deref(self), cobj_RCBoxVT_deref(b));
}

bool cobj_RCBoxVT_is_ge(cobj_RCBoxVT const *const self, cobj_RCBoxVT const *const b)
{
    return T_is_ge(cobj_RCBoxVT_deref(self), cobj_RCBoxVT_deref(b));
}

bool cobj_RCBoxVT_contains(cobj_RCBoxVT const *const self, T const *const v)
{
    return T_is_eq(cobj_RCBoxVT_deref(self), v);
}

// no own of ptr: that leads to heap fragmentation
// no own of value: that's a new_default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.
