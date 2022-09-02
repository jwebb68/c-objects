#include "cobj_queuevt.h"

#include "cobj_memory.h"
// ===========================================================================
static void QueueVT_elem_move(void *const d, void *const s)
{
    T_move(d, s);
}

static void *QueueVT_elem_ptr_mut(void *const arr, size_t idx)
{
    return &((T *)arr)[idx];
}

static void QueueVT_elem_destroy_member(void *const elem)
{
    T_destroy_member(elem);
}

static void QueueVT_wipe(QueueVT *const self)
{
    STRUCTWIPE(self);
}

void QueueVT_destroy_member(QueueVT *const self)
{
    QueueV_destroy_member(&self->inner, QueueVT_elem_ptr_mut, QueueVT_elem_destroy_member);
}

void QueueVT_destroy(QueueVT *const self)
{
    QueueVT_destroy_member(self);
    QueueVT_wipe(self);
}

void QueueVT_move_member(QueueVT *const self, QueueVT *const src)
{
    QueueV_move_member(&self->inner, &src->inner);
}

void QueueVT_move(QueueVT *const self, QueueVT *const src)
{
    QueueVT_move_member(self, src);
    QueueVT_wipe(src);
}

void QueueVT_new(QueueVT *const self, void *const ptr, size_t alloc)
{
    QueueV_new(&self->inner, ptr, alloc);
}
void QueueVT_clear(QueueVT *const self)
{
    QueueV_clear(&self->inner, QueueVT_elem_ptr_mut, QueueVT_elem_destroy_member);
}

bool QueueVT_is_empty(QueueVT const *const self)
{
    return QueueV_is_empty(&self->inner);
}

size_t QueueVT_len(QueueVT const *const self)
{
    return QueueV_len(&self->inner);
}
size_t QueueVT_alloc(QueueVT const *const self)
{
    return QueueV_alloc(&self->inner);
}

bool WARN_UNUSED_RESULT QueueVT_put(QueueVT *const self, T *const elem)
{
    return QueueV_put(&self->inner, elem, QueueVT_elem_ptr_mut, QueueVT_elem_move);
}
bool WARN_UNUSED_RESULT QueueVT_get(QueueVT *const self, T *const item)
{
    return QueueV_get(&self->inner, item, QueueVT_elem_ptr_mut, QueueVT_elem_move);
}
