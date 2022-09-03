#include <cobj/baseless/cobj_carrayt.h>
#include <cobj/baseless/cobj_queuet.h>
#include <cobj/core/cobj_memory.h>

static size_t cobj_QueueT_advance(cobj_QueueT *const self, size_t idx)
{
    idx += 1;
    if (idx >= self->alloc) { idx = 0; }
    return idx;
}

static void cobj_QueueT_elem_destroy(cobj_QueueT *const self)
{
    if (self->wp > self->rp) {
        // not wrapped
        cobj_CArrayTMut_erase(self->ptr, self->rp, self->wp - self->rp);
    } else if (self->wp < self->rp) {
        // wrapped
        cobj_CArrayTMut_erase(self->ptr, self->rp, self->alloc - self->rp);
        cobj_CArrayTMut_erase(self->ptr, 0, self->wp);
    } else if (self->len != 0) {
        // all allocd
        // cobj_CArrayTMut_erase(self->ptr, 0, self->alloc);
        cobj_CArrayTMut_destroy(self->ptr, self->alloc);
    } else {
        // none allocd
    }
}

static void cobj_QueueT_wipe(cobj_QueueT *const self)
{
    STRUCTWIPE(self);
}

void cobj_QueueT_destroy_member(cobj_QueueT *const self)
{
    cobj_QueueT_elem_destroy(self);
}
void cobj_QueueT_destroy(cobj_QueueT *const self)
{
    cobj_QueueT_destroy_member(self);
    cobj_QueueT_wipe(self);
}

void cobj_QueueT_move_member(cobj_QueueT *const self, cobj_QueueT *const src)
{
    *self = *src;
}
void cobj_QueueT_move(cobj_QueueT *const self, cobj_QueueT *const src)
{
    cobj_QueueT_move_member(self, src);
    cobj_QueueT_wipe(src);
}

void cobj_QueueT_new(cobj_QueueT *const self, T *const ptr, size_t alloc)
{
    self->ptr = ptr;
    self->alloc = alloc;
    self->len = 0;
    self->wp = 0;
    self->rp = 0;
}

void cobj_QueueT_clear(cobj_QueueT *const self)
{
    cobj_QueueT_elem_destroy(self);
    self->len = 0;
    self->rp = 0;
    self->wp = 0;
}

bool cobj_QueueT_is_empty(cobj_QueueT const *const self)
{
    return self->len == 0;
}

static bool cobj_QueueT_is_full(cobj_QueueT const *const self)
{
    return self->len == self->alloc;
}

size_t cobj_QueueT_len(cobj_QueueT const *const self)
{
    return self->len;
}

size_t cobj_QueueT_alloc(cobj_QueueT const *const self)
{
    return self->alloc;
}

bool WARN_UNUSED_RESULT cobj_QueueT_put(cobj_QueueT *const self, T *const item)
{
    if (cobj_QueueT_is_full(self)) { return false; }
    self->len += 1;
    T_move(&self->ptr[self->wp], item);
    self->wp = cobj_QueueT_advance(self, self->wp);
    return true;
}

bool WARN_UNUSED_RESULT cobj_QueueT_get(cobj_QueueT *const self, T *const item)
{
    if (cobj_QueueT_is_empty(self)) { return false; }
    self->len -= 1;
    T_move(item, &self->ptr[self->rp]);
    self->rp = cobj_QueueT_advance(self, self->rp);
    return true;
}
