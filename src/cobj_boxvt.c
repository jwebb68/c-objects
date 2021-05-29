#include "cobj_boxvt.h"

static void BoxVT_destroy_elem(void *const self)
{
    T_destroy((T *)self);
}

static void BoxVT_move_elem(void *const elem, void *const src)
{
    T_move((T *)elem, (T *)src);
}

static bool BoxVT_try_copy_elem(void *self, void const *src, Error *err)
{
    return T_try_copy((T *)self, (T const *)src, err);
}

void BoxVT_destroy(BoxVT *const self)
{
    BoxV_destroy(&self->inner, BoxVT_destroy_elem);
}

void BoxVT_move(BoxVT *const self, BoxVT *const src)
{
    BoxV_move(&self->inner, &src->inner);
}

bool WARN_UNUSED_RESULT BoxVT_try_copy(BoxVT *const self, BoxVT const *const v, Error *err)
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

void BoxVT_new_own(BoxVT *const self, T *const p)
{
    BoxV_new_own(&self->inner, p);
}

// void BoxVT_new_(BoxVT_ *const self, ...) {
//  T_new(&self->elem, ...);
//}

bool WARN_UNUSED_RESULT BoxVT_try_new_from_T(BoxVT *const self, T *const v, Error *err)
{
    return BoxV_try_new_from(&self->inner, v, err, sizeof(T), BoxVT_move_elem);
}
