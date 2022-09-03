#include <cobj/baseless/cobj_arrayt.h>
#include <cobj/baseless/cobj_carrayt.h>
#include <cobj/core/cobj_memory.h> //memwipe

static void cobj_ArrayT_wipe(cobj_ArrayT *const self)
{
    STRUCTWIPE(self);
}

void cobj_ArrayT_destroy_member(cobj_ArrayT *const self)
{
    COBJ_UNUSED_ARG(self);
}

void cobj_ArrayT_destroy(cobj_ArrayT *const self)
{
    cobj_ArrayT_destroy_member(self);
    cobj_ArrayT_wipe(self);
}

void cobj_ArrayT_move(cobj_ArrayT *const self, cobj_ArrayT *const src)
{
    *self = *src;
    cobj_ArrayT_wipe(src);
}

void cobj_ArrayT_own(cobj_ArrayT *const self, T const *const ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}
void cobj_ArrayT_disown(cobj_ArrayT *const self, T const **const ptr, size_t *const len)
{
    *ptr = self->ptr;
    *len = self->len;
    cobj_ArrayT_wipe(self);
}
void cobj_ArrayT_swap(cobj_ArrayT *const self, cobj_ArrayT *const other)
{
    ptrswap((void **)&self->ptr, (void **)&other->ptr);
    sizetswap(&self->len, &other->len);
}

T const *cobj_ArrayT_ptr(cobj_ArrayT const *const self)
{
    return self->ptr;
}

size_t cobj_ArrayT_len(cobj_ArrayT const *const self)
{
    return self->len;
}

T const *cobj_ArrayT_try_get(cobj_ArrayT const *const self, cobj_Index pos)
{
    return cobj_CArrayT_try_get(self->ptr, self->len, pos);
}

void cobj_ArrayT_as_cobj_SliceT(cobj_ArrayT const *const self, cobj_SliceT *const s)
{
    cobj_CArrayT_as_cobj_SliceT(self->ptr, self->len, s);
}

void cobj_ArrayT_iter(cobj_ArrayT const *const self, cobj_SliceTIter *const it)
{
    cobj_CArrayT_iter(self->ptr, self->len, it);
}

void cobj_ArrayT_from_cobj_ArrayTMut(cobj_ArrayT *const self, cobj_ArrayTMut *const src)
{
    cobj_ArrayTMut_into_cobj_ArrayT(src, self);
}

// =========================================================================

static void cobj_ArrayTMut_wipe(cobj_ArrayTMut *const self)
{
    STRUCTWIPE(self);
}

void cobj_ArrayTMut_destroy_member(cobj_ArrayTMut *const self)
{
    cobj_CArrayTMut_destroy_member(self->ptr, self->len);
}
void cobj_ArrayTMut_destroy(cobj_ArrayTMut *const self)
{
    cobj_ArrayTMut_destroy_member(self);
    cobj_ArrayTMut_wipe(self);
}

void cobj_ArrayTMut_move_member(cobj_ArrayTMut *const self, cobj_ArrayTMut *const src)
{
    *self = *src;
}
void cobj_ArrayTMut_move(cobj_ArrayTMut *const self, cobj_ArrayTMut *const src)
{
    cobj_ArrayTMut_move_member(self, src);
    cobj_ArrayTMut_wipe(src);
}

// bool WARN_UNUSED_RESULT cobj_ArrayTMut_try_copy_from(cobj_ArrayTMut *const self,
//                                            cobj_ArrayT const *const src,
//                                            cobj_Error *err)
// {
//     if (self->len != src->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
//     if (!cobj_CArrayTMut_try_copy(self->ptr, self->len, src->ptr, err)) {
//         cobj_ArrayTMut_wipe(self);
//         return false;
//     }
//     return true;
// }

// bool WARN_UNUSED_RESULT cobj_ArrayTMut_try_copy_mut(cobj_ArrayTMut *const self,
//                                                cobj_ArrayTMut const *const src,
//                                                cobj_Error *err)
// {
//     if (self->len != src->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
//     if (!cobj_CArrayTMut_try_copy(self->ptr, self->len, src->ptr, err)) {
//         cobj_ArrayTMut_wipe(self);
//         return false;
//     }
//     return true;
// }

void cobj_ArrayTMut_own(cobj_ArrayTMut *const self, T *const arr, size_t len)
{
    self->ptr = arr;
    self->len = len;
}

void cobj_ArrayTMut_disown(cobj_ArrayTMut *const self, T **const arr, size_t *const len)
{
    *arr = self->ptr;
    *len = self->len;
    cobj_ArrayTMut_wipe(self);
}
void cobj_ArrayTMut_swap(cobj_ArrayTMut *const self, cobj_ArrayTMut *const other)
{
    ptrswap((void **)&self->ptr, (void **)&other->ptr);
    sizetswap(&self->len, &other->len);
}

void cobj_ArrayTMut_default(cobj_ArrayTMut *const self, T *const arr, size_t len)
{
    cobj_ArrayTMut_own(self, arr, len);
    cobj_CArrayTMut_default(arr, len);
}

// void cobj_ArrayTMut_fill_with(cobj_ArrayTMut *const self, T *const arr, size_t len, void
// (*elem_init)(T *const self))
// {
//     cobj_CArrayTMut_fill_with(arr, len, elem_init);
// }

T *cobj_ArrayTMut_ptr(cobj_ArrayTMut const *const self)
{
    return self->ptr;
}

size_t cobj_ArrayTMut_len(cobj_ArrayTMut const *const self)
{
    return self->len;
}

T *cobj_ArrayTMut_try_get(cobj_ArrayTMut const *const self, cobj_Index pos)
{
    return cobj_CArrayTMut_try_get(self->ptr, self->len, pos);
}

void cobj_ArrayTMut_into_cobj_ArrayT(cobj_ArrayTMut *const self, cobj_ArrayT *const arr)
{
    arr->ptr = self->ptr;
    arr->len = self->len;
    cobj_ArrayTMut_wipe(self);
}

// two structs cannot hold reference to same array!...
// void cobj_ArrayTMut_as_cobj_ArrayT(cobj_ArrayTMut const *const self, cobj_ArrayT *const arr)
// {
//     arr->ptr = self->ptr;
//     arr->len = self->len;
// }

void cobj_ArrayTMut_as_cobj_SliceT(cobj_ArrayTMut const *const self, cobj_SliceT *const s)
{
    cobj_CArrayTMut_as_cobj_SliceT(self->ptr, self->len, s);
}

void cobj_ArrayTMut_as_cobj_SliceTMut(cobj_ArrayTMut const *const self, cobj_SliceTMut *const s)
{
    cobj_CArrayTMut_as_cobj_SliceTMut(self->ptr, self->len, s);
}

void cobj_ArrayTMut_iter(cobj_ArrayTMut const *const self, cobj_SliceTIter *const it)
{
    cobj_CArrayTMut_iter(self->ptr, self->len, it);
}

void cobj_ArrayTMut_iter_mut(cobj_ArrayTMut const *const self, cobj_SliceTMutIter *const it)
{
    cobj_CArrayTMut_iter_mut(self->ptr, self->len, it);
}
