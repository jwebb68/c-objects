#include "cobj_boxv.h"

#include "cobj_memory.h" // STRUCTWIPE

#include <stddef.h> // NULL
#include <stdlib.h> // malloc/free

static void BoxV_wipe(BoxV *const self)
{
    STRUCTWIPE(self);
}

void BoxV_destroy(BoxV *const self, void (*elem_destroy)(void *const elem))
{
    elem_destroy(&self->elem);
    BoxV_wipe(self);
}

void BoxV_move(BoxV *const self, BoxV *const src)
{
    *self = *src;
    BoxV_wipe(src);
}

bool WARN_UNUSED_RESULT BoxV_try_copy(BoxV *const self,
                                      BoxV const *const v,
                                      Error *const err,
                                      size_t elem_size,
                                      bool (*elem_try_copy)(void *const elem,
                                                            void const *const elem_src,
                                                            Error *const err))
{
    void *p = malloc(elem_size);
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    bool ok;
    ok = elem_try_copy(p, BoxV_deref(v), err);
    if (!ok) {
        free(p);
        return ERROR_RAISE(err, Error_EFAIL);
    }

    BoxV_new_own(self, p);
    return true;
}

void const *BoxV_deref(BoxV const *const self)
{
    return self->elem;
}

void *BoxV_deref_mut(BoxV *const self)
{
    return self->elem;
}

void BoxV_new_own(BoxV *const self, void *const p)
{
    self->elem = p;
}

// void BoxV_new_(BoxV_ *const self, ...) {
//  T_new(&self->elem, ...);
//}

bool WARN_UNUSED_RESULT BoxV_try_new_from(BoxV *const self,
                                          void *const elem,
                                          Error *const err,
                                          size_t elem_size,
                                          void (*elem_move)(void *const elem, void *const src))
{
    void *p = malloc(elem_size);
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    elem_move(p, elem);
    BoxV_new_own(self, p);
    return true;
}
