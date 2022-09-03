#include <cobj/based/cobj_rcboxv.h>
#include <cobj/core/cobj_error.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE
#include <inttypes.h> // uint8_t
#include <stdlib.h> // malloc/free

// shared internal node struct between box and RCBox
// RCNode or SharedBox
// vs AutoBox/UniqueBox ?

typedef struct cobj_RCNodeV_s cobj_RCNodeV;
struct cobj_RCNodeV_s {
    size_t alloc;
    size_t rc;
    uint8_t val[];
};

void cobj_RCNodeV_wipe(cobj_RCNodeV *const self)
{
    memwipe(self, sizeof(size_t) + self->alloc);
}

void cobj_RCNodeV_destroy(cobj_RCNodeV *const self, void (*elem_destroy)(void *const elem))
{
    elem_destroy(self->val);
    cobj_RCNodeV_wipe(self);
}

void cobj_RCNodeV_grab(cobj_RCNodeV *const self)
{
    self->rc += 1;
}

size_t WARN_UNUSED_RESULT cobj_RCNodeV_release(cobj_RCNodeV *const self)
{
    self->rc -= 1;
    return self->rc;
}

void const *cobj_RCNodeV_deref(cobj_RCNodeV const *const self)
{
    return self->val;
}

void *cobj_RCNodeV_deref_mut(cobj_RCNodeV *const self)
{
    return self->val;
}

static cobj_RCNodeV *cobj_RCNodeV_cobj_try_malloc(size_t elem_size, cobj_Error *const err)
{
    size_t size = sizeof(size_t) + sizeof(size_t) + elem_size;
    return cobj_try_malloc(size, err);
}

// ==========================================================================

static void cobj_RCBoxV_wipe(cobj_RCBoxV *const self)
{
    STRUCTWIPE(self);
}

void cobj_RCBoxV_destroy_member(cobj_RCBoxV *const self, void (*elem_destroy)(void *const elem))
{
    if (cobj_RCNodeV_release(self->node) == 0) { cobj_RCNodeV_destroy(self->node, elem_destroy); }
    cobj_RCBoxV_wipe(self);
}

void cobj_RCBoxV_destroy(cobj_RCBoxV *const self, void (*elem_destroy)(void *const elem))
{
    cobj_RCBoxV_destroy_member(self, elem_destroy);
    cobj_RCBoxV_wipe(self);
}

void cobj_RCBoxV_move_member(cobj_RCBoxV *const self, cobj_RCBoxV *const src)
{
    *self = *src;
}

void cobj_RCBoxV_move(cobj_RCBoxV *const self, cobj_RCBoxV *const src)
{
    cobj_RCBoxV_move_member(self, src);
    cobj_RCBoxV_wipe(src);
}

bool cobj_RCBoxV_try_copy(cobj_RCBoxV *const self,
                          cobj_RCBoxV const *const src,
                          cobj_Error *const err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    cobj_RCNodeV_grab(self->node);
    return true;
}

void const *cobj_RCBoxV_deref(cobj_RCBoxV const *const self)
{
    return cobj_RCNodeV_deref(self->node);
}

void *cobj_RCBoxV_deref_mut(cobj_RCBoxV *const self)
{
    return cobj_RCNodeV_deref_mut(self->node);
}

bool WARN_UNUSED_RESULT cobj_RCBoxV_try_from_V(cobj_RCBoxV *const self,
                                               void *const v,
                                               cobj_Error *const err,
                                               size_t elem_size,
                                               void (*elem_move)(void *const self, void *const src))
{
    cobj_RCNodeV *p = cobj_RCNodeV_cobj_try_malloc(elem_size, err);
    if (p == NULL) { return false; }

    elem_move(cobj_RCNodeV_deref_mut(p), v);

    cobj_RCNodeV_grab(p);
    self->node = p;
    return true;
}

bool WARN_UNUSED_RESULT cobj_RCBoxV_try_copy_V(cobj_RCBoxV *const self,
                                               void const *const v,
                                               cobj_Error *const err,
                                               size_t elem_size,
                                               bool (*elem_try_copy)(void *const elem,
                                                                     void const *const src,
                                                                     cobj_Error *const err))
{
    cobj_RCNodeV *p = cobj_RCNodeV_cobj_try_malloc(elem_size, err);
    if (p == NULL) { return false; }

    bool ok;
    ok = elem_try_copy(cobj_RCNodeV_deref_mut(p), v, err);
    if (!ok) {
        // no destroy as node is not initialised and destroy wants it
        // initialised.
        free(p);
        return false;
    }

    cobj_RCNodeV_grab(p);
    self->node = p;
    return true;
}

// no own of ptr: that leads to heap fragmentation
// no own of value: that's a default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.
