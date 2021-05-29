#include "cobj_rcboxt.h"

#include "cobj_memory.h" // STRUCTWIPE

#include <stdlib.h> // malloc/free

// shared internal node struct between box and RCBox
// RCNode or SharedBox
// vs AutoBox/UniqueBox ?

typedef struct RCNodeT_ RCNodeT;
struct RCNodeT_ {
    T val;
    size_t rc;
};

void RCNodeT_wipe(RCNodeT *const self)
{
    STRUCTWIPE(self);
}

void RCNodeT_destroy(RCNodeT *const self)
{
    T_destroy(&self->val);
    RCNodeT_wipe(self);
}

void RCNodeT_grab(RCNodeT *const self)
{
    self->rc += 1;
}

bool RCNodeT_release(RCNodeT *const self)
{
    self->rc -= 1;
    return (self->rc != 0);
}

T const *RCNodeT_deref(RCNodeT const *const self)
{
    return &self->val;
}

T *RCNodeT_deref_mut(RCNodeT *const self)
{
    return &self->val;
}

RCNodeT *RCNodeT_malloc(void)
{
    RCNodeT *p = malloc(sizeof(RCNodeT));
    return p;
}

void RCNodeT_new_int(RCNodeT *const self, int v)
{
    self->rc = 0;
    T_new_int(&self->val, v);
}

void RCNodeT_new_from_T(RCNodeT *const self, T *const src)
{
    self->rc = 0;
    T_move(&self->val, src);
}

bool RCNodeT_try_new_copy_T(RCNodeT *const self, T const *const src, Error *const err)
{
    self->rc = 0;
    return T_try_copy(&self->val, src, err);
}

// ==========================================================================

static void RCBoxT_wipe(RCBoxT *const self)
{
    STRUCTWIPE(self);
}

void RCBoxT_destroy(RCBoxT *const self)
{
    if (!RCNodeT_release(self->node)) {
        RCNodeT_destroy(self->node);
    }
    RCBoxT_wipe(self);
}

void RCBoxT_move(RCBoxT *const self, RCBoxT *const src)
{
    *self = *src;
    RCBoxT_wipe(src);
}

bool RCBoxT_try_copy(RCBoxT *const self, RCBoxT const *const src)
{
    *self = *src;
    RCNodeT_grab(self->node);
    return true;
}

T const *RCBoxT_deref(RCBoxT const *const self)
{
    return RCNodeT_deref(self->node);
}

T *RCBoxT_deref_mut(RCBoxT *const self)
{
    return RCNodeT_deref_mut(self->node);
}

bool WARN_UNUSED_RESULT RCBoxT_try_new_int(RCBoxT *const self, int v, Error *const err)
{
    RCNodeT *p = RCNodeT_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    RCNodeT_new_int(p, v);
    RCNodeT_grab(p);
    self->node = p;
    return true;
}

bool WARN_UNUSED_RESULT RCBoxT_try_new_from_T(RCBoxT *const self, T *const v, Error *const err)
{
    RCNodeT *p = RCNodeT_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    T_move(RCNodeT_deref_mut(p), v);
    RCNodeT_grab(p);
    self->node = p;
    return true;
}

// new_copy variant? copy direct into dest without intermed storage?
bool WARN_UNUSED_RESULT RCBoxT_try_new_copy_T(RCBoxT *const self,
                                              T const *const v,
                                              Error *const err)
{
    RCNodeT *p = RCNodeT_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }

    bool ok;
    ok = T_try_copy(RCNodeT_deref_mut(p), v, err);
    if (!ok) {
        free(p);
        return false;
    }

    RCNodeT_grab(p);
    self->node = p;
    return true;
}

bool RCBoxT_is_eq(RCBoxT const *const self, RCBoxT const *const b)
{
    return T_is_eq(RCBoxT_deref(self), RCBoxT_deref(b));
}

bool RCBoxT_is_lt(RCBoxT const *const self, RCBoxT const *const b)
{
    return T_is_lt(RCBoxT_deref(self), RCBoxT_deref(b));
}

bool RCBoxT_is_gt(RCBoxT const *const self, RCBoxT const *const b)
{
    return T_is_gt(RCBoxT_deref(self), RCBoxT_deref(b));
}

// no own of just a ptr: that leads to heap fragmentation
// no own of value: that's a new_default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.
