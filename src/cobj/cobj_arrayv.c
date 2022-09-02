#include "cobj_arrayv.h"

#include "cobj_carrayv.h"
#include "cobj_memory.h" //memwipe

static void ArrayV_wipe(ArrayV *const self)
{
    STRUCTWIPE(self);
}

void ArrayV_destroy_member(ArrayV *const self,
                           void *(*elem_ptr_mut)(void *const arr, size_t idx),
                           void (*elem_destroy_member)(void *const elem))
{
    CArrayV_destroy_member(self->ptr, self->len, elem_ptr_mut, elem_destroy_member);
}

void ArrayV_destroy(ArrayV *const self,
                    void *(*elem_ptr_mut)(void *const arr, size_t idx),
                    void (*elem_destroy_member)(void *const elem))
{
    CArrayV_destroy(self->ptr, self->len, elem_ptr_mut, elem_destroy_member);
    ArrayV_wipe(self);
}

void ArrayV_move(ArrayV *const self, ArrayV *const src)
{
    // moving struct not contents.
    *self = *src;
    ArrayV_wipe(src);
}

void ArrayV_own(ArrayV *const self, void *const ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

void ArrayV_disown(ArrayV *const self, void **const ptr, size_t *const len)
{
    *ptr = self->ptr;
    *len = self->len;
    ArrayV_wipe(self);
}

// bool WARN_UNUSED_RESULT ArrayV_try_copy(ArrayV *const self,
//                                         ArrayV const *const src,
//                                         Error *const err,
//                                         void const *(*elem_ptr)(void const *const arr, size_t
//                                         idx), void *(*elem_ptr_mut)(void *const arr, size_t idx),
//                                         bool(elem_try_copy)(void *const dest,
//                                                             void const *const src,
//                                                             Error *const err),
//                                         void (*elem_destroy)(void *const elem))
// {
//     if (self->len != src->len) { return ERROR_RAISE(err, Error_EFAIL); }
//     return CArrayV_try_copy(self->ptr,
//                             self->len,
//                             src->ptr,
//                             err,
//                             elem_ptr,
//                             elem_ptr_mut,
//                             elem_try_copy,
//                             elem_destroy);
// }

void ArrayV_default(ArrayV *const self,
                    void *const arr,
                    size_t len,
                    void *(*elem_ptr_mut)(void *const arr, size_t idx),
                    void (*elem_default)(void *const elem))
{
    self->ptr = arr;
    self->len = len;
    CArrayV_default(self->ptr, self->len, elem_ptr_mut, elem_default);
}

void ArrayV_as_SliceV(ArrayV const *const self, SliceV *const s)
{
    CArrayV_as_SliceV(self->ptr, self->len, s);
}

void ArrayV_as_SliceVMut(ArrayV *const self, SliceVMut *const s)
{
    CArrayV_as_SliceVMut(self->ptr, self->len, s);
}

void ArrayV_iter(ArrayV const *const self,
                 SliceVIter *const it,
                 void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    CArrayV_iter(self->ptr, self->len, it, elem_ptr);
}

void ArrayV_iter_mut(ArrayV *const self,
                     SliceVMutIter *const it,
                     void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    CArrayV_iter_mut(self->ptr, self->len, it, elem_ptr_mut);
}
