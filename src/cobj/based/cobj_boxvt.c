#include <cobj/based/cobj_boxvt.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE

static void cobj_BoxVT_elem_free(void *const self)
{
    free(self);
}

static void *cobj_BoxVT_elem_try_alloc(cobj_Error *const err)
{
    return cobj_try_malloc(sizeof(T), err);
}

static void cobj_BoxVT_elem_destroy(void *const self)
{
    T_destroy((T *)self);
}

static void cobj_BoxVT_elem_move(void *const elem, void *const src)
{
    T_move((T *)elem, (T *)src);
}

static bool cobj_BoxVT_elem_try_copy(void *const self, void const *src, cobj_Error *const err)
{
    return T_try_copy((T *)self, (T const *)src, err);
}

//###############################################################

static void cobj_BoxVT_wipe(cobj_BoxVT *const self)
{
    STRUCTWIPE(self);
}

void cobj_BoxVT_destroy_member(cobj_BoxVT *const self)
{
    cobj_BoxV_destroy_member(&self->inner, cobj_BoxVT_elem_destroy, cobj_BoxVT_elem_free);
}

void cobj_BoxVT_destroy(cobj_BoxVT *const self)
{
    cobj_BoxVT_destroy_member(self);
    cobj_BoxVT_wipe(self);
}

void cobj_BoxVT_move_member(cobj_BoxVT *const self, cobj_BoxVT *const src)
{
    *self = *src;
}

void cobj_BoxVT_move(cobj_BoxVT *const self, cobj_BoxVT *const src)
{
    cobj_BoxVT_move_member(self, src);
    cobj_BoxVT_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_BoxVT_try_copy_member(cobj_BoxVT *const self,
                                                   cobj_BoxVT const *const v,
                                                   cobj_Error *const err)
{
    return cobj_BoxV_try_copy_member(&self->inner,
                                     &v->inner,
                                     err,
                                     cobj_BoxVT_elem_try_alloc,
                                     cobj_BoxVT_elem_free,
                                     cobj_BoxVT_elem_try_copy);
}

bool WARN_UNUSED_RESULT cobj_BoxVT_try_copy(cobj_BoxVT *const self,
                                            cobj_BoxVT const *const v,
                                            cobj_Error *const err)
{
    if (!cobj_BoxVT_try_copy_member(self, v, err)) { cobj_BoxVT_wipe(self); }
    return true;
}

T const *cobj_BoxVT_deref(cobj_BoxVT const *const self)

{
    return (T const *)cobj_BoxV_deref(&self->inner);
}

T *cobj_BoxVT_deref_mut(cobj_BoxVT *const self)
{
    return (T *)cobj_BoxV_deref_mut(&self->inner);
}

bool cobj_BoxVT_is_eq(cobj_BoxVT const *const self, cobj_BoxVT const *const b)
{
    return T_is_eq(cobj_BoxVT_deref(self), cobj_BoxVT_deref(b));
}

bool cobj_BoxVT_is_ne(cobj_BoxVT const *const self, cobj_BoxVT const *const b)
{
    return T_is_ne(cobj_BoxVT_deref(self), cobj_BoxVT_deref(b));
}

bool cobj_BoxVT_is_lt(cobj_BoxVT const *const self, cobj_BoxVT const *const b)
{
    return T_is_lt(cobj_BoxVT_deref(self), cobj_BoxVT_deref(b));
}

bool cobj_BoxVT_is_gt(cobj_BoxVT const *const self, cobj_BoxVT const *const b)
{
    return T_is_gt(cobj_BoxVT_deref(self), cobj_BoxVT_deref(b));
}

bool cobj_BoxVT_is_le(cobj_BoxVT const *const self, cobj_BoxVT const *const b)
{
    return T_is_le(cobj_BoxVT_deref(self), cobj_BoxVT_deref(b));
}

bool cobj_BoxVT_is_ge(cobj_BoxVT const *const self, cobj_BoxVT const *const b)
{
    return T_is_ge(cobj_BoxVT_deref(self), cobj_BoxVT_deref(b));
}

bool cobj_BoxVT_contains(cobj_BoxVT const *const self, T const *const v)
{
    return T_is_eq(cobj_BoxVT_deref(self), v);
}

void cobj_BoxVT_own(cobj_BoxVT *const self, T *const p)
{
    cobj_BoxV_own(&self->inner, p);
}

T *cobj_BoxVT_disown(cobj_BoxVT *const self)
{
    T *const p = cobj_BoxV_disown_member(&self->inner);
    cobj_BoxVT_wipe(self);
    return p;
}

void cobj_BoxVT_swap(cobj_BoxVT *const self, cobj_BoxVT *const other)
{
    cobj_BoxV_swap(&self->inner, &other->inner);
}

bool WARN_UNUSED_RESULT cobj_BoxVT_try_from_T(cobj_BoxVT *const self,
                                              T *const v,
                                              cobj_Error *const err)
{
    return cobj_BoxV_try_from_V(&self->inner,
                                v,
                                err,
                                cobj_BoxVT_elem_try_alloc,
                                cobj_BoxVT_elem_move);
}

bool WARN_UNUSED_RESULT cobj_BoxVT_try_copy_T(cobj_BoxVT *const self,
                                              T const *const v,
                                              cobj_Error *const err)
{
    return cobj_BoxV_try_copy_V(&self->inner,
                                v,
                                err,
                                cobj_BoxVT_elem_try_alloc,
                                cobj_BoxVT_elem_free,
                                cobj_BoxVT_elem_try_copy);
}
