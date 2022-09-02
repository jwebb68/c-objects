#include "cobj_arrayvt.h"

#include "cobj_arrayv.h"
#include "cobj_carrayvt.h"
#include "cobj_memory.h" //memwipe

static void ArrayVT_elem_destroy_member(void *const self)
{
    T_destroy_member(self);
}

static void ArrayVT_elem_default(void *const self)
{
    T_default(self);
}

// static bool WARN_UNUSED_RESULT ArrayVT_elem_try_copy_member(void *const self,
//                                                      void const *const src,
//                                                      Error *const err)
// {
//     return T_try_copy_member(self, src, err);
// }

static void const *ArrayVT_get(void const *const arr, size_t idx)
{
    return CArrayVT_get(arr, idx);
}

static void *ArrayVT_get_mut(void *const arr, size_t idx)
{
    return CArrayVT_get_mut(arr, idx);
}

static void ArrayVT_wipe(ArrayVT *const self)
{
    STRUCTWIPE(self);
}

void ArrayVT_destroy_member(ArrayVT *const self)
{
    ArrayV_destroy_member(&self->inner, ArrayVT_get_mut, ArrayVT_elem_destroy_member);
}
void ArrayVT_destroy(ArrayVT *const self)
{
    ArrayVT_destroy_member(self);
    ArrayVT_wipe(self);
}

void ArrayVT_move(ArrayVT *const self, ArrayVT *const src)
{
    *self = *src;
    ArrayVT_wipe(src);
}

// bool WARN_UNUSED_RESULT ArrayVT_try_copy(ArrayVT *const self, ArrayVT const *const src, Error
// *err)
// {
//     return ArrayV_try_copy(&self->inner,
//                            &src->inner,
//                            err,
//                            ArrayVT_get,
//                            ArrayVT_get_mut,
//                            ArrayVT_elem_try_copy,
//                            ArrayVT_elem_destroy);
// }

void ArrayVT_default(ArrayVT *const self, T *const arr, size_t len)
{
    ArrayV_default(&self->inner, arr, len, ArrayVT_get_mut, ArrayVT_elem_default);
}

void ArrayVT_own(ArrayVT *const self, T *const arr, size_t len)
{
    ArrayV_own(&self->inner, arr, len);
}

void ArrayVT_disown(ArrayVT *const self, T **const arr, size_t *const len)
{
    ArrayV_disown(&self->inner, (void **)arr, len);
}

void SliceV_into_SliceT(SliceV *const self, SliceT *const dest)
{
    dest->ptr = (T const *)self->ptr;
    dest->len = self->len;
    SliceV_wipe(self);
}

void ArrayVT_as_SliceT(ArrayVT const *const self, SliceT *const s)
{
    SliceV sv;
    ArrayV_as_SliceV(&self->inner, &sv);
    SliceV_into_SliceT(&sv, s);
}

static void SliceVMut_into_SliceTMut(SliceVMut *const self, SliceTMut *const dest)
{
    dest->ptr = (T *)self->ptr;
    dest->len = self->len;
    SliceVMut_wipe(self);
}

void ArrayVT_as_SliceTMut(ArrayVT *const self, SliceTMut *const s)
{
    SliceVMut sv;
    ArrayV_as_SliceVMut(&self->inner, &sv);
    SliceVMut_into_SliceTMut(&sv, s);
}

static void SliceVIter_into_SliceTIter(SliceVIter *const self, SliceTIter *const dest)
{
    dest->p = (T const *)self->p;
    dest->e = (T const *)self->e;
    SliceVIter_wipe(self);
}

void ArrayVT_iter(ArrayVT const *const self, SliceTIter *const it)
{
    SliceVIter si;
    ArrayV_iter(&self->inner, &si, ArrayVT_get);
    SliceVIter_into_SliceTIter(&si, it);
}

static void SliceVMutIter_into_SliceTMutIter(SliceVMutIter *const self, SliceTMutIter *const dest)
{
    dest->p = (T *)self->p;
    dest->e = (T *)self->e;
    SliceVMutIter_wipe(self);
}

void ArrayVT_iter_mut(ArrayVT *const self, SliceTMutIter *const it)
{
    SliceVMutIter si;
    ArrayV_iter_mut(&self->inner, &si, ArrayVT_get_mut);
    SliceVMutIter_into_SliceTMutIter(&si, it);
}
