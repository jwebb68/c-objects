#include "cobj_queuev.h"

#include "cobj_carrayv.h"
#include "cobj_memory.h" // STRUCTWIPE

// =======================================================================

static size_t QueueV_advance(QueueV *const self, size_t idx)
{
    idx += 1;
    if (idx >= self->alloc) { idx = 0; }
    return idx;
}

static void QueueV_elem_destroy(QueueV *const self,
                                void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                void (*elem_destroy_member)(void *const))
{
    if (self->wp > self->rp) {
        // not wrapped
        CArrayV_erase(self->ptr, self->rp, self->wp - self->rp, elem_ptr_mut, elem_destroy_member);
    } else if (self->wp < self->rp) {
        // wrapped
        CArrayV_erase(self->ptr,
                      self->rp,
                      self->alloc - self->rp,
                      elem_ptr_mut,
                      elem_destroy_member);
        CArrayV_erase(self->ptr, 0, self->wp, elem_ptr_mut, elem_destroy_member);
    } else if (self->len != 0) {
        // all allocd
        // CArrayV_erase(self->ptr, 0, self->alloc, elem_ptr_mut, elem_destroy_member);
        CArrayV_destroy(self->ptr, self->alloc, elem_ptr_mut, elem_destroy_member);
    } else {
        // none allocd
    }
}

//#############################################################################

static void QueueV_wipe(QueueV *const self)
{
    STRUCTWIPE(self);
}

void QueueV_destroy_member(QueueV *const self,
                           void *(*elem_ptr_mut)(void *const arr, size_t idx),
                           void (*elem_destroy)(void *const))
{
    QueueV_elem_destroy(self, elem_ptr_mut, elem_destroy);
}

void QueueV_destroy(QueueV *const self,
                    void *(*elem_ptr_mut)(void *const arr, size_t idx),
                    void (*elem_destroy)(void *const))
{
    QueueV_destroy_member(self, elem_ptr_mut, elem_destroy);
    QueueV_wipe(self);
}

void QueueV_move_member(QueueV *const self, QueueV *const src)
{
    *self = *src;
}

void QueueV_move(QueueV *const self, QueueV *const src)
{
    QueueV_move_member(self, src);
    QueueV_wipe(src);
}

void QueueV_new(QueueV *const self, void *const ptr, size_t alloc)
{
    self->ptr = ptr;
    self->alloc = alloc;
    self->len = 0;
    self->wp = 0;
    self->rp = 0;
}

bool QueueV_is_empty(QueueV const *const self)
{
    return self->len == 0;
}

static bool QueueV_is_full(QueueV const *const self)
{
    return self->len == self->alloc;
}

void QueueV_clear(QueueV *const self,
                  void *(*elem_ptr_mut)(void *const arr, size_t idx),
                  void (*elem_destroy_member)(void *const))
{
    QueueV_elem_destroy(self, elem_ptr_mut, elem_destroy_member);
    self->len = 0;
    self->wp = 0;
    self->rp = 0;
}

size_t QueueV_len(QueueV const *const self)
{
    return self->len;
}

size_t QueueV_alloc(QueueV const *const self)
{
    return self->alloc;
}

bool WARN_UNUSED_RESULT QueueV_put(QueueV *const self,
                                   void *const elem,
                                   void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                   void (*elem_move)(void *const, void *const))
{
    if (QueueV_is_full(self)) { return false; }
    self->len += 1;
    elem_move(elem_ptr_mut(self->ptr, self->wp), elem);
    self->wp = QueueV_advance(self, self->wp);
    return true;
}

bool WARN_UNUSED_RESULT QueueV_get(QueueV *const self,
                                   void *const elem,
                                   void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                   void (*elem_move)(void *const, void *const))
{
    if (QueueV_is_empty(self)) { return false; }
    self->len -= 1;
    elem_move(elem, elem_ptr_mut(self->ptr, self->rp));
    self->rp = QueueV_advance(self, self->rp);
    return true;
}
