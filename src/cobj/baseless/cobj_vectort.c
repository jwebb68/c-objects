#include <cobj/baseless/cobj_carrayt.h>
#include <cobj/baseless/cobj_vectort.h>
#include <cobj/core/cobj_defs.h> // COBJ_UNUSED_ARG
#include <cobj/core/cobj_memory.h> // STRUCTWIPE

static void VectorT_wipe(VectorT *const self)
{
    STRUCTWIPE(self);
}

void VectorT_destroy_member(VectorT *const self)
{
    CArrayTMut_destroy(self->ptr, self->len);
}

void VectorT_destroy(VectorT *const self)
{
    VectorT_destroy_member(self);
    VectorT_wipe(self);
}

void VectorT_move_member(VectorT *const self, VectorT *const src)
{
    *self = *src;
}
void VectorT_move(VectorT *const self, VectorT *const src)
{
    VectorT_move_member(self, src);
    VectorT_wipe(self);
}

void VectorT_new(VectorT *const self, T *const ptr, size_t len)
{
    // don't use init lists, they are inefficient.
    self->ptr = ptr;
    self->alloc = len;
    self->len = 0;
}

void VectorT_clear(VectorT *const self)
{
    CArrayTMut_destroy(self->ptr, self->len);
    self->len = 0;
}

bool VectorT_is_empty(VectorT const *const self)
{
    return self->len == 0;
}

size_t VectorT_len(VectorT const *const self)
{
    return self->len;
}

size_t VectorT_alloc(VectorT const *const self)
{
    return self->alloc;
}

bool WARN_UNUSED_RESULT VectorT_realloc(VectorT *const self, size_t newalloc)
{
    // heapless vector cannot realloc?
    // can it realloc smaller?
    // cannot realloc larger.
    COBJ_UNUSED_ARG(self);
    COBJ_UNUSED_ARG(newalloc);
    return false;
}

bool WARN_UNUSED_RESULT VectorT_push_back(VectorT *const self, T *const item)
{
    if (self->len >= self->alloc) { return false; }
    T_move(&self->ptr[self->len], item);
    self->len += 1;
    return true;
}

bool WARN_UNUSED_RESULT VectorT_pop_back(VectorT *const self, T *const item)
{
    if (self->len == 0) { return false; }
    self->len -= 1;
    T_move(item, &self->ptr[self->len]);
    return true;
}

T *VectorT_try_get_mut(VectorT const *const self, cobj_Index pos)
{
    if (pos >= self->len) { return NULL; }
    return &self->ptr[pos];
}

T const *VectorT_try_get(VectorT const *const self, cobj_Index pos)
{
    if (pos >= self->len) { return NULL; }
    return &self->ptr[pos];
}

void VectorT_as_SliceT(VectorT const *const self, SliceT *const s)
{
    SliceT_new(s, self->ptr, self->len);
}

void VectorT_as_SliceTMut(VectorT const *const self, SliceTMut *const s)
{
    SliceTMut_new(s, self->ptr, self->len);
}

void VectorT_iter(VectorT const *const self, SliceTIter *const it)
{
    SliceTIter_new(it, self->ptr, self->ptr + self->len);
}

void VectorT_iter_mut(VectorT const *const self, SliceTMutIter *const it)
{
    SliceTMutIter_new(it, self->ptr, self->ptr + self->len);
}
