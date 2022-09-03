#include <cobj/based/cobj_arrayv.h>
#include <cobj/based/cobj_arrayvt.h>
#include <cobj/based/cobj_carrayvt.h>
#include <cobj/core/cobj_memory.h> //memwipe

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

void ArrayVT_as_SliceVT(ArrayVT const *const self, SliceVT *const s)
{
    ArrayV_as_SliceV(&self->inner, &s->inner);
}

void ArrayVT_as_SliceVTMut(ArrayVT *const self, SliceVTMut *const s)
{
    ArrayV_as_SliceVMut(&self->inner, &s->inner);
}

void ArrayVT_iter(ArrayVT const *const self, SliceVTIter *const it)
{
    ArrayV_iter(&self->inner, &it->inner, ArrayVT_get);
}

void ArrayVT_iter_mut(ArrayVT *const self, SliceVTMutIter *const it)
{
    ArrayV_iter_mut(&self->inner, &it->inner, ArrayVT_get_mut);
}
