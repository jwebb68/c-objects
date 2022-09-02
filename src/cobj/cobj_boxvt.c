#include "cobj_boxvt.h"

#include "cobj_memory.h" // STRUCTWIPE

static void BoxVT_elem_free(void *const self)
{
    free(self);
}

static void *BoxVT_elem_try_alloc(Error *const err)
{
    return try_malloc(sizeof(T), err);
}

static void BoxVT_elem_destroy(void *const self)
{
    T_destroy((T *)self);
}

static void BoxVT_elem_move(void *const elem, void *const src)
{
    T_move((T *)elem, (T *)src);
}

static bool BoxVT_elem_try_copy(void *const self, void const *src, Error *const err)
{
    return T_try_copy((T *)self, (T const *)src, err);
}

//###############################################################

static void BoxVT_wipe(BoxVT *const self)
{
    STRUCTWIPE(self);
}

void BoxVT_destroy_member(BoxVT *const self)
{
    BoxV_destroy_member(&self->inner, BoxVT_elem_destroy, BoxVT_elem_free);
}

void BoxVT_destroy(BoxVT *const self)
{
    BoxVT_destroy_member(self);
    BoxVT_wipe(self);
}

void BoxVT_move_member(BoxVT *const self, BoxVT *const src)
{
    *self = *src;
}

void BoxVT_move(BoxVT *const self, BoxVT *const src)
{
    BoxVT_move_member(self, src);
    BoxVT_wipe(src);
}

bool WARN_UNUSED_RESULT BoxVT_try_copy_member(BoxVT *const self,
                                              BoxVT const *const v,
                                              Error *const err)
{
    return BoxV_try_copy_member(&self->inner,
                                &v->inner,
                                err,
                                BoxVT_elem_try_alloc,
                                BoxVT_elem_free,
                                BoxVT_elem_try_copy);
}

bool WARN_UNUSED_RESULT BoxVT_try_copy(BoxVT *const self, BoxVT const *const v, Error *const err)
{
    if (!BoxVT_try_copy_member(self, v, err)) { BoxVT_wipe(self); }
    return true;
}

T const *BoxVT_deref(BoxVT const *const self)

{
    return (T const *)BoxV_deref(&self->inner);
}

T *BoxVT_deref_mut(BoxVT *const self)
{
    return (T *)BoxV_deref_mut(&self->inner);
}

bool BoxVT_is_eq(BoxVT const *const self, BoxVT const *const b)
{
    return T_is_eq(BoxVT_deref(self), BoxVT_deref(b));
}

bool BoxVT_is_ne(BoxVT const *const self, BoxVT const *const b)
{
    return T_is_ne(BoxVT_deref(self), BoxVT_deref(b));
}

bool BoxVT_is_lt(BoxVT const *const self, BoxVT const *const b)
{
    return T_is_lt(BoxVT_deref(self), BoxVT_deref(b));
}

bool BoxVT_is_gt(BoxVT const *const self, BoxVT const *const b)
{
    return T_is_gt(BoxVT_deref(self), BoxVT_deref(b));
}

bool BoxVT_is_le(BoxVT const *const self, BoxVT const *const b)
{
    return T_is_le(BoxVT_deref(self), BoxVT_deref(b));
}

bool BoxVT_is_ge(BoxVT const *const self, BoxVT const *const b)
{
    return T_is_ge(BoxVT_deref(self), BoxVT_deref(b));
}

bool BoxVT_contains(BoxVT const *const self, T const *const v)
{
    return T_is_eq(BoxVT_deref(self), v);
}

void BoxVT_own(BoxVT *const self, T *const p)
{
    BoxV_own(&self->inner, p);
}

T *BoxVT_disown(BoxVT *const self)
{
    T *const p = BoxV_disown_member(&self->inner);
    BoxVT_wipe(self);
    return p;
}

void BoxVT_swap(BoxVT *const self, BoxVT *const other)
{
    BoxV_swap(&self->inner, &other->inner);
}

// void BoxVT_new...(BoxVT_ *const self, ...) {
//  T_new(&self->elem, ...);
//}

bool WARN_UNUSED_RESULT BoxVT_try_from_T(BoxVT *const self, T *const v, Error *const err)
{
    return BoxV_try_from_V(&self->inner, v, err, BoxVT_elem_try_alloc, BoxVT_elem_move);
}

bool WARN_UNUSED_RESULT BoxVT_try_copy_T(BoxVT *const self, T const *const v, Error *const err)
{
    return BoxV_try_copy_V(&self->inner,
                           v,
                           err,
                           BoxVT_elem_try_alloc,
                           BoxVT_elem_free,
                           BoxVT_elem_try_copy);
}
