#include <cobj/based/cobj_carrayv.h>
#include <cobj/based/cobj_vectorv.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE

static void cobj_VectorV_wipe(cobj_VectorV *const self)
{
    STRUCTWIPE(self);
}

void cobj_VectorV_destroy_member(cobj_VectorV *const self,
                                 void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                 void (*elem_destroy)(void *const))
{
    cobj_CArrayV_destroy(self->ptr, self->len, elem_ptr_mut, elem_destroy);
}
void cobj_VectorV_destroy(cobj_VectorV *const self,
                          void *(*elem_ptr_mut)(void *const arr, size_t idx),
                          void (*elem_destroy)(void *const))
{
    cobj_VectorV_destroy_member(self, elem_ptr_mut, elem_destroy);
    cobj_VectorV_wipe(self);
}

void cobj_VectorV_move_member(cobj_VectorV *const self, cobj_VectorV *const src)
{
    *self = *src;
}
void cobj_VectorV_move(cobj_VectorV *const self, cobj_VectorV *const src)
{
    cobj_VectorV_move_member(self, src);
    cobj_VectorV_wipe(src);
}

void cobj_VectorV_new(cobj_VectorV *const self, void *const ptr, size_t alloc)
{
    self->ptr = ptr;
    self->alloc = alloc;
    self->len = 0;
}

bool cobj_VectorV_is_empty(cobj_VectorV const *const self)
{
    return self->len == 0;
}

void cobj_VectorV_clear(cobj_VectorV *const self,
                        void *(*elem_ptr_mut)(void *const arr, size_t idx),
                        void (*elem_destroy)(void *const))
{
    cobj_CArrayV_destroy(self->ptr, self->len, elem_ptr_mut, elem_destroy);
    self->len = 0;
}

size_t cobj_VectorV_len(cobj_VectorV const *const self)
{
    return self->len;
}

bool WARN_UNUSED_RESULT cobj_VectorV_push_back(cobj_VectorV *const self,
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

bool WARN_UNUSED_RESULT cobj_VectorV_pop_back(cobj_VectorV *const self,
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

void *cobj_VectorV_try_get_mut(cobj_VectorV const *const self,
                               cobj_Index pos,
                               void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    if (pos >= self->len) { return false; }
    void *p = elem_ptr_mut(self->ptr, pos);
    return p;
}

void const *cobj_VectorV_try_get(cobj_VectorV const *const self,
                                 cobj_Index pos,
                                 void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    if (pos >= self->len) { return false; }
    void const *p = elem_ptr(self->ptr, pos);
    return p;
}

void cobj_VectorV_as_cobj_SliceV(cobj_VectorV const *const self, cobj_SliceV *const s)
{
    cobj_SliceV_new(s, self->ptr, self->len);
}

void cobj_VectorV_as_cobj_SliceVMut(cobj_VectorV const *const self, cobj_SliceVMut *const s)
{
    cobj_SliceVMut_new(s, self->ptr, self->len);
}

void cobj_VectorV_iter(cobj_VectorV const *const self,
                       cobj_SliceVIter *const it,
                       void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    cobj_SliceVIter_new(it, self->ptr, elem_ptr(self->ptr, self->len));
}

void cobj_VectorV_iter_mut(cobj_VectorV const *const self,
                           cobj_SliceVMutIter *const it,
                           void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    cobj_SliceVMutIter_new(it, self->ptr, elem_ptr_mut(self->ptr, self->len));
}
