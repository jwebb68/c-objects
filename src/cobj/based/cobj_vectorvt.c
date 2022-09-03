#include <cobj/based/cobj_carrayvt.h>
#include <cobj/based/cobj_vectorvt.h>
#include <cobj/core/cobj_memory.h> // STRUCTWIPE

static void cobj_VectorVT_move_elem(void *const self, void *const src)
{
    T_move(self, src);
}

static void cobj_VectorVT_destroy_elem(void *const self)
{
    T_destroy(self);
}

static void const *cobj_VectorVT_elem_ptr(void const *const arr, size_t idx)
{
    return cobj_CArrayVT_get(arr, idx);
}

static void *cobj_VectorVT_elem_ptr_mut(void *const arr, size_t idx)
{
    return cobj_CArrayVT_get_mut(arr, idx);
}

static void cobj_VectorVT_wipe(cobj_VectorVT *const self)
{
    STRUCTWIPE(self);
}

void cobj_VectorVT_destroy_member(cobj_VectorVT *const self)
{
    cobj_VectorV_destroy_member(&self->inner,
                                cobj_VectorVT_elem_ptr_mut,
                                cobj_VectorVT_destroy_elem);
}
void cobj_VectorVT_destroy(cobj_VectorVT *const self)
{
    cobj_VectorVT_destroy_member(self);
    cobj_VectorVT_wipe(self);
}

void cobj_VectorVT_move_member(cobj_VectorVT *const self, cobj_VectorVT *const src)
{
    cobj_VectorV_move_member(&self->inner, &src->inner);
}
void cobj_VectorVT_move(cobj_VectorVT *const self, cobj_VectorVT *const src)
{
    cobj_VectorVT_move_member(self, src);
    cobj_VectorVT_wipe(self);
}

void cobj_VectorVT_new(cobj_VectorVT *const self, T *const ptr, size_t alloc)
{
    cobj_VectorV_new(&self->inner, ptr, alloc);
}

bool cobj_VectorVT_is_empty(cobj_VectorVT const *const self)
{
    return cobj_VectorV_is_empty(&self->inner);
}

void cobj_VectorVT_clear(cobj_VectorVT *const self)
{
    cobj_VectorV_clear(&self->inner, cobj_VectorVT_elem_ptr_mut, cobj_VectorVT_destroy_elem);
}

size_t cobj_VectorVT_len(cobj_VectorVT const *const self)
{
    return cobj_VectorV_len(&self->inner);
}

bool WARN_UNUSED_RESULT cobj_VectorVT_push_back(cobj_VectorVT *const self, T *const elem)
{
    return cobj_VectorV_push_back(&self->inner,
                                  elem,
                                  cobj_VectorVT_elem_ptr_mut,
                                  cobj_VectorVT_move_elem);
}

bool WARN_UNUSED_RESULT cobj_VectorVT_pop_back(cobj_VectorVT *const self, T *const elem)
{
    return cobj_VectorV_pop_back(&self->inner,
                                 elem,
                                 cobj_VectorVT_elem_ptr_mut,
                                 cobj_VectorVT_move_elem);
}

T *cobj_VectorVT_try_get_mut(cobj_VectorVT const *const self, cobj_Index const pos)
{
    return cobj_VectorV_try_get_mut(&self->inner, pos, cobj_VectorVT_elem_ptr_mut);
}

T const *cobj_VectorVT_try_get(cobj_VectorVT const *const self, cobj_Index const pos)
{
    return cobj_VectorV_try_get(&self->inner, pos, cobj_VectorVT_elem_ptr);
}

void cobj_VectorVT_as_cobj_SliceVT(cobj_VectorVT const *const self, cobj_SliceVT *const s)
{
    cobj_VectorV_as_cobj_SliceV(&self->inner, &s->inner);
}

void cobj_VectorVT_as_cobj_SliceVTMut(cobj_VectorVT const *const self, cobj_SliceVTMut *const s)
{
    cobj_VectorV_as_cobj_SliceVMut(&self->inner, &s->inner);
}

void cobj_VectorVT_iter(cobj_VectorVT const *const self, cobj_SliceVTIter *const it)
{
    cobj_VectorV_iter(&self->inner, &it->inner, cobj_VectorVT_elem_ptr);
}

void cobj_VectorVT_iter_mut(cobj_VectorVT const *const self, cobj_SliceVTMutIter *const it)
{
    cobj_VectorV_iter_mut(&self->inner, &it->inner, cobj_VectorVT_elem_ptr_mut);
}
