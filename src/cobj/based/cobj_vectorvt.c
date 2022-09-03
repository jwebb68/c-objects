#include <cobj/based/cobj_carrayvt.h>
#include <cobj/based/cobj_vectorvt.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE

static void VectorVT_move_elem(void *const self, void *const src)
{
    T_move(self, src);
}

static void VectorVT_destroy_elem(void *const self)
{
    T_destroy(self);
}

static void const *VectorVT_elem_ptr(void const *const arr, size_t idx)
{
    return CArrayVT_get(arr, idx);
}

static void *VectorVT_elem_ptr_mut(void *const arr, size_t idx)
{
    return CArrayVT_get_mut(arr, idx);
}

static void VectorVT_wipe(VectorVT *const self)
{
    STRUCTWIPE(self);
}

void VectorVT_destroy_member(VectorVT *const self)
{
    VectorV_destroy_member(&self->inner, VectorVT_elem_ptr_mut, VectorVT_destroy_elem);
}
void VectorVT_destroy(VectorVT *const self)
{
    VectorVT_destroy_member(self);
    VectorVT_wipe(self);
}

void VectorVT_move_member(VectorVT *const self, VectorVT *const src)
{
    VectorV_move_member(&self->inner, &src->inner);
}
void VectorVT_move(VectorVT *const self, VectorVT *const src)
{
    VectorVT_move_member(self, src);
    VectorVT_wipe(self);
}

void VectorVT_new(VectorVT *const self, T *const ptr, size_t alloc)
{
    VectorV_new(&self->inner, ptr, alloc);
}

bool VectorVT_is_empty(VectorVT const *const self)
{
    return VectorV_is_empty(&self->inner);
}

void VectorVT_clear(VectorVT *const self)
{
    VectorV_clear(&self->inner, VectorVT_elem_ptr_mut, VectorVT_destroy_elem);
}

size_t VectorVT_len(VectorVT const *const self)
{
    return VectorV_len(&self->inner);
}

bool WARN_UNUSED_RESULT VectorVT_push_back(VectorVT *const self, T *const elem)
{
    return VectorV_push_back(&self->inner, elem, VectorVT_elem_ptr_mut, VectorVT_move_elem);
}

bool WARN_UNUSED_RESULT VectorVT_pop_back(VectorVT *const self, T *const elem)
{
    return VectorV_pop_back(&self->inner, elem, VectorVT_elem_ptr_mut, VectorVT_move_elem);
}

T *VectorVT_try_get_mut(VectorVT const *const self, cobj_Index const pos)
{
    return VectorV_try_get_mut(&self->inner, pos, VectorVT_elem_ptr_mut);
}

T const *VectorVT_try_get(VectorVT const *const self, cobj_Index const pos)
{
    return VectorV_try_get(&self->inner, pos, VectorVT_elem_ptr);
}

void VectorVT_as_SliceVT(VectorVT const *const self, SliceVT *const s)
{
    VectorV_as_SliceV(&self->inner, &s->inner);
}

void VectorVT_as_SliceVTMut(VectorVT const *const self, SliceVTMut *const s)
{
    VectorV_as_SliceVMut(&self->inner, &s->inner);
}

void VectorVT_iter(VectorVT const *const self, SliceVTIter *const it)
{
    VectorV_iter(&self->inner, &it->inner, VectorVT_elem_ptr);
}

void VectorVT_iter_mut(VectorVT const *const self, SliceVTMutIter *const it)
{
    VectorV_iter_mut(&self->inner, &it->inner, VectorVT_elem_ptr_mut);
}
