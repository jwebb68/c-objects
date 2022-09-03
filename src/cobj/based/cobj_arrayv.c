#include <cobj/based/cobj_arrayv.h>
#include <cobj/based/cobj_carrayv.h>
#include <cobj/core/cobj_memory.h> //memwipe

static void cobj_ArrayV_wipe(cobj_ArrayV *const self)
{
    STRUCTWIPE(self);
}

void cobj_ArrayV_destroy_member(cobj_ArrayV *const self,
                                void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                void (*elem_destroy_member)(void *const elem))
{
    cobj_CArrayV_destroy_member(self->ptr, self->len, elem_ptr_mut, elem_destroy_member);
}

void cobj_ArrayV_destroy(cobj_ArrayV *const self,
                         void *(*elem_ptr_mut)(void *const arr, size_t idx),
                         void (*elem_destroy_member)(void *const elem))
{
    cobj_CArrayV_destroy(self->ptr, self->len, elem_ptr_mut, elem_destroy_member);
    cobj_ArrayV_wipe(self);
}

void cobj_ArrayV_move(cobj_ArrayV *const self, cobj_ArrayV *const src)
{
    // moving struct not contents.
    *self = *src;
    cobj_ArrayV_wipe(src);
}

void cobj_ArrayV_own(cobj_ArrayV *const self, void *const ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

void cobj_ArrayV_disown(cobj_ArrayV *const self, void **const ptr, size_t *const len)
{
    *ptr = self->ptr;
    *len = self->len;
    cobj_ArrayV_wipe(self);
}

// bool WARN_UNUSED_RESULT cobj_ArrayV_try_copy(cobj_ArrayV *const self,
//                                         cobj_ArrayV const *const src,
//                                         cobj_Error*const err,
//                                         void const *(*elem_ptr)(void const *const arr, size_t
//                                         idx), void *(*elem_ptr_mut)(void *const arr, size_t idx),
//                                         bool(elem_try_copy)(void *const dest,
//                                                             void const *const src,
//                                                             cobj_Error*const err),
//                                         void (*elem_destroy)(void *const elem))
// {
//     if (self->len != src->len) { return COBJ_ERROR_RAISE(err, cobj_cobj_ErrorCode_EFAIL); }
//     return cobj_CArrayV_try_copy(self->ptr,
//                             self->len,
//                             src->ptr,
//                             err,
//                             elem_ptr,
//                             elem_ptr_mut,
//                             elem_try_copy,
//                             elem_destroy);
// }

void cobj_ArrayV_default(cobj_ArrayV *const self,
                         void *const arr,
                         size_t len,
                         void *(*elem_ptr_mut)(void *const arr, size_t idx),
                         void (*elem_default)(void *const elem))
{
    self->ptr = arr;
    self->len = len;
    cobj_CArrayV_default(self->ptr, self->len, elem_ptr_mut, elem_default);
}

void cobj_ArrayV_as_cobj_SliceV(cobj_ArrayV const *const self, cobj_SliceV *const s)
{
    cobj_CArrayV_as_cobj_SliceV(self->ptr, self->len, s);
}

void cobj_ArrayV_as_cobj_SliceVMut(cobj_ArrayV *const self, cobj_SliceVMut *const s)
{
    cobj_CArrayV_as_cobj_SliceVMut(self->ptr, self->len, s);
}

void cobj_ArrayV_iter(cobj_ArrayV const *const self,
                      cobj_SliceVIter *const it,
                      void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    cobj_CArrayV_iter(self->ptr, self->len, it, elem_ptr);
}

void cobj_ArrayV_iter_mut(cobj_ArrayV *const self,
                          cobj_SliceVMutIter *const it,
                          void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    cobj_CArrayV_iter_mut(self->ptr, self->len, it, elem_ptr_mut);
}
