#include <cobj/based/cobj_carrayvt.h>
#include <string.h> // memset

static void CArrayVT_elem_destroy(void *const self)
{
    T_destroy(self);
}

static void CArrayVT_elem_default(void *const self)
{
    T_default(self);
}

static bool WARN_UNUSED_RESULT CArrayVT_elem_try_copy(void *const self,
                                                      void const *const src,
                                                      Error *const err)
{
    return T_try_copy(self, src, err);
}

static void const *CArrayVT_elem_ptr(void const *const arr, size_t idx)
{
    return CArrayVT_get(arr, idx);
}

static void *CArrayVT_elem_ptr_mut(void *const arr, size_t idx)
{
    return CArrayVT_get_mut(arr, idx);
}

static void CArrayVT_wipe(T *const arr, size_t len)
{
    uint8_t *const e = (uint8_t *)CArrayVT_get_mut(arr, len);
    memset(arr, 0xa5, e - (uint8_t *)arr);
}

void CArrayVT_destroy_member(T *const arr, size_t len)
{
    CArrayV_destroy_member(arr, len, CArrayVT_elem_ptr_mut, CArrayVT_elem_destroy);
}

void CArrayVT_destroy(T *const arr, size_t len)
{
    CArrayVT_destroy_member(arr, len);
    CArrayVT_wipe(arr, len);
}

void CArrayVT_move_member(T *const arr, size_t len, T *const src)
{
    CArrayV_move_member(arr, len, src, CArrayVT_elem_ptr_mut);
}
void CArrayVT_move(T *const arr, size_t len, T *const src)
{
    CArrayVT_move_member(arr, len, src);
    CArrayVT_wipe(src, len);
}

bool WARN_UNUSED_RESULT CArrayVT_try_copy(T *const arr, size_t len, T const *const src, Error *err)
{
    return CArrayV_try_copy(arr,
                            len,
                            src,
                            err,
                            CArrayVT_elem_ptr,
                            CArrayVT_elem_ptr_mut,
                            CArrayVT_elem_try_copy,
                            CArrayVT_elem_destroy);
}

bool WARN_UNUSED_RESULT CArrayVT_try_fill(T *const arr, size_t len, T const *const src, Error *err)
{
    return CArrayV_try_fill(arr,
                            len,
                            src,
                            err,
                            CArrayVT_elem_ptr_mut,
                            CArrayVT_elem_try_copy,
                            CArrayVT_elem_destroy);
}

void CArrayVT_default(T *const arr, size_t len)
{
    CArrayV_default(arr, len, CArrayVT_elem_ptr_mut, CArrayVT_elem_default);
}

#if 0
void CArrayVT_default(T *const b, T *const e)
{
    CArrayV_default(b, e, sizeof(T), CArrayVT_elem_default);
}
#endif

T *CArrayVT_get_mut(T *const arr, size_t idx)
{
    return &((T *)arr)[idx];
}

T const *CArrayVT_get(T const *const arr, size_t idx)
{
    return &((T const *)arr)[idx];
}

void CArrayVT_as_SliceVT(T const *const arr, size_t len, SliceVT *const s)
{
    CArrayV_as_SliceV(arr, len, &s->inner);
}

void CArrayVT_as_SliceVTMut(T *const arr, size_t len, SliceVTMut *const s)
{
    CArrayV_as_SliceVMut(arr, len, &s->inner);
}

void CArrayVT_iter(T const *const arr, size_t len, SliceVTIter *const it)
{
    CArrayV_iter(arr, len, &it->inner, CArrayVT_elem_ptr);
}

void CArrayVT_iter_mut(T *const arr, size_t len, SliceVTMutIter *const it)
{
    CArrayV_iter_mut(arr, len, &it->inner, CArrayVT_elem_ptr_mut);
}
