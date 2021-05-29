#include "cobj_vectort.h"

#include "cobj_arrayt.h"
#include "cobj_defs.h" // UNUSED_ARG
#include "cobj_memory.h" // STRUCTWIPE

static void VectorT_wipe(VectorT *const self)
{
    STRUCTWIPE(self);
}

void VectorT_destroy(VectorT *const self)
{
    ArrayT_destroy_p(self->arr, self->arr_pos);
    VectorT_wipe(self);
}

void VectorT_move(VectorT *const self, VectorT *const src)
{
    *self = *src;
    VectorT_wipe(self);
}

void VectorT_new(VectorT *const self, T *const arr, size_t len)
{
    // don't use init lists, they are inefficient.
    self->arr = arr;
    self->arr_end = arr + len;
    self->arr_pos = arr;
}

void VectorT_clear(VectorT *const self)
{
    ArrayT_destroy_p(self->arr, self->arr_pos);
    self->arr_pos = self->arr;
}

bool VectorT_is_empty(VectorT const *const self)
{
    return self->arr_pos == self->arr;
}

size_t VectorT_len(VectorT const *const self)
{
    return self->arr_pos - self->arr;
}

size_t VectorT_alloc(VectorT const *const self)
{
    return self->arr_end - self->arr;
}

bool WARN_UNUSED_RESULT VectorT_realloc(VectorT *const self, size_t newalloc)
{
    // heapless vector cannot realloc?
    // can it realloc smaller?
    // cannot realloc larger.
    UNUSED_ARG(self);
    UNUSED_ARG(newalloc);
    return false;
}

bool WARN_UNUSED_RESULT VectorT_push_back(VectorT *const self, T *const item)
{
    // moves item
    if (self->arr_pos >= self->arr_end) {
        return false;
    }
    T_move(self->arr_pos, item);
    self->arr_pos += 1;
    return true;
}

bool WARN_UNUSED_RESULT VectorT_pop_back(VectorT *const self, T *const item)
{
    // moves item
    if (self->arr_pos == self->arr) {
        return false;
    }
    self->arr_pos -= 1;
    T_move(item, self->arr_pos);
    return true;
}

T *VectorT_get_item_at_mut(VectorT const *const self, Index pos)
{
    // NULL if not exist
    T *p = self->arr + pos;
    if (p >= self->arr_end) {
        return NULL;
    }
    return p;
}

T const *VectorT_get_item_at(VectorT const *const self, Index pos)
{
    // NULL if not exist
    T *p = self->arr + pos;
    if (p >= self->arr_end) {
        return NULL;
    }
    return p;
}

void VectorT_as_slice(VectorT const *const self, SliceT *const s)
{
    SliceT_new(s, self->arr, VectorT_len(self));
}

void VectorT_as_slice_mut(VectorT const *const self, SliceTMut *const s)
{
    SliceTMut_new(s, self->arr, VectorT_len(self));
}

void VectorT_iter(VectorT const *const self, SliceTIter *const it)
{
    SliceTIter_new(it, self->arr, self->arr_pos);
}

void VectorT_iter_mut(VectorT const *const self, SliceTMutIter *const it)
{
    SliceTMutIter_new(it, self->arr, self->arr_pos);
}
