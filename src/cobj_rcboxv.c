#include "cobj_rcboxv.h"

#include "cobj_error.h"
#include "cobj_memory.h" // STRUCTWIPE

#include <inttypes.h> // uint8_t
#include <stdlib.h> // malloc/free

// shared internal node struct between box and RCBox
// RCNode or SharedBox
// vs AutoBox/UniqueBox ?

typedef struct RCNodeV_ RCNodeV;
struct RCNodeV_ {
    size_t alloc;
    size_t rc;
    uint8_t val[];
};

void RCNodeV_wipe(RCNodeV *const self)
{
    memwipe(self, sizeof(size_t) + self->alloc);
}

void RCNodeV_destroy(RCNodeV *const self, void (*elem_destroy)(void *const elem))
{
    elem_destroy(self->val);
    RCNodeV_wipe(self);
}

void RCNodeV_grab(RCNodeV *const self)
{
    self->rc += 1;
}

bool WARN_UNUSED_RESULT RCNodeV_release(RCNodeV *const self)
{
    self->rc -= 1;
    return (self->rc != 0);
}

void const *RCNodeV_deref(RCNodeV const *const self)
{
    return self->val;
}

void *RCNodeV_deref_mut(RCNodeV *const self)
{
    return self->val;
}

RCNodeV *RCNodeV_malloc(size_t elem_size, Error *const err)
{
    size_t size = sizeof(size_t) + sizeof(size_t) + elem_size;
    RCNodeV *p = malloc(size);
    if (p == NULL) {
        bool ok;
        ok = ERROR_RAISE(err, Error_ENOMEM);
        UNUSED_ARG(ok);
        return p;
    }
    return p;
}

// ==========================================================================

static void RCBoxV_wipe(RCBoxV *const self)
{
    STRUCTWIPE(self);
}

void RCBoxV_destroy(RCBoxV *const self, void (*elem_destroy)(void *const elem))
{
    if (!RCNodeV_release(self->node)) {
        RCNodeV_destroy(self->node, elem_destroy);
    }
    RCBoxV_wipe(self);
}

void RCBoxV_move(RCBoxV *const self, RCBoxV *const src)
{
    *self = *src;
    RCBoxV_wipe(src);
}

bool RCBoxV_try_copy(RCBoxV *const self, RCBoxV const *const src, Error *const err)
{
    UNUSED_ARG(err);
    *self = *src;
    RCNodeV_grab(self->node);
    return true;
}

void const *RCBoxV_deref(RCBoxV const *const self)
{
    return RCNodeV_deref(self->node);
}

void *RCBoxV_deref_mut(RCBoxV *const self)
{
    return RCNodeV_deref_mut(self->node);
}

bool WARN_UNUSED_RESULT RCBoxV_try_new_from(RCBoxV *const self,
                                            void *const v,
                                            Error *const err,
                                            size_t elem_size,
                                            void (*elem_move)(void *const self, void *const src))
{
    RCNodeV *p = RCNodeV_malloc(elem_size, err);
    if (p == NULL) {
        return p;
    }

    elem_move(RCNodeV_deref_mut(p), v);

    RCNodeV_grab(p);
    self->node = p;
    return true;
}

bool WARN_UNUSED_RESULT RCBoxV_try_new_copy(RCBoxV *const self,
                                            T const *const v,
                                            Error *const err,
                                            size_t elem_size,
                                            bool (*elem_try_copy)(void *const elem,
                                                                  void const *const src,
                                                                  Error *const err))
{
    RCNodeV *p = RCNodeV_malloc(elem_size, err);
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    bool ok;
    ok = elem_try_copy(RCNodeV_deref_mut(p), v, err);
    if (!ok) {
        // no destroy as node is not initialised and destroy wants it
        // initialised.
        free(p);
        return false;
    }

    RCNodeV_grab(p);
    self->node = p;
    return true;
}

// no own of ptr: that leads to heap fragmentation
// no own of value: that's a new_default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.
