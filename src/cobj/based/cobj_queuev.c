#include <cobj/based/cobj_carrayv.h>
#include <cobj/based/cobj_queuev.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE

// =======================================================================

static size_t cobj_QueueV_advance(cobj_QueueV *const self, size_t idx)
{
    idx += 1;
    if (idx >= self->alloc) { idx = 0; }
    return idx;
}

static void cobj_QueueV_elem_destroy(cobj_QueueV *const self,
                                     void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                     void (*elem_destroy_member)(void *const))
{
    if (self->wp > self->rp) {
        // not wrapped
        cobj_CArrayV_erase(self->ptr,
                           self->rp,
                           self->wp - self->rp,
                           elem_ptr_mut,
                           elem_destroy_member);
    } else if (self->wp < self->rp) {
        // wrapped
        cobj_CArrayV_erase(self->ptr,
                           self->rp,
                           self->alloc - self->rp,
                           elem_ptr_mut,
                           elem_destroy_member);
        cobj_CArrayV_erase(self->ptr, 0, self->wp, elem_ptr_mut, elem_destroy_member);
    } else if (self->len != 0) {
        // all allocd
        // cobj_CArrayV_erase(self->ptr, 0, self->alloc, elem_ptr_mut, elem_destroy_member);
        cobj_CArrayV_destroy(self->ptr, self->alloc, elem_ptr_mut, elem_destroy_member);
    } else {
        // none allocd
    }
}

//#############################################################################

static void cobj_QueueV_wipe(cobj_QueueV *const self)
{
    STRUCTWIPE(self);
}

void cobj_QueueV_destroy_member(cobj_QueueV *const self,
                                void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                void (*elem_destroy)(void *const))
{
    cobj_QueueV_elem_destroy(self, elem_ptr_mut, elem_destroy);
}

void cobj_QueueV_destroy(cobj_QueueV *const self,
                         void *(*elem_ptr_mut)(void *const arr, size_t idx),
                         void (*elem_destroy)(void *const))
{
    cobj_QueueV_destroy_member(self, elem_ptr_mut, elem_destroy);
    cobj_QueueV_wipe(self);
}

void cobj_QueueV_move_member(cobj_QueueV *const self, cobj_QueueV *const src)
{
    *self = *src;
}

void cobj_QueueV_move(cobj_QueueV *const self, cobj_QueueV *const src)
{
    cobj_QueueV_move_member(self, src);
    cobj_QueueV_wipe(src);
}

void cobj_QueueV_new(cobj_QueueV *const self, void *const ptr, size_t alloc)
{
    self->ptr = ptr;
    self->alloc = alloc;
    self->len = 0;
    self->wp = 0;
    self->rp = 0;
}

bool cobj_QueueV_is_empty(cobj_QueueV const *const self)
{
    return self->len == 0;
}

static bool cobj_QueueV_is_full(cobj_QueueV const *const self)
{
    return self->len == self->alloc;
}

void cobj_QueueV_clear(cobj_QueueV *const self,
                       void *(*elem_ptr_mut)(void *const arr, size_t idx),
                       void (*elem_destroy_member)(void *const))
{
    cobj_QueueV_elem_destroy(self, elem_ptr_mut, elem_destroy_member);
    self->len = 0;
    self->wp = 0;
    self->rp = 0;
}

size_t cobj_QueueV_len(cobj_QueueV const *const self)
{
    return self->len;
}

size_t cobj_QueueV_alloc(cobj_QueueV const *const self)
{
    return self->alloc;
}

bool WARN_UNUSED_RESULT cobj_QueueV_put(cobj_QueueV *const self,
                                        void *const elem,
                                        void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                        void (*elem_move)(void *const, void *const))
{
    if (cobj_QueueV_is_full(self)) { return false; }
    self->len += 1;
    elem_move(elem_ptr_mut(self->ptr, self->wp), elem);
    self->wp = cobj_QueueV_advance(self, self->wp);
    return true;
}

bool WARN_UNUSED_RESULT cobj_QueueV_get(cobj_QueueV *const self,
                                        void *const elem,
                                        void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                        void (*elem_move)(void *const, void *const))
{
    if (cobj_QueueV_is_empty(self)) { return false; }
    self->len -= 1;
    elem_move(elem, elem_ptr_mut(self->ptr, self->rp));
    self->rp = cobj_QueueV_advance(self, self->rp);
    return true;
}
