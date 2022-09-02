#include "cobj_arrayt.h"

#include "cobj_carrayt.h"
#include "cobj_memory.h" //memwipe

static void ArrayT_wipe(ArrayT *const self)
{
    STRUCTWIPE(self);
}

void ArrayT_destroy_member(ArrayT *const self)
{
    UNUSED_ARG(self);
}

void ArrayT_destroy(ArrayT *const self)
{
    ArrayT_destroy_member(self);
    ArrayT_wipe(self);
}

void ArrayT_move(ArrayT *const self, ArrayT *const src)
{
    *self = *src;
    ArrayT_wipe(src);
}

void ArrayT_own(ArrayT *const self, T const *const ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}
void ArrayT_disown(ArrayT *const self, T const **const ptr, size_t *const len)
{
    *ptr = self->ptr;
    *len = self->len;
    ArrayT_wipe(self);
}
void ArrayT_swap(ArrayT *const self, ArrayT *const other)
{
    ptrswap((void **)&self->ptr, (void **)&other->ptr);
    sizetswap(&self->len, &other->len);
}

T const *ArrayT_ptr(ArrayT const *const self)
{
    return self->ptr;
}

size_t ArrayT_len(ArrayT const *const self)
{
    return self->len;
}

T const *ArrayT_try_get(ArrayT const *const self, Index pos)
{
    return CArrayT_try_get(self->ptr, self->len, pos);
}

void ArrayT_as_SliceT(ArrayT const *const self, SliceT *const s)
{
    CArrayT_as_SliceT(self->ptr, self->len, s);
}

void ArrayT_iter(ArrayT const *const self, SliceTIter *const it)
{
    CArrayT_iter(self->ptr, self->len, it);
}

void ArrayT_from_ArrayTMut(ArrayT *const self, ArrayTMut *const src)
{
    ArrayTMut_into_ArrayT(src, self);
}

// =========================================================================

static void ArrayTMut_wipe(ArrayTMut *const self)
{
    STRUCTWIPE(self);
}

void ArrayTMut_destroy_member(ArrayTMut *const self)
{
    CArrayTMut_destroy_member(self->ptr, self->len);
}
void ArrayTMut_destroy(ArrayTMut *const self)
{
    ArrayTMut_destroy_member(self);
    ArrayTMut_wipe(self);
}

void ArrayTMut_move_member(ArrayTMut *const self, ArrayTMut *const src)
{
    *self = *src;
}
void ArrayTMut_move(ArrayTMut *const self, ArrayTMut *const src)
{
    ArrayTMut_move_member(self, src);
    ArrayTMut_wipe(src);
}

// bool WARN_UNUSED_RESULT ArrayTMut_try_copy_from(ArrayTMut *const self,
//                                            ArrayT const *const src,
//                                            Error *err)
// {
//     if (self->len != src->len) { return ERROR_RAISE(err, Error_EFAIL); }
//     if (!CArrayTMut_try_copy(self->ptr, self->len, src->ptr, err)) {
//         ArrayTMut_wipe(self);
//         return false;
//     }
//     return true;
// }

// bool WARN_UNUSED_RESULT ArrayTMut_try_copy_mut(ArrayTMut *const self,
//                                                ArrayTMut const *const src,
//                                                Error *err)
// {
//     if (self->len != src->len) { return ERROR_RAISE(err, Error_EFAIL); }
//     if (!CArrayTMut_try_copy(self->ptr, self->len, src->ptr, err)) {
//         ArrayTMut_wipe(self);
//         return false;
//     }
//     return true;
// }

void ArrayTMut_own(ArrayTMut *const self, T *const arr, size_t len)
{
    self->ptr = arr;
    self->len = len;
}

void ArrayTMut_disown(ArrayTMut *const self, T **const arr, size_t *const len)
{
    *arr = self->ptr;
    *len = self->len;
    ArrayTMut_wipe(self);
}
void ArrayTMut_swap(ArrayTMut *const self, ArrayTMut *const other)
{
    ptrswap((void **)&self->ptr, (void **)&other->ptr);
    sizetswap(&self->len, &other->len);
}

void ArrayTMut_default(ArrayTMut *const self, T *const arr, size_t len)
{
    ArrayTMut_own(self, arr, len);
    CArrayTMut_default(arr, len);
}

// void ArrayTMut_fill_with(ArrayTMut *const self, T *const arr, size_t len, void (*elem_init)(T *const self))
// {
//     CArrayTMut_fill_with(arr, len, elem_init);
// }


T *ArrayTMut_ptr(ArrayTMut const *const self)
{
    return self->ptr;
}

size_t ArrayTMut_len(ArrayTMut const *const self)
{
    return self->len;
}

T *ArrayTMut_try_get(ArrayTMut const *const self, Index pos)
{
    return CArrayTMut_try_get(self->ptr, self->len, pos);
}

void ArrayTMut_into_ArrayT(ArrayTMut *const self, ArrayT *const arr)
{
    arr->ptr = self->ptr;
    arr->len = self->len;
    ArrayTMut_wipe(self);
}

// two structs cannot hold reference to same array!...
// void ArrayTMut_as_ArrayT(ArrayTMut const *const self, ArrayT *const arr)
// {
//     arr->ptr = self->ptr;
//     arr->len = self->len;
// }

void ArrayTMut_as_SliceT(ArrayTMut const *const self, SliceT *const s)
{
    CArrayTMut_as_SliceT(self->ptr, self->len, s);
}

void ArrayTMut_as_SliceTMut(ArrayTMut const *const self, SliceTMut *const s)
{
    CArrayTMut_as_SliceTMut(self->ptr, self->len, s);
}

void ArrayTMut_iter(ArrayTMut const *const self, SliceTIter *const it)
{
    CArrayTMut_iter(self->ptr, self->len, it);
}

void ArrayTMut_iter_mut(ArrayTMut const *const self, SliceTMutIter *const it)
{
    CArrayTMut_iter_mut(self->ptr, self->len, it);
}
