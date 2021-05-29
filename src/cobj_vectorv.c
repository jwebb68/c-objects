#include "cobj_vectorv.h"

#include "cobj_memory.h" // STRUCTWIPE

static void
VectorV_destroy_elems(VectorV *const self, size_t elem_size, void (*elem_destroy)(void *const))
{
    for (uint8_t *it = self->buf; it != self->buf_pos; it += elem_size) {
        elem_destroy(it);
    }
}

static void VectorV_wipe(VectorV *const self)
{
    STRUCTWIPE(self);
}

void VectorV_destroy(VectorV *const self, size_t elem_size, void (*elem_destroy)(void *const))
{
    VectorV_destroy_elems(self, elem_size, elem_destroy);
    VectorV_wipe(self);
}

void VectorV_move(VectorV *const self, VectorV *const src)
{
    *self = *src;
    VectorV_wipe(src);
}

void VectorV_new(VectorV *const self, uint8_t *const arr, uint8_t *arr_e)
{
    self->buf = arr;
    self->buf_end = arr_e;
    self->buf_pos = arr;
}

bool VectorV_is_empty(VectorV const *const self)
{
    return self->buf_pos == self->buf;
}

void VectorV_clear(VectorV *const self, size_t elem_size, void (*elem_destroy)(void *const))
{
    VectorV_destroy_elems(self, elem_size, elem_destroy);
    self->buf_pos = self->buf;
}

size_t VectorV_len(VectorV const *self, size_t elem_size)
{
    return (self->buf_pos - self->buf) / elem_size;
}

bool WARN_UNUSED_RESULT VectorV_push_back(VectorV *self,
                                          void *elem,
                                          size_t elem_size,
                                          void (*elem_move)(void *const, void *const))
{
    // moves, not copies
    if (self->buf_pos >= self->buf_end) {
        return false;
    }
    elem_move(self->buf_pos, elem);
    self->buf_pos += elem_size;
    return true;
}

bool WARN_UNUSED_RESULT VectorV_pop_back(VectorV *self,
                                         void *elem,
                                         size_t elem_size,
                                         void (*elem_move)(void *const, void *const))
{
    // moves, not copies
    if (self->buf_pos == self->buf) {
        return false;
    }
    self->buf_pos -= elem_size;
    elem_move(elem, self->buf_pos);
    return true;
}

void *VectorV_get_item_at_mut(VectorV *self, Index pos, size_t elem_size)
{
    // null if no item at pos
    uint8_t *p = self->buf + (pos * elem_size);
    if (p >= self->buf_end) {
        return NULL;
    }
    return p;
}

void const *VectorV_get_item_at(VectorV *self, Index pos, size_t elem_size)
{
    // null if no item at pos
    uint8_t *p = self->buf + (pos * elem_size);
    if (p >= self->buf_end) {
        return NULL;
    }
    return p;
}

void VectorV_iter(VectorV const *const self, VectorVIter *const it)
{
    VectorVIter_new(it, self);
}

void VectorV_iter_mut(VectorV *const self, VectorVIterMut *const it)
{
    VectorVIterMut_new(it, self);
}

// ==========================================================================

static void VectorVIter_wipe(VectorVIter *const self)
{
    STRUCTWIPE(self);
}

void VectorVIter_destroy(VectorVIter *const self)
{
    VectorVIter_wipe(self);
}

void VectorVIter_move(VectorVIter *const self, VectorVIter *const src)
{
    *self = *src;
    VectorVIter_wipe(src);
}

void VectorVIter_new(VectorVIter *const self, VectorV const *const vec)
{
    self->vec = vec;
    self->it = vec->buf;
}

void const *WARN_UNUSED_RESULT VectorVIter_next(VectorVIter *const self, size_t elem_size)
{
    if (self->it >= self->vec->buf_pos) {
        return NULL;
    }
    self->it += elem_size;
    return self->it;
}

// ==========================================================================

static void VectorVIterMut_wipe(VectorVIterMut *const self)
{
    STRUCTWIPE(self);
}

void VectorVIterMut_destroy(VectorVIterMut *const self)
{
    VectorVIterMut_wipe(self);
}

void VectorVIterMut_move(VectorVIterMut *const self, VectorVIterMut *const src)
{
    *self = *src;
    VectorVIterMut_wipe(src);
}

void VectorVIterMut_new(VectorVIterMut *const self, VectorV *const vec)
{
    self->vec = vec;
    self->it = vec->buf;
}

void *WARN_UNUSED_RESULT VectorVIterMut_next(VectorVIterMut *const self, size_t elem_size)
{
    if (self->it >= self->vec->buf_pos) {
        return NULL;
    }
    self->it += elem_size;
    return self->it;
}
