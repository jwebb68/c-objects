#include <cobj/based/cobj_boxv.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE
#include <stddef.h> // NULL
#include <stdlib.h> // malloc/free

static void BoxV_wipe(BoxV *const self)
{
    STRUCTWIPE(self);
}

void BoxV_destroy_member(BoxV *const self,
                         void (*elem_destroy)(void *const elem),
                         void (*elem_free)(void *elem))
{
    elem_destroy(self->elem);
    elem_free(self->elem);
}

void BoxV_destroy(BoxV *const self,
                  void (*elem_destroy)(void *const elem),
                  void (*elem_free)(void *elem))
{
    BoxV_destroy_member(self, elem_destroy, elem_free);
    BoxV_wipe(self);
}

void BoxV_move_member(BoxV *const self, BoxV *const src)
{
    *self = *src;
}
void BoxV_move(BoxV *const self, BoxV *const src)
{
    BoxV_move_member(self, src);
    BoxV_wipe(src);
}

bool WARN_UNUSED_RESULT BoxV_try_copy_member(BoxV *const self,
                                             BoxV const *const v,
                                             Error *const err,
                                             void *(*elem_try_alloc)(Error *const err),
                                             void (*elem_free)(void *elem),
                                             bool (*elem_try_copy)(void *const elem,
                                                                   void const *const elem_src,
                                                                   Error *const err))
{
    void *p = elem_try_alloc(err);
    if (p == NULL) { return false; }

    bool ok;
    ok = elem_try_copy(p, BoxV_deref(v), err);
    if (!ok) {
        elem_free(p);
        // no need to wipe, as p not assigned into self
        // BoxV_wipe(self);
        return false;
    }

    BoxV_own(self, p);
    return true;
}

bool WARN_UNUSED_RESULT BoxV_try_copy(BoxV *const self,
                                      BoxV const *const v,
                                      Error *const err,
                                      void *(*elem_try_alloc)(Error *const err),
                                      void (*elem_free)(void *elem),
                                      bool (*elem_try_copy)(void *const elem,
                                                            void const *const elem_src,
                                                            Error *const err))
{
    if (!BoxV_try_copy_member(self, v, err, elem_try_alloc, elem_free, elem_try_copy)) {
        BoxV_wipe(self);
        return false;
    }
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

void BoxV_own(BoxV *const self, void *const p)
{
    self->elem = p;
}
void *BoxV_disown_member(BoxV *const self)
{
    return self->elem;
}
void *BoxV_disown(BoxV *const self)
{
    void *p = BoxV_disown_member(self);
    BoxV_wipe(self);
    return p;
}
void BoxV_swap(BoxV *const self, BoxV *const other)
{
    ptrswap(&self->elem, &other->elem);
}

// void BoxV_new...(BoxV_ *const self, ...) {
//  T_new(&self->elem, ...);
//}

bool WARN_UNUSED_RESULT BoxV_try_from_V(BoxV *const self,
                                        void *const elem,
                                        Error *const err,
                                        void *(*elem_try_alloc)(Error *const err),
                                        void (*elem_move)(void *const elem, void *const src))
{
    void *p = elem_try_alloc(err);
    if (p == NULL) { return false; }
    elem_move(p, elem);
    BoxV_own(self, p);
    return true;
}

void BoxV_into_V(BoxV *const self,
                 void *const elem,
                 void (*elem_move)(void *const elem, void *const src),
                 void (*elem_free)(void *elem))
{
    void *p = BoxV_disown(self);
    elem_move(elem, p);
    elem_free(p);
}

bool WARN_UNUSED_RESULT BoxV_try_copy_V(BoxV *const self,
                                        void const *const elem,
                                        Error *const err,
                                        void *(*elem_try_alloc)(Error *const err),
                                        void (*elem_free)(void *elem),
                                        bool (*elem_try_copy)(void *const elem,
                                                              void const *const src,
                                                              Error *const err))
{
    void *p = elem_try_alloc(err);
    if (p == NULL) { return false; }

    bool ok;
    ok = elem_try_copy(p, elem, err);
    if (!ok) {
        elem_free(p);
        // BoxV_wipe(self);
        return false;
    }
    BoxV_own(self, p);
    return true;
}
