#include <cobj/based/cobj_carrayv.h>
#include <cobj/core/cobj_memory.h> //memwipe
#include <string.h> // memmove

void cobj_CArrayV_wipe(void *const arr,
                       size_t len,
                       void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    uint8_t *const b = arr;
    uint8_t *const e = elem_ptr_mut(arr, len);
    memset(b, 0xa5, e - b);
}

void cobj_CArrayV_destroy_member(void *const arr,
                                 size_t len,
                                 void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                 void (*elem_destroy_member)(void *const elem))
{
    for (size_t i = 0; i < len; ++i) {
        void *const p = elem_ptr_mut(arr, i);
        elem_destroy_member(p);
    }
}

void cobj_CArrayV_erase_member(void *const arr,
                               size_t b,
                               size_t len,
                               void *(*elem_ptr_mut)(void *const arr, size_t idx),
                               void (*elem_destroy_member)(void *const elem))
{
    for (size_t i = b; i < len; ++i) {
        void *const p = elem_ptr_mut(arr, i);
        elem_destroy_member(p);
    }
}

void cobj_CArrayV_destroy(void *const arr,
                          size_t len,
                          void *(*elem_ptr_mut)(void *const arr, size_t idx),
                          void (*elem_destroy_member)(void *const elem))
{
    cobj_CArrayV_destroy_member(arr, len, elem_ptr_mut, elem_destroy_member);
    cobj_CArrayV_wipe(arr, len, elem_ptr_mut);
}

void cobj_CArrayV_erase(void *const arr,
                        size_t b,
                        size_t len,
                        void *(*elem_ptr_mut)(void *const arr, size_t idx),
                        void (*elem_destroy_member)(void *const elem))
{
    cobj_CArrayV_erase_member(arr, b, len, elem_ptr_mut, elem_destroy_member);
    cobj_CArrayV_wipe(arr, len, elem_ptr_mut);
}

void cobj_CArrayV_move_member(void *const arr,
                              size_t len,
                              void *const src,
                              void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    // self/dest must be uninitialised
    // This will mean a mid-range move will have a very bad contents overall in the dest.
    // After the move, the src will be in a bad state if it's a midrange slice.
    // (an uninitialised block in a block of initialised)
    // use memmove to move the bits en masse, for speed.
    // bad: memmove src is const, when shouldn't be.
    uint8_t *const sb = (uint8_t *)src;
    uint8_t *const se = elem_ptr_mut(src, len);
    memmove(arr, src, se - sb);
}

void cobj_CArrayV_move(void *const arr,
                       size_t len,
                       void *const src,
                       void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    cobj_CArrayV_move_member(arr, len, src, elem_ptr_mut);
    cobj_CArrayV_wipe(arr, len, elem_ptr_mut);
}

bool WARN_UNUSED_RESULT cobj_CArrayV_try_copy(
    void *const arr,
    size_t len,
    void const *const src,
    cobj_Error *const err,
    void const *(*elem_ptr)(void const *const arr, size_t idx),
    void *(*elem_ptr_mut)(void *const arr, size_t idx),
    bool(try_elem_copy_member)(void *const dest, void const *const src, cobj_Error *const err),
    void (*elem_destroy)(void *const elem))
{
    // If initially initialised, does the fail path preserve the orig data?
    // what if this is in the middle of an array? so can end up with de-initialised items in array
    // of initialised. preserving original state is not possible without another array. so,
    // try_copy_from cannot be used to mutate contents of an initialised array. try_copy_from can be
    // used to populate an uninitialised array.
    bool ok = true;
    size_t i = 0;
    for (; ok && i < len; ++i) {
        uint8_t *const d = elem_ptr_mut(arr, i);
        uint8_t const *const s = elem_ptr(src, i);
        ok = try_elem_copy_member(d, s, err);
    }
    if (!ok) {
        cobj_CArrayV_destroy_member(arr, i, elem_ptr_mut, elem_destroy);
        cobj_CArrayV_wipe(arr, i + 1, elem_ptr_mut);
    }
    return ok;
}

bool WARN_UNUSED_RESULT cobj_CArrayV_try_fill(void *const arr,
                                              size_t len,
                                              void const *const v,
                                              cobj_Error *const err,
                                              void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                              bool(try_elem_copy_member)(void *const dest,
                                                                         void const *const src,
                                                                         cobj_Error *const err),
                                              void (*elem_destroy_member)(void *const elem))
{
    // If initially initialised, does the fail path preserve the orig data?
    // what if this is in the middle of an array? so can end up with de-initialised items in array
    // of initialised. preserving original state is not possible without another array. so, try_fill
    // cannot be used to mutate contents of an initialised array. try_fill can be used to populate
    // an uninitialised array.
    bool ok = true;
    size_t i = 0;
    for (; ok && i < len; ++i) {
        uint8_t *const d = elem_ptr_mut(arr, i);
        ok = try_elem_copy_member(d, v, err);
    }
    if (!ok) {
        cobj_CArrayV_destroy_member(arr, i, elem_ptr_mut, elem_destroy_member);
        cobj_CArrayV_wipe(arr, i + 1, elem_ptr_mut);
    }
    return ok;
}

void cobj_CArrayV_default(void *const arr,
                          size_t len,
                          void *(*elem_ptr_mut)(void *const arr, size_t idx),
                          void (*elem_default)(void *const elem))
{
    for (size_t i = 0; i < len; ++i) {
        void *p = elem_ptr_mut(arr, i);
        elem_default(p);
    }
}

void cobj_CArrayV_as_cobj_SliceV(void const *const arr, size_t len, cobj_SliceV *const s)
{
    cobj_SliceV_new(s, arr, len);
}

void cobj_CArrayV_as_cobj_SliceVMut(void *const arr, size_t len, cobj_SliceVMut *const s)
{
    cobj_SliceVMut_new(s, arr, len);
}

void cobj_CArrayV_iter(void const *const arr,
                       size_t len,
                       cobj_SliceVIter *const it,
                       void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    cobj_SliceVIter_new(it, arr, elem_ptr(arr, len));
}

void cobj_CArrayV_iter_mut(void *const arr,
                           size_t len,
                           cobj_SliceVMutIter *const it,
                           void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    cobj_SliceVMutIter_new(it, arr, elem_ptr_mut(arr, len));
}
