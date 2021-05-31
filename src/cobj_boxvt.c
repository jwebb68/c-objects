#include "cobj_boxvt.h"

#include "cobj_memory.h" // STRUCTWIPE

static void BoxVT_destroy_elem(void *const self)
{
    T_destroy((T *)self);
}

static void BoxVT_move_elem(void *const elem, void *const src)
{
    T_move((T *)elem, (T *)src);
}

static bool BoxVT_try_copy_elem(void *const self, void const *src, Error *const err)
{
    return T_try_copy((T *)self, (T const *)src, err);
}

static void BoxVT_wipe(BoxVT *const self)
{
    STRUCTWIPE(self);
}

void BoxVT_destroy(BoxVT *const self)
{
    BoxV_destroy(&self->inner, BoxVT_destroy_elem);
    // is wipe needed if only one member that wipes itself?
}

void BoxVT_move(BoxVT *const self, BoxVT *const src)
{
    *self = *src;
    BoxVT_wipe(src);
}

bool WARN_UNUSED_RESULT BoxVT_try_copy(BoxVT *const self, BoxVT const *const v, Error *const err)
{
    return BoxV_try_copy(&self->inner, &v->inner, err, sizeof(T), BoxVT_try_copy_elem);
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

void BoxVT_new_own(BoxVT *const self, T *const p)
{
    BoxV_new_own(&self->inner, p);
}

// void BoxVT_new_(BoxVT_ *const self, ...) {
//  T_new(&self->elem, ...);
//}

bool WARN_UNUSED_RESULT BoxVT_try_new_from_T(BoxVT *const self, T *const v, Error *const err)
{
    return BoxV_try_new_from(&self->inner, v, err, sizeof(T), BoxVT_move_elem);
}

bool WARN_UNUSED_RESULT BoxVT_try_new_copy_T(BoxVT *const self, T const *const v, Error *const err)
{
    return BoxV_try_new_copy(&self->inner, v, err, sizeof(T), BoxVT_try_copy_elem);
}
