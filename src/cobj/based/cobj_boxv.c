#include <cobj/based/cobj_boxv.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE
#include <stddef.h> // NULL
#include <stdlib.h> // malloc/free

static void cobj_BoxV_wipe(cobj_BoxV *const self)
{
    STRUCTWIPE(self);
}

void cobj_BoxV_destroy_member(cobj_BoxV *const self,
                              void (*elem_destroy)(void *const elem),
                              void (*elem_free)(void *elem))
{
    elem_destroy(self->elem);
    elem_free(self->elem);
}

void cobj_BoxV_destroy(cobj_BoxV *const self,
                       void (*elem_destroy)(void *const elem),
                       void (*elem_free)(void *elem))
{
    cobj_BoxV_destroy_member(self, elem_destroy, elem_free);
    cobj_BoxV_wipe(self);
}

void cobj_BoxV_move_member(cobj_BoxV *const self, cobj_BoxV *const src)
{
    *self = *src;
}
void cobj_BoxV_move(cobj_BoxV *const self, cobj_BoxV *const src)
{
    cobj_BoxV_move_member(self, src);
    cobj_BoxV_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_BoxV_try_copy_member(cobj_BoxV *const self,
                                                  cobj_BoxV const *const v,
                                                  cobj_Error *const err,
                                                  void *(*elem_try_alloc)(cobj_Error *const err),
                                                  void (*elem_free)(void *elem),
                                                  bool (*elem_try_copy)(void *const elem,
                                                                        void const *const elem_src,
                                                                        cobj_Error *const err))
{
    void *p = elem_try_alloc(err);
    if (p == NULL) { return false; }

    bool ok;
    ok = elem_try_copy(p, cobj_BoxV_deref(v), err);
    if (!ok) {
        elem_free(p);
        // no need to wipe, as p not assigned into self
        // cobj_BoxV_wipe(self);
        return false;
    }

    cobj_BoxV_own(self, p);
    return true;
}

bool WARN_UNUSED_RESULT cobj_BoxV_try_copy(cobj_BoxV *const self,
                                           cobj_BoxV const *const v,
                                           cobj_Error *const err,
                                           void *(*elem_try_alloc)(cobj_Error *const err),
                                           void (*elem_free)(void *elem),
                                           bool (*elem_try_copy)(void *const elem,
                                                                 void const *const elem_src,
                                                                 cobj_Error *const err))
{
    if (!cobj_BoxV_try_copy_member(self, v, err, elem_try_alloc, elem_free, elem_try_copy)) {
        cobj_BoxV_wipe(self);
        return false;
    }
    return true;
}

void const *cobj_BoxV_deref(cobj_BoxV const *const self)
{
    return self->elem;
}

void *cobj_BoxV_deref_mut(cobj_BoxV *const self)
{
    return self->elem;
}

void cobj_BoxV_own(cobj_BoxV *const self, void *const p)
{
    self->elem = p;
}
void *cobj_BoxV_disown_member(cobj_BoxV *const self)
{
    return self->elem;
}
void *cobj_BoxV_disown(cobj_BoxV *const self)
{
    void *p = cobj_BoxV_disown_member(self);
    cobj_BoxV_wipe(self);
    return p;
}
void cobj_BoxV_swap(cobj_BoxV *const self, cobj_BoxV *const other)
{
    ptrswap(&self->elem, &other->elem);
}

// void cobj_BoxV_new...(cobj_BoxV_ *const self, ...) {
//  T_new(&self->elem, ...);
//}

bool WARN_UNUSED_RESULT cobj_BoxV_try_from_V(cobj_BoxV *const self,
                                             void *const elem,
                                             cobj_Error *const err,
                                             void *(*elem_try_alloc)(cobj_Error *const err),
                                             void (*elem_move)(void *const elem, void *const src))
{
    void *p = elem_try_alloc(err);
    if (p == NULL) { return false; }
    elem_move(p, elem);
    cobj_BoxV_own(self, p);
    return true;
}

void cobj_BoxV_into_V(cobj_BoxV *const self,
                      void *const elem,
                      void (*elem_move)(void *const elem, void *const src),
                      void (*elem_free)(void *elem))
{
    void *p = cobj_BoxV_disown(self);
    elem_move(elem, p);
    elem_free(p);
}

bool WARN_UNUSED_RESULT cobj_BoxV_try_copy_V(cobj_BoxV *const self,
                                             void const *const elem,
                                             cobj_Error *const err,
                                             void *(*elem_try_alloc)(cobj_Error *const err),
                                             void (*elem_free)(void *elem),
                                             bool (*elem_try_copy)(void *const elem,
                                                                   void const *const src,
                                                                   cobj_Error *const err))
{
    void *p = elem_try_alloc(err);
    if (p == NULL) { return false; }

    bool ok;
    ok = elem_try_copy(p, elem, err);
    if (!ok) {
        elem_free(p);
        // cobj_BoxV_wipe(self);
        return false;
    }
    cobj_BoxV_own(self, p);
    return true;
}
