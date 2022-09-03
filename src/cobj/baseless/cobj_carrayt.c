#include <cobj/baseless/cobj_carrayt.h>
#include <cobj/core/cobj_memory.h> //memwipe
#include <inttypes.h> // uint8_t
#include <string.h> // memmove

T const *cobj_CArrayT_try_get(T const *const arr, size_t len, size_t pos)
{
    if (pos >= len) { return NULL; }
    T const *const p = &arr[pos];
    return p;
}

void cobj_CArrayT_as_cobj_SliceT(T const *const arr, size_t len, cobj_SliceT *const s)
{
    cobj_SliceT_new(s, arr, len);
}

void cobj_CArrayT_iter(T const *const arr, size_t len, cobj_SliceTIter *const it)
{
    cobj_SliceTIter_new(it, arr, arr + len);
}

// ===========================================================================
static void cobj_CArrayTMut_wipe(T *const arr, size_t len)
{
    memset(arr, 0xa5, (uint8_t *)(arr + len) - (uint8_t *)arr);
}
static void _cobj_CArrayTMut_wipe(T *const b, T *const e)
{
    memset(b, 0xa5, (uint8_t *)e - (uint8_t *)b);
}

static void _cobj_CArrayTMut_destroy_member(T *const b, T *const e)
{
    // for (T *it = b; it != e; ++it) {
    //     T_destroy_member(it);
    // }
    for (T *it = e; it != b;) {
        --it;
        T_destroy_member(it);
    }
}

void cobj_CArrayTMut_destroy_member(T *const arr, size_t len)
{
    _cobj_CArrayTMut_destroy_member(arr, arr + len);
}

void cobj_CArrayTMut_destroy(T *const arr, size_t len)
{
    cobj_CArrayTMut_destroy_member(arr, len);
    cobj_CArrayTMut_wipe(arr, len);
}

void cobj_CArrayTMut_move_member(T *const arr, size_t len, T *const src)
{
    memmove(arr, src, len * sizeof(T));
}
void cobj_CArrayTMut_move(T *const arr, size_t len, T *const src)
{
    cobj_CArrayTMut_move_member(arr, len, src);
    cobj_CArrayTMut_wipe(src, len);
}

bool WARN_UNUSED_RESULT _cobj_CArrayTMut_try_copy(T *const b,
                                                  T *const e,
                                                  T const *const s,
                                                  cobj_Error *err)
{
    bool ok = true;
    T *it;
    T const *s_i = s;
    for (it = b; ok && it != e; ++it, ++s_i) {
        ok = T_try_copy_member(it, s_i, err);
    }
    if (!ok) {
        _cobj_CArrayTMut_destroy_member(b, it);
        // if failed then it needs to be wiped as well..
        // if failed then it will be >b and < e, so can do the +1
        _cobj_CArrayTMut_wipe(b, it + 1);
    }
    return ok;
}

bool WARN_UNUSED_RESULT cobj_CArrayTMut_try_copy(T *const arr,
                                                 size_t len,
                                                 T const *const src,
                                                 cobj_Error *err)
{
    if (!_cobj_CArrayTMut_try_copy(arr, arr + len, src, err)) {
        cobj_CArrayTMut_wipe(arr, len);
        return false;
    }
    return true;
}

bool WARN_UNUSED_RESULT _cobj_CArrayTMut_try_fill(T *const b,
                                                  T *const e,
                                                  T const *const v,
                                                  cobj_Error *err)
{
    bool ok = true;
    T *it;
    for (it = b; ok && it != e; ++it) {
        ok = T_try_copy_member(it, v, err);
    }
    if (!ok) {
        _cobj_CArrayTMut_destroy_member(b, it);
        // if failed then it needs to be wiped as well..
        // if failed then it will be >b and < e, so can do the +1
        _cobj_CArrayTMut_wipe(b, it + 1);
    }
    return ok;
}

bool WARN_UNUSED_RESULT cobj_CArrayTMut_try_fill(T *const arr,
                                                 size_t len,
                                                 T const *const v,
                                                 cobj_Error *err)
{
    return _cobj_CArrayTMut_try_fill(arr, arr + len, v, err);
}

static void _cobj_CArrayTMut_default(T *const b, T *const e)
{
    for (T *it = b; it != e; ++it) {
        T_default(it);
    }
}

void cobj_CArrayTMut_default(T *const arr, size_t len)
{
    _cobj_CArrayTMut_default(arr, arr + len);
}

void cobj_CArrayTMut_erase_member(T *const arr, size_t b, size_t len)
{
    _cobj_CArrayTMut_destroy_member(arr + b, arr + b + len);
}

void cobj_CArrayTMut_erase(T *const arr, size_t b, size_t len)
{
    cobj_CArrayTMut_erase_member(arr, b, len);
    cobj_CArrayTMut_wipe(arr + b, len);
}

T *cobj_CArrayTMut_try_get(T *const arr, size_t len, size_t pos)
{
    if (pos >= len) { return NULL; }
    T *const p = &arr[pos];
    return p;
}

void cobj_CArrayTMut_as_cobj_SliceT(T *const arr, size_t len, cobj_SliceT *const s)
{
    cobj_SliceT_new(s, arr, len);
}

void cobj_CArrayTMut_as_cobj_SliceTMut(T *const arr, size_t len, cobj_SliceTMut *const s)
{
    cobj_SliceTMut_new(s, arr, len);
}

void cobj_CArrayTMut_iter(T *const arr, size_t len, cobj_SliceTIter *const it)
{
    cobj_SliceTIter_new(it, arr, arr + len);
}

void cobj_CArrayTMut_iter_mut(T *const arr, size_t len, cobj_SliceTMutIter *const it)
{
    cobj_SliceTMutIter_new(it, arr, arr + len);
}
