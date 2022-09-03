#include <cobj/baseless/cobj_rcboxt.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE
#include <stdlib.h> // malloc/free

// shared internal node struct between box and RCBox
// RCNode or SharedBox
// vs AutoBox/UniqueBox ?

typedef struct cobj_RCNodeT_s cobj_RCNodeT;
struct cobj_RCNodeT_s {
    T val;
    size_t rc;
};

void cobj_RCNodeT_wipe(cobj_RCNodeT *const self)
{
    STRUCTWIPE(self);
}

void cobj_RCNodeT_destroy(cobj_RCNodeT *const self)
{
    T_destroy(&self->val);
    cobj_RCNodeT_wipe(self);
}

void cobj_RCNodeT_grab(cobj_RCNodeT *const self)
{
    self->rc += 1;
}

size_t cobj_RCNodeT_release(cobj_RCNodeT *const self)
{
    self->rc -= 1;
    return self->rc;
}

T const *cobj_RCNodeT_deref(cobj_RCNodeT const *const self)
{
    return &self->val;
}

T *cobj_RCNodeT_deref_mut(cobj_RCNodeT *const self)
{
    return &self->val;
}

static cobj_RCNodeT *cobj_RCNodeT_cobj_try_malloc(cobj_Error *const err)
{
    return cobj_try_malloc(sizeof(cobj_RCNodeT), err);
}

void cobj_RCNodeT_new(cobj_RCNodeT *const self, int v)
{
    self->rc = 0;
    T_new(&self->val, v);
}

void cobj_RCNodeT_from_T(cobj_RCNodeT *const self, T *const src)
{
    self->rc = 0;
    T_move(&self->val, src);
}

bool cobj_RCNodeT_try_copy_T(cobj_RCNodeT *const self, T const *const src, cobj_Error *const err)
{
    self->rc = 0;
    return T_try_copy(&self->val, src, err);
}

// ==========================================================================

static void cobj_RCBoxT_wipe(cobj_RCBoxT *const self)
{
    STRUCTWIPE(self);
}

void cobj_RCBoxT_destroy_member(cobj_RCBoxT *const self)
{
    if (cobj_RCNodeT_release(self->node) == 0) { cobj_RCNodeT_destroy(self->node); }
    cobj_RCBoxT_wipe(self);
}
void cobj_RCBoxT_destroy(cobj_RCBoxT *const self)
{
    cobj_RCBoxT_destroy_member(self);
    cobj_RCBoxT_wipe(self);
}

void cobj_RCBoxT_move_member(cobj_RCBoxT *const self, cobj_RCBoxT *const src)
{
    *self = *src;
}

void cobj_RCBoxT_move(cobj_RCBoxT *const self, cobj_RCBoxT *const src)
{
    cobj_RCBoxT_move_member(self, src);
    cobj_RCBoxT_wipe(src);
}

bool cobj_RCBoxT_try_copy(cobj_RCBoxT *const self, cobj_RCBoxT const *const src)
{
    *self = *src;
    cobj_RCNodeT_grab(self->node);
    return true;
}

T const *cobj_RCBoxT_deref(cobj_RCBoxT const *const self)
{
    return cobj_RCNodeT_deref(self->node);
}

T *cobj_RCBoxT_deref_mut(cobj_RCBoxT *const self)
{
    return cobj_RCNodeT_deref_mut(self->node);
}

#if 0
bool WARN_UNUSED_RESULT cobj_RCBoxT_try_new(cobj_RCBoxT *const self, int v, cobj_Error *const err)
{
    cobj_RCNodeT *p = cobj_RCNodeT_cobj_try_malloc(err);
    if (p == NULL) { return false; }

    cobj_RCNodeT_new(p, v);
    cobj_RCNodeT_grab(p);
    self->node = p;
    return true;
}
#endif

bool WARN_UNUSED_RESULT cobj_RCBoxT_try_from_T(cobj_RCBoxT *const self,
                                               T *const v,
                                               cobj_Error *const err)
{
    cobj_RCNodeT *p = cobj_RCNodeT_cobj_try_malloc(err);
    if (p == NULL) { return false; }

    T_move(cobj_RCNodeT_deref_mut(p), v);
    cobj_RCNodeT_grab(p);
    self->node = p;
    return true;
}

// copy variant: copy direct into dest without intermed storage?
bool WARN_UNUSED_RESULT cobj_RCBoxT_try_copy_T(cobj_RCBoxT *const self,
                                               T const *const v,
                                               cobj_Error *const err)
{
    cobj_RCNodeT *p = cobj_RCNodeT_cobj_try_malloc(err);
    if (p == NULL) { return false; }

    bool ok;
    ok = T_try_copy(cobj_RCNodeT_deref_mut(p), v, err);
    if (!ok) {
        free(p);
        return false;
    }

    cobj_RCNodeT_grab(p);
    self->node = p;
    return true;
}

bool cobj_RCBoxT_is_eq(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b)
{
    return T_is_eq(cobj_RCNodeT_deref(self->node), cobj_RCNodeT_deref(b->node));
}

bool cobj_RCBoxT_is_lt(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b)
{
    return T_is_lt(cobj_RCNodeT_deref(self->node), cobj_RCNodeT_deref(b->node));
}

bool cobj_RCBoxT_is_gt(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b)
{
    return T_is_gt(cobj_RCNodeT_deref(self->node), cobj_RCNodeT_deref(b->node));
}

bool cobj_RCBoxT_is_le(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b)
{
    return T_is_le(cobj_RCNodeT_deref(self->node), cobj_RCNodeT_deref(b->node));
}

bool cobj_RCBoxT_is_ge(cobj_RCBoxT const *const self, cobj_RCBoxT const *const b)
{
    return T_is_ge(cobj_RCNodeT_deref(self->node), cobj_RCNodeT_deref(b->node));
}

bool cobj_RCBoxT_contains(cobj_RCBoxT const *const self, T const *const v)
{
    return T_is_eq(cobj_RCNodeT_deref(self->node), v);
}

// no own of just a ptr: that leads to heap fragmentation
// no own of value: that's a default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.
