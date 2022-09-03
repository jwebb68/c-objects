#include <cobj/based/cobj_queuevt.h>
#include <cobj/core/cobj_memory.h>
// ===========================================================================
static void cobj_QueueVT_elem_move(void *const d, void *const s)
{
    T_move(d, s);
}

static void *cobj_QueueVT_elem_ptr_mut(void *const arr, size_t idx)
{
    return &((T *)arr)[idx];
}

static void cobj_QueueVT_elem_destroy_member(void *const elem)
{
    T_destroy_member(elem);
}

static void cobj_QueueVT_wipe(cobj_QueueVT *const self)
{
    STRUCTWIPE(self);
}

void cobj_QueueVT_destroy_member(cobj_QueueVT *const self)
{
    cobj_QueueV_destroy_member(&self->inner,
                               cobj_QueueVT_elem_ptr_mut,
                               cobj_QueueVT_elem_destroy_member);
}

void cobj_QueueVT_destroy(cobj_QueueVT *const self)
{
    cobj_QueueVT_destroy_member(self);
    cobj_QueueVT_wipe(self);
}

void cobj_QueueVT_move_member(cobj_QueueVT *const self, cobj_QueueVT *const src)
{
    cobj_QueueV_move_member(&self->inner, &src->inner);
}

void cobj_QueueVT_move(cobj_QueueVT *const self, cobj_QueueVT *const src)
{
    cobj_QueueVT_move_member(self, src);
    cobj_QueueVT_wipe(src);
}

void cobj_QueueVT_new(cobj_QueueVT *const self, void *const ptr, size_t alloc)
{
    cobj_QueueV_new(&self->inner, ptr, alloc);
}
void cobj_QueueVT_clear(cobj_QueueVT *const self)
{
    cobj_QueueV_clear(&self->inner, cobj_QueueVT_elem_ptr_mut, cobj_QueueVT_elem_destroy_member);
}

bool cobj_QueueVT_is_empty(cobj_QueueVT const *const self)
{
    return cobj_QueueV_is_empty(&self->inner);
}

size_t cobj_QueueVT_len(cobj_QueueVT const *const self)
{
    return cobj_QueueV_len(&self->inner);
}
size_t cobj_QueueVT_alloc(cobj_QueueVT const *const self)
{
    return cobj_QueueV_alloc(&self->inner);
}

bool WARN_UNUSED_RESULT cobj_QueueVT_put(cobj_QueueVT *const self, T *const elem)
{
    return cobj_QueueV_put(&self->inner, elem, cobj_QueueVT_elem_ptr_mut, cobj_QueueVT_elem_move);
}
bool WARN_UNUSED_RESULT cobj_QueueVT_get(cobj_QueueVT *const self, T *const item)
{
    return cobj_QueueV_get(&self->inner, item, cobj_QueueVT_elem_ptr_mut, cobj_QueueVT_elem_move);
}
