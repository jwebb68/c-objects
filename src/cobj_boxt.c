#include "cobj_boxt.h"

#include "cobj_memory.h" // memwipe/STRUCTWIPE

#include <stddef.h> // NULL
#include <stdlib.h> // malloc/free

static T *_T_malloc(void)
{
    return (T *)malloc(sizeof(T));
}

static void BoxT_wipe(BoxT *const self)
{
    STRUCTWIPE(self);
}

void BoxT_destroy(BoxT *const self)
{
    T_destroy(self->elem);
    free(self->elem);
    BoxT_wipe(self);
}

void BoxT_move(BoxT *const self, BoxT *const src)
{
    *self = *src;
    BoxT_wipe(src);
}

bool WARN_UNUSED_RESULT BoxT_try_copy(BoxT *const self, BoxT const *const src, Error *const err)
{
    T *const p = _T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    bool ok;
    ok = T_try_copy(p, src->elem, err);
    if (!ok) {
        free(p);
        return false;
    }

    self->elem = p;
    return true;
}

T const *BoxT_deref(BoxT const *const self)
{
    return self->elem;
}

T *BoxT_deref_mut(BoxT *const self)
{
    return self->elem;
}

bool BoxT_is_eq(BoxT const *const self, BoxT const *const b)
{
    return T_is_eq(self->elem, b->elem);
}

bool BoxT_is_ne(BoxT const *const self, BoxT const *const b)
{
    return T_is_ne(self->elem, b->elem);
}

bool BoxT_is_lt(BoxT const *const self, BoxT const *const b)
{
    return T_is_lt(self->elem, b->elem);
}

bool BoxT_is_gt(BoxT const *const self, BoxT const *const b)
{
    return T_is_gt(self->elem, b->elem);
}

void BoxT_new_own(BoxT *const self, T *const p)
{
    self->elem = p;
}

bool WARN_UNUSED_RESULT BoxT_try_new_int(BoxT *const self, int v, Error *const err)
{
    // err, malloc can fail, so new can fail
    // try_new ?
    T *const p = _T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    T_new_int(p, v);
    self->elem = p;
    return true;
}

bool WARN_UNUSED_RESULT BoxT_try_new_from_T(BoxT *const self, T *const v, Error *const err)
{
    T *const p = _T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    T_move(p, v);
    self->elem = p;
    return true;
}

bool WARN_UNUSED_RESULT BoxT_try_new_copy_T(BoxT *const self, T const *const v, Error *const err)
{
    T *const p = _T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    bool ok;
    ok = T_try_copy(p, v, err);
    if (!ok) {
        free(p);
        return false;
    }

    self->elem = p;
    return true;
}

// hmm, BoxT look like unique_ptr implementation
// but without the release
