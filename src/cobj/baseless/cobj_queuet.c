#include <cobj/baseless/cobj_carrayt.h>
#include <cobj/baseless/cobj_queuet.h>
#include <cobj/core/cobj_memory.h>

static size_t QueueT_advance(QueueT *const self, size_t idx)
{
    idx += 1;
    if (idx >= self->alloc) { idx = 0; }
    return idx;
}

static void QueueT_elem_destroy(QueueT *const self)
{
    if (self->wp > self->rp) {
        // not wrapped
        CArrayTMut_erase(self->ptr, self->rp, self->wp - self->rp);
    } else if (self->wp < self->rp) {
        // wrapped
        CArrayTMut_erase(self->ptr, self->rp, self->alloc - self->rp);
        CArrayTMut_erase(self->ptr, 0, self->wp);
    } else if (self->len != 0) {
        // all allocd
        // CArrayTMut_erase(self->ptr, 0, self->alloc);
        CArrayTMut_destroy(self->ptr, self->alloc);
    } else {
        // none allocd
    }
}

static void QueueT_wipe(QueueT *const self)
{
    STRUCTWIPE(self);
}

void QueueT_destroy_member(QueueT *const self)
{
    QueueT_elem_destroy(self);
}
void QueueT_destroy(QueueT *const self)
{
    QueueT_destroy_member(self);
    QueueT_wipe(self);
}

void QueueT_move_member(QueueT *const self, QueueT *const src)
{
    *self = *src;
}
void QueueT_move(QueueT *const self, QueueT *const src)
{
    QueueT_move_member(self, src);
    QueueT_wipe(src);
}

void QueueT_new(QueueT *const self, T *const ptr, size_t alloc)
{
    self->ptr = ptr;
    self->alloc = alloc;
    self->len = 0;
    self->wp = 0;
    self->rp = 0;
}

void QueueT_clear(QueueT *const self)
{
    QueueT_elem_destroy(self);
    self->len = 0;
    self->rp = 0;
    self->wp = 0;
}

bool QueueT_is_empty(QueueT const *const self)
{
    return self->len == 0;
}

static bool QueueT_is_full(QueueT const *const self)
{
    return self->len == self->alloc;
}

size_t QueueT_len(QueueT const *const self)
{
    return self->len;
}

size_t QueueT_alloc(QueueT const *const self)
{
    return self->alloc;
}

bool WARN_UNUSED_RESULT QueueT_put(QueueT *const self, T *const item)
{
    if (QueueT_is_full(self)) { return false; }
    self->len += 1;
    T_move(&self->ptr[self->wp], item);
    self->wp = QueueT_advance(self, self->wp);
    return true;
}

bool WARN_UNUSED_RESULT QueueT_get(QueueT *const self, T *const item)
{
    if (QueueT_is_empty(self)) { return false; }
    self->len -= 1;
    T_move(item, &self->ptr[self->rp]);
    self->rp = QueueT_advance(self, self->rp);
    return true;
}
