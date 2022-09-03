#include <cobj/baseless/cobj_carrayt.h>
#include <cobj/baseless/cobj_vectort.h>
#include <cobj/core/cobj_defs.h> // COBJ_UNUSED_ARG
#include <cobj/core/cobj_memory.h> // STRUCTWIPE

static void cobj_VectorT_wipe(cobj_VectorT *const self)
{
    STRUCTWIPE(self);
}

void cobj_VectorT_destroy_member(cobj_VectorT *const self)
{
    cobj_CArrayTMut_destroy(self->ptr, self->len);
}

void cobj_VectorT_destroy(cobj_VectorT *const self)
{
    cobj_VectorT_destroy_member(self);
    cobj_VectorT_wipe(self);
}

void cobj_VectorT_move_member(cobj_VectorT *const self, cobj_VectorT *const src)
{
    *self = *src;
}
void cobj_VectorT_move(cobj_VectorT *const self, cobj_VectorT *const src)
{
    cobj_VectorT_move_member(self, src);
    cobj_VectorT_wipe(self);
}

void cobj_VectorT_new(cobj_VectorT *const self, T *const ptr, size_t len)
{
    // don't use init lists, they are inefficient.
    self->ptr = ptr;
    self->alloc = len;
    self->len = 0;
}

void cobj_VectorT_clear(cobj_VectorT *const self)
{
    cobj_CArrayTMut_destroy(self->ptr, self->len);
    self->len = 0;
}

bool cobj_VectorT_is_empty(cobj_VectorT const *const self)
{
    return self->len == 0;
}

size_t cobj_VectorT_len(cobj_VectorT const *const self)
{
    return self->len;
}

size_t cobj_VectorT_alloc(cobj_VectorT const *const self)
{
    return self->alloc;
}

bool WARN_UNUSED_RESULT cobj_VectorT_realloc(cobj_VectorT *const self, size_t newalloc)
{
    // heapless vector cannot realloc?
    // can it realloc smaller?
    // cannot realloc larger.
    COBJ_UNUSED_ARG(self);
    COBJ_UNUSED_ARG(newalloc);
    return false;
}

bool WARN_UNUSED_RESULT cobj_VectorT_push_back(cobj_VectorT *const self, T *const item)
{
    if (self->len >= self->alloc) { return false; }
    T_move(&self->ptr[self->len], item);
    self->len += 1;
    return true;
}

bool WARN_UNUSED_RESULT cobj_VectorT_pop_back(cobj_VectorT *const self, T *const item)
{
    if (self->len == 0) { return false; }
    self->len -= 1;
    T_move(item, &self->ptr[self->len]);
    return true;
}

T *cobj_VectorT_try_get_mut(cobj_VectorT const *const self, cobj_Index pos)
{
    if (pos >= self->len) { return NULL; }
    return &self->ptr[pos];
}

T const *cobj_VectorT_try_get(cobj_VectorT const *const self, cobj_Index pos)
{
    if (pos >= self->len) { return NULL; }
    return &self->ptr[pos];
}

void cobj_VectorT_as_cobj_SliceT(cobj_VectorT const *const self, cobj_SliceT *const s)
{
    cobj_SliceT_new(s, self->ptr, self->len);
}

void cobj_VectorT_as_cobj_SliceTMut(cobj_VectorT const *const self, cobj_SliceTMut *const s)
{
    cobj_SliceTMut_new(s, self->ptr, self->len);
}

void cobj_VectorT_iter(cobj_VectorT const *const self, cobj_SliceTIter *const it)
{
    cobj_SliceTIter_new(it, self->ptr, self->ptr + self->len);
}

void cobj_VectorT_iter_mut(cobj_VectorT const *const self, cobj_SliceTMutIter *const it)
{
    cobj_SliceTMutIter_new(it, self->ptr, self->ptr + self->len);
}
