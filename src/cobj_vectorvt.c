#include "cobj_vectorvt.h"

#include "cobj_memory.h" // STRUCTWIPE

static void VectorVT_move_elem(void *const self, void *const src)
{
    T_move((T *)self, (T *)src);
}

static void VectorVT_destroy_elem(void *const self)
{
    T_destroy((T *)self);
}

static void VectorVT_wipe(VectorVT *const self)
{
    STRUCTWIPE(self);
}

void VectorVT_destroy(VectorVT *const self)
{
    VectorV_destroy(&self->inner, sizeof(T), VectorVT_destroy_elem);
    // do we need to wipe if only 1 struct in struct with own wipe?
}

void VectorVT_move(VectorVT *const self, VectorVT *const src)
{
    *self = *src;
    VectorVT_wipe(src);
}

void VectorVT_new(VectorVT *const self, T *const b, T *const e)
{
    VectorV_new(&self->inner, (uint8_t *)b, (uint8_t *)e);
}

bool VectorVT_is_empty(VectorVT const *const self)
{
    return VectorV_is_empty(&self->inner);
}

void VectorVT_clear(VectorVT *const self)
{
    VectorV_clear(&self->inner, sizeof(T), VectorVT_destroy_elem);
}

size_t VectorVT_len(VectorVT const *const self)
{
    return VectorV_len(&self->inner, sizeof(T));
}

bool WARN_UNUSED_RESULT VectorVT_push_back(VectorVT *const self, T *const elem)
{
    return VectorV_push_back(&self->inner, elem, sizeof(T), VectorVT_move_elem);
}

bool WARN_UNUSED_RESULT VectorVT_pop_back(VectorVT *const self, T *const elem)
{
    return VectorV_pop_back(&self->inner, elem, sizeof(T), VectorVT_move_elem);
}

T *VectorVT_get_item_at_mut(VectorVT const *const self, Index const pos)
{
    return (T *)VectorV_get_item_at_mut(&self->inner, pos, sizeof(T));
}

T const *VectorVT_get_item_at(VectorVT const *const self, Index const pos)
{
    return (T const *)VectorV_get_item_at(&self->inner, pos, sizeof(T));
}

void VectorVT_iter(VectorVT const *const self, VectorVTIter *const it)
{
    VectorV_iter(&self->inner, &it->inner);
}

void VectorVT_iter_mut(VectorVT const *const self, VectorVTMutIter *const it)
{
    VectorV_iter_mut(&self->inner, &it->inner);
}

//===========================================================================

static void VectorVTIter_wipe(VectorVTIter *const self)
{
    STRUCTWIPE(self);
}

void VectorVTIter_destroy(VectorVTIter *const self)
{
    VectorVIter_destroy(&self->inner);
    // do we need to wipe if only 1 struct in struct with own wipe?
}

void VectorVTIter_move(VectorVTIter *const self, VectorVTIter *const src)
{
    *self = *src;
    VectorVTIter_wipe(src);
}

T const *WARN_UNUSED_RESULT VectorVTIter_next(VectorVTIter *const self)
{
    return (T const *)VectorVIter_next(&self->inner, sizeof(T));
}

void VectorVTIter_new(VectorVTIter *const self, T const *const b, T const *const e)
{
    VectorVIter_new(&self->inner, b, e);
}

// ========================================================================

static void VectorVTMutIter_wipe(VectorVTMutIter *const self)
{
    STRUCTWIPE(self);
}

void VectorVTMutIter_destroy(VectorVTMutIter *const self)
{
    VectorVMutIter_destroy(&self->inner);
    // do we need to wipe if only 1 struct in struct with own wipe?
}

void VectorVTMutIter_move(VectorVTMutIter *const self, VectorVTMutIter *const src)
{
    *self = *src;
    VectorVTMutIter_wipe(src);
}

T *WARN_UNUSED_RESULT VectorVTMutIter_next(VectorVTMutIter *const self)
{
    return (T *)VectorVMutIter_next(&self->inner, sizeof(T));
}

void VectorVTMutIter_new(VectorVTMutIter *const self, T *const b, T *const e)
{
    VectorVMutIter_new(&self->inner, b, e);
}
