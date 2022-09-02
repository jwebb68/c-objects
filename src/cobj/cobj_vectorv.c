#include "cobj_vectorv.h"

#include "cobj_carrayv.h"
#include "cobj_memory.h" // STRUCTWIPE

static void VectorV_wipe(VectorV *const self)
{
    STRUCTWIPE(self);
}

void VectorV_destroy_member(VectorV *const self,
                            void *(*elem_ptr_mut)(void *const arr, size_t idx),
                            void (*elem_destroy)(void *const))
{
    CArrayV_destroy(self->ptr, self->len, elem_ptr_mut, elem_destroy);
}
void VectorV_destroy(VectorV *const self,
                     void *(*elem_ptr_mut)(void *const arr, size_t idx),
                     void (*elem_destroy)(void *const))
{
    VectorV_destroy_member(self, elem_ptr_mut, elem_destroy);
    VectorV_wipe(self);
}

void VectorV_move_member(VectorV *const self, VectorV *const src)
{
    *self = *src;
}
void VectorV_move(VectorV *const self, VectorV *const src)
{
    VectorV_move_member(self, src);
    VectorV_wipe(src);
}

void VectorV_new(VectorV *const self, void *const ptr, size_t alloc)
{
    self->ptr = ptr;
    self->alloc = alloc;
    self->len = 0;
}

bool VectorV_is_empty(VectorV const *const self)
{
    return self->len == 0;
}

void VectorV_clear(VectorV *const self,
                   void *(*elem_ptr_mut)(void *const arr, size_t idx),
                   void (*elem_destroy)(void *const))
{
    CArrayV_destroy(self->ptr, self->len, elem_ptr_mut, elem_destroy);
    self->len = 0;
}

size_t VectorV_len(VectorV const *const self)
{
    return self->len;
}

bool WARN_UNUSED_RESULT VectorV_push_back(VectorV *const self,
                                          void *const elem,
                                          void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                          void (*elem_move)(void *const, void *const))
{
    // moves, not copies
    if (self->len >= self->alloc) { return false; }
    void *p = elem_ptr_mut(self->ptr, self->len);
    elem_move(p, elem);
    self->len += 1;
    return true;
}

bool WARN_UNUSED_RESULT VectorV_pop_back(VectorV *const self,
                                         void *const elem,
                                         void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                         void (*elem_move)(void *const, void *const))
{
    if (self->len == 0) { return false; }
    self->len -= 1;
    void *p = elem_ptr_mut(self->ptr, self->len);
    elem_move(elem, p);
    return true;
}

void *VectorV_try_get_mut(VectorV const *const self,
                          Index pos,
                          void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    if (pos >= self->len) { return false; }
    void *p = elem_ptr_mut(self->ptr, pos);
    return p;
}

void const *VectorV_try_get(VectorV const *const self,
                            Index pos,
                            void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    if (pos >= self->len) { return false; }
    void const *p = elem_ptr(self->ptr, pos);
    return p;
}

void VectorV_as_SliceV(VectorV const *const self, SliceV *const s)
{
    SliceV_new(s, self->ptr, self->len);
}

void VectorV_as_SliceVMut(VectorV const *const self, SliceVMut *const s)
{
    SliceVMut_new(s, self->ptr, self->len);
}

void VectorV_iter(VectorV const *const self,
                  SliceVIter *const it,
                  void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    SliceVIter_new(it, self->ptr, elem_ptr(self->ptr, self->len));
}

void VectorV_iter_mut(VectorV const *const self,
                      SliceVMutIter *const it,
                      void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    SliceVMutIter_new(it, self->ptr, elem_ptr_mut(self->ptr, self->len));
}
