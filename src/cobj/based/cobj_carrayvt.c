#include <cobj/based/cobj_carrayvt.h>
#include <string.h> // memset

static void cobj_CArrayVT_elem_destroy(void *const self)
{
    T_destroy(self);
}

static void cobj_CArrayVT_elem_default(void *const self)
{
    T_default(self);
}

static bool WARN_UNUSED_RESULT cobj_CArrayVT_elem_try_copy(void *const self,
                                                           void const *const src,
                                                           cobj_Error *const err)
{
    return T_try_copy(self, src, err);
}

static void const *cobj_CArrayVT_elem_ptr(void const *const arr, size_t idx)
{
    return cobj_CArrayVT_get(arr, idx);
}

static void *cobj_CArrayVT_elem_ptr_mut(void *const arr, size_t idx)
{
    return cobj_CArrayVT_get_mut(arr, idx);
}

static void cobj_CArrayVT_wipe(T *const arr, size_t len)
{
    uint8_t *const e = (uint8_t *)cobj_CArrayVT_get_mut(arr, len);
    memset(arr, 0xa5, e - (uint8_t *)arr);
}

void cobj_CArrayVT_destroy_member(T *const arr, size_t len)
{
    cobj_CArrayV_destroy_member(arr, len, cobj_CArrayVT_elem_ptr_mut, cobj_CArrayVT_elem_destroy);
}

void cobj_CArrayVT_destroy(T *const arr, size_t len)
{
    cobj_CArrayVT_destroy_member(arr, len);
    cobj_CArrayVT_wipe(arr, len);
}

void cobj_CArrayVT_move_member(T *const arr, size_t len, T *const src)
{
    cobj_CArrayV_move_member(arr, len, src, cobj_CArrayVT_elem_ptr_mut);
}
void cobj_CArrayVT_move(T *const arr, size_t len, T *const src)
{
    cobj_CArrayVT_move_member(arr, len, src);
    cobj_CArrayVT_wipe(src, len);
}

bool WARN_UNUSED_RESULT cobj_CArrayVT_try_copy(T *const arr,
                                               size_t len,
                                               T const *const src,
                                               cobj_Error *err)
{
    return cobj_CArrayV_try_copy(arr,
                                 len,
                                 src,
                                 err,
                                 cobj_CArrayVT_elem_ptr,
                                 cobj_CArrayVT_elem_ptr_mut,
                                 cobj_CArrayVT_elem_try_copy,
                                 cobj_CArrayVT_elem_destroy);
}

bool WARN_UNUSED_RESULT cobj_CArrayVT_try_fill(T *const arr,
                                               size_t len,
                                               T const *const src,
                                               cobj_Error *err)
{
    return cobj_CArrayV_try_fill(arr,
                                 len,
                                 src,
                                 err,
                                 cobj_CArrayVT_elem_ptr_mut,
                                 cobj_CArrayVT_elem_try_copy,
                                 cobj_CArrayVT_elem_destroy);
}

void cobj_CArrayVT_default(T *const arr, size_t len)
{
    cobj_CArrayV_default(arr, len, cobj_CArrayVT_elem_ptr_mut, cobj_CArrayVT_elem_default);
}

#if 0
void cobj_CArrayVT_default(T *const b, T *const e)
{
    cobj_CArrayV_default(b, e, sizeof(T), cobj_CArrayVT_elem_default);
}
#endif

T *cobj_CArrayVT_get_mut(T *const arr, size_t idx)
{
    return &((T *)arr)[idx];
}

T const *cobj_CArrayVT_get(T const *const arr, size_t idx)
{
    return &((T const *)arr)[idx];
}

void cobj_CArrayVT_as_cobj_SliceVT(T const *const arr, size_t len, cobj_SliceVT *const s)
{
    cobj_CArrayV_as_cobj_SliceV(arr, len, &s->inner);
}

void cobj_CArrayVT_as_cobj_SliceVTMut(T *const arr, size_t len, cobj_SliceVTMut *const s)
{
    cobj_CArrayV_as_cobj_SliceVMut(arr, len, &s->inner);
}

void cobj_CArrayVT_iter(T const *const arr, size_t len, cobj_SliceVTIter *const it)
{
    cobj_CArrayV_iter(arr, len, &it->inner, cobj_CArrayVT_elem_ptr);
}

void cobj_CArrayVT_iter_mut(T *const arr, size_t len, cobj_SliceVTMutIter *const it)
{
    cobj_CArrayV_iter_mut(arr, len, &it->inner, cobj_CArrayVT_elem_ptr_mut);
}
