#include "cobj_carrayt.h"

#include "cobj_memory.h" //memwipe

#include <inttypes.h> // uint8_t
#include <string.h> // memmove

T const *CArrayT_try_get(T const *const arr, size_t len, size_t pos)
{
    if (pos >= len) { return NULL; }
    T const *const p = &arr[pos];
    return p;
}

void CArrayT_as_SliceT(T const *const arr, size_t len, SliceT *const s)
{
    SliceT_new(s, arr, len);
}

void CArrayT_iter(T const *const arr, size_t len, SliceTIter *const it)
{
    SliceTIter_new(it, arr, arr + len);
}

// ===========================================================================
static void CArrayTMut_wipe(T *const arr, size_t len)
{
    memset(arr, 0xa5, len * sizeof(T));
}
static void _CArrayTMut_wipe(T *const b, T *const e)
{
    memset(b, 0xa5, (uint8_t *)e - (uint8_t *)b);
}

static void _CArrayTMut_destroy_member(T *const b, T *const e)
{
    // for (T *it = b; it != e; ++it) {
    //     T_destroy_member(it);
    // }
    for (T *it = e; it != b;) {
        --it;
        T_destroy_member(it);
    }
}

void CArrayTMut_destroy_member(T *const arr, size_t len)
{
    _CArrayTMut_destroy_member(arr, arr + len);
}

void CArrayTMut_destroy(T *const arr, size_t len)
{
    CArrayTMut_destroy_member(arr, len);
    CArrayTMut_wipe(arr, len);
}

void CArrayTMut_move_member(T *const arr, size_t len, T *const src)
{
    memmove(arr, src, len * sizeof(T));
}
void CArrayTMut_move(T *const arr, size_t len, T *const src)
{
    CArrayTMut_move_member(arr, len, src);
    CArrayTMut_wipe(src, len);
}

bool WARN_UNUSED_RESULT _CArrayTMut_try_copy(T *const b, T *const e, T const *const s, Error *err)
{
    bool ok = true;
    T *it;
    T const *s_i = s;
    for (it = b; ok && it != e; ++it, ++s_i) {
        ok = T_try_copy_member(it, s_i, err);
    }
    if (!ok) {
        _CArrayTMut_destroy_member(b, it);
        // if failed then it needs to be wiped as well..
        // if failed then it will be >b and < e, so can do the +1
        _CArrayTMut_wipe(b, it + 1);
    }
    return ok;
}

bool WARN_UNUSED_RESULT CArrayTMut_try_copy(T *const arr,
                                            size_t len,
                                            T const *const src,
                                            Error *err)
{
    if (!_CArrayTMut_try_copy(arr, arr + len, src, err)) {
        CArrayTMut_wipe(arr, len);
        return false;
    }
    return true;
}

bool WARN_UNUSED_RESULT _CArrayTMut_try_fill(T *const b, T *const e, T const *const v, Error *err)
{
    bool ok = true;
    T *it;
    for (it = b; ok && it != e; ++it) {
        ok = T_try_copy_member(it, v, err);
    }
    if (!ok) {
        _CArrayTMut_destroy_member(b, it);
        // if failed then it needs to be wiped as well..
        // if failed then it will be >b and < e, so can do the +1
        _CArrayTMut_wipe(b, it + 1);
    }
    return ok;
}

bool WARN_UNUSED_RESULT CArrayTMut_try_fill(T *const arr, size_t len, T const *const v, Error *err)
{
    return _CArrayTMut_try_fill(arr, arr + len, v, err);
}

static void _CArrayTMut_default(T *const b, T *const e)
{
    for (T *it = b; it != e; ++it) {
        T_default(it);
    }
}

void CArrayTMut_default(T *const arr, size_t len)
{
    _CArrayTMut_default(arr, arr + len);
}

void CArrayTMut_erase_member(T *const arr, size_t b, size_t len)
{
    _CArrayTMut_destroy_member(arr + b, arr + b + len);
}

void CArrayTMut_erase(T *const arr, size_t b, size_t len)
{
    CArrayTMut_erase_member(arr, b, len);
    CArrayTMut_wipe(arr + b, len);
}

T *CArrayTMut_try_get(T *const arr, size_t len, size_t pos)
{
    if (pos >= len) { return NULL; }
    T *const p = &arr[pos];
    return p;
}

void CArrayTMut_as_SliceT(T *const arr, size_t len, SliceT *const s)
{
    SliceT_new(s, arr, len);
}

void CArrayTMut_as_SliceTMut(T *const arr, size_t len, SliceTMut *const s)
{
    SliceTMut_new(s, arr, len);
}

void CArrayTMut_iter(T *const arr, size_t len, SliceTIter *const it)
{
    SliceTIter_new(it, arr, arr + len);
}

void CArrayTMut_iter_mut(T *const arr, size_t len, SliceTMutIter *const it)
{
    SliceTMutIter_new(it, arr, arr + len);
}
