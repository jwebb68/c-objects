#include "cobj_vectorvt.h"

#include "cobj_carrayvt.h"
#include "cobj_memory.h" // STRUCTWIPE

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

T *VectorVT_try_get_mut(VectorVT const *const self, Index const pos)
{
    return VectorV_try_get_mut(&self->inner, pos, VectorVT_elem_ptr_mut);
}

T const *VectorVT_try_get(VectorVT const *const self, Index const pos)
{
    return VectorV_try_get(&self->inner, pos, VectorVT_elem_ptr);
}

static void SliceV_into_SliceT(SliceV *const self, SliceT *const dest)
{
    dest->ptr = (T const *)self->ptr;
    dest->len = self->len;
    SliceV_wipe(self);
}

void VectorVT_as_SliceT(VectorVT const *const self, SliceT *const s)
{
    SliceV vs;
    VectorV_as_SliceV(&self->inner, &vs);
    SliceV_into_SliceT(&vs, s);
}

static void SliceVMut_into_SliceTMut(SliceVMut *const self, SliceTMut *const dest)
{
    dest->ptr = (T *)self->ptr;
    dest->len = self->len;
    SliceVMut_wipe(self);
}

void VectorVT_as_SliceTMut(VectorVT const *const self, SliceTMut *const s)
{
    SliceVMut vs;
    VectorV_as_SliceVMut(&self->inner, &vs);
    SliceVMut_into_SliceTMut(&vs, s);
}

static void SliceVIter_into_SliceTIter(SliceVIter *const self, SliceTIter *const dest)
{
    dest->p = (T const *)self->p;
    dest->e = (T const *)self->e;
    SliceVIter_wipe(self);
}

void VectorVT_iter(VectorVT const *const self, SliceTIter *const it)
{
    SliceVIter vi;
    VectorV_iter(&self->inner, &vi, VectorVT_elem_ptr);
    SliceVIter_into_SliceTIter(&vi, it);
}

static void SliceVMutIter_into_SliceTMutIter(SliceVMutIter *const self, SliceTMutIter *const dest)
{
    dest->p = (T *)self->p;
    dest->e = (T *)self->e;
    SliceVMutIter_wipe(self);
}

void VectorVT_iter_mut(VectorVT const *const self, SliceTMutIter *const it)
{
    SliceVMutIter vi;
    VectorV_iter_mut(&self->inner, &vi, VectorVT_elem_ptr_mut);
    SliceVMutIter_into_SliceTMutIter(&vi, it);
}
