#include "cobj_vectorv.h"

#include "cobj_memory.h" // STRUCTWIPE

static void
VectorV_destroy_elems(VectorV *const self, size_t elem_size, void (*elem_destroy)(void *const))
{
    for (uint8_t *it = self->b; it != self->p; it += elem_size) {
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

void VectorV_new(VectorV *const self, uint8_t *const b, uint8_t *const e)
{
    self->b = b;
    self->e = e;
    self->p = b;
}

bool VectorV_is_empty(VectorV const *const self)
{
    return self->p == self->b;
}

void VectorV_clear(VectorV *const self, size_t elem_size, void (*elem_destroy)(void *const))
{
    VectorV_destroy_elems(self, elem_size, elem_destroy);
    self->p = self->b;
}

size_t VectorV_len(VectorV const *const self, size_t elem_size)
{
    return (self->p - self->b) / elem_size;
}

bool WARN_UNUSED_RESULT VectorV_push_back(VectorV *const self,
                                          void *const elem,
                                          size_t elem_size,
                                          void (*elem_move)(void *const, void *const))
{
    // moves, not copies
    if (self->p >= self->e) {
        return false;
    }
    elem_move(self->p, elem);
    self->p += elem_size;
    return true;
}

bool WARN_UNUSED_RESULT VectorV_pop_back(VectorV *const self,
                                         void *const elem,
                                         size_t elem_size,
                                         void (*elem_move)(void *const, void *const))
{
    // moves, not copies
    if (self->p == self->b) {
        return false;
    }
    self->p -= elem_size;
    elem_move(elem, self->p);
    return true;
}

void *VectorV_get_item_at_mut(VectorV const *const self, Index pos, size_t elem_size)
{
    // null if no item at pos
    uint8_t *p = self->p + (pos * elem_size);
    if (p >= self->e) {
        return NULL;
    }
    return p;
}

void const *VectorV_get_item_at(VectorV const *const self, Index pos, size_t elem_size)
{
    // null if no item at pos
    uint8_t *p = self->b + (pos * elem_size);
    if (p >= self->e) {
        return NULL;
    }
    return p;
}

void VectorV_as_slice(VectorV const *const self, SliceV *const s)
{
    SliceV_new(s, self->b, self->p);
}

void VectorV_as_slice_mut(VectorV const *const self, SliceVMut *const s)
{
    SliceVMut_new(s, self->b, self->p);
}

void VectorV_iter(VectorV const *const self, SliceVIter *const it)
{
    SliceVIter_new(it, self->b, self->p);
}

void VectorV_iter_mut(VectorV const *const self, SliceVMutIter *const it)
{
    SliceVMutIter_new(it, self->b, self->p);
}
