#include <cobj/based/cobj_arrayv.h>
#include <cobj/based/cobj_arrayvt.h>
#include <cobj/based/cobj_carrayvt.h>
#include <cobj/core/cobj_memory.h> //memwipe

static void cobj_ArrayVT_elem_destroy_member(void *const self)
{
    T_destroy_member(self);
}

static void cobj_ArrayVT_elem_default(void *const self)
{
    T_default(self);
}

// static bool WARN_UNUSED_RESULT cobj_ArrayVT_elem_try_copy_member(void *const self,
//                                                      void const *const src,
//                                                      cobj_Error*const err)
// {
//     return T_try_copy_member(self, src, err);
// }

static void const *cobj_ArrayVT_get(void const *const arr, size_t idx)
{
    return cobj_CArrayVT_get(arr, idx);
}

static void *cobj_ArrayVT_get_mut(void *const arr, size_t idx)
{
    return cobj_CArrayVT_get_mut(arr, idx);
}

static void cobj_ArrayVT_wipe(cobj_ArrayVT *const self)
{
    STRUCTWIPE(self);
}

void cobj_ArrayVT_destroy_member(cobj_ArrayVT *const self)
{
    cobj_ArrayV_destroy_member(&self->inner,
                               cobj_ArrayVT_get_mut,
                               cobj_ArrayVT_elem_destroy_member);
}
void cobj_ArrayVT_destroy(cobj_ArrayVT *const self)
{
    cobj_ArrayVT_destroy_member(self);
    cobj_ArrayVT_wipe(self);
}

void cobj_ArrayVT_move(cobj_ArrayVT *const self, cobj_ArrayVT *const src)
{
    *self = *src;
    cobj_ArrayVT_wipe(src);
}

// bool WARN_UNUSED_RESULT cobj_ArrayVT_try_copy(cobj_ArrayVT *const self, cobj_ArrayVT const *const
// src, cobj_Error*err)
// {
//     return cobj_ArrayV_try_copy(&self->inner,
//                            &src->inner,
//                            err,
//                            cobj_ArrayVT_get,
//                            cobj_ArrayVT_get_mut,
//                            cobj_ArrayVT_elem_try_copy,
//                            cobj_ArrayVT_elem_destroy);
// }

void cobj_ArrayVT_default(cobj_ArrayVT *const self, T *const arr, size_t len)
{
    cobj_ArrayV_default(&self->inner, arr, len, cobj_ArrayVT_get_mut, cobj_ArrayVT_elem_default);
}

void cobj_ArrayVT_own(cobj_ArrayVT *const self, T *const arr, size_t len)
{
    cobj_ArrayV_own(&self->inner, arr, len);
}

void cobj_ArrayVT_disown(cobj_ArrayVT *const self, T **const arr, size_t *const len)
{
    cobj_ArrayV_disown(&self->inner, (void **)arr, len);
}

void cobj_ArrayVT_as_cobj_SliceVT(cobj_ArrayVT const *const self, cobj_SliceVT *const s)
{
    cobj_ArrayV_as_cobj_SliceV(&self->inner, &s->inner);
}

void cobj_ArrayVT_as_cobj_SliceVTMut(cobj_ArrayVT *const self, cobj_SliceVTMut *const s)
{
    cobj_ArrayV_as_cobj_SliceVMut(&self->inner, &s->inner);
}

void cobj_ArrayVT_iter(cobj_ArrayVT const *const self, cobj_SliceVTIter *const it)
{
    cobj_ArrayV_iter(&self->inner, &it->inner, cobj_ArrayVT_get);
}

void cobj_ArrayVT_iter_mut(cobj_ArrayVT *const self, cobj_SliceVTMutIter *const it)
{
    cobj_ArrayV_iter_mut(&self->inner, &it->inner, cobj_ArrayVT_get_mut);
}
