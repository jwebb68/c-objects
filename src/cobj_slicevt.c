#include "cobj_slicevt.h"

#include "cobj_defs.h" // UNUSED_ARG
#include "cobj_memory.h"

#include <memory.h> // memmove

static void SliceVT_wipe(SliceVT *const self)
{
    STRUCTWIPE(self);
}

void SliceVT_destroy(SliceVT *const self)
{
    SliceV_destroy(&self->inner);
}

void SliceVT_move(SliceVT *const self, SliceVT *const src)
{
    *self = *src;
    SliceVT_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVT_try_copy(SliceVT *self, SliceVT const *const src, Error *err)
{
    UNUSED_ARG(err);
    *self = *src;
    return true;
}

void SliceVT_new(SliceVT *const self, T const *const b, T const *const e)
{
    SliceV_new(&self->inner, b, e);
}

size_t SliceVT_len(SliceVT const *const self)
{
    return SliceV_len(&self->inner, sizeof(T));
}

bool SliceVT_is_empty(SliceVT const *const self)
{
    return SliceV_is_empty(&self->inner);
}

T const *SliceVT_get_item_at(SliceVT const *const self, size_t pos)
{
    return (T const *)SliceV_get_item_at(&self->inner, pos, sizeof(T));
}

bool SliceVT_try_subslice(SliceVT const *const self,
                          size_t b,
                          size_t e,
                          SliceVT *const dest,
                          Error *err)
{
    return SliceV_try_subslice(&self->inner, b, e, &dest->inner, err, sizeof(T));
}

void SliceVT_iter(SliceVT const *const self, SliceVTIter *const it)
{
    SliceV_iter(&self->inner, &it->inner);
}

// =============================================================================

static void SliceVTIter_wipe(SliceVTIter *const self)
{
    STRUCTWIPE(self);
}

void SliceVTIter_destroy(SliceVTIter *const self)
{
    SliceVIter_destroy(&self->inner);
}

void SliceVTIter_move(SliceVTIter *const self, SliceVTIter *const src)
{
    *self = *src;
    SliceVTIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVTIter_try_copy(SliceVTIter *const self, SliceVTIter const *const src)
{
    *self = *src;
    return true;
}

T const *SliceVTIter_next(SliceVTIter *const self)
{
    return SliceVIter_next(&self->inner, sizeof(T));
}

void SliceVTIter_new(SliceVTIter *const self, T const *const b, T const *const e)
{
    SliceVIter_new(&self->inner, b, e);
}

//============================================================================
static bool SliceVT_elem_try_copy(void *self, void const *src, Error *err)
{
    return T_try_copy((T *)self, (T const *)src, err);
}

static void SliceVT_elem_destroy(void *self)
{
    T_destroy((T *)self);
}

void SliceVTMut_wipe(SliceVTMut *const self)
{
    STRUCTWIPE(self);
}

void SliceVTMut_destroy(SliceVTMut *const self)
{
    SliceVMut_destroy(&self->inner);
}

void SliceVTMut_move(SliceVTMut *const self, SliceVTMut *const src)
{
    *self = *src;
    SliceVTMut_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVTMut_try_copy(SliceVTMut *self,
                                            SliceVTMut const *const src,
                                            Error *err)
{
    UNUSED_ARG(err);
    *self = *src;
    return true;
}

void SliceVTMut_new(SliceVTMut *const self, T *const b, T *const e)
{
    SliceVMut_new(&self->inner, b, e);
}

size_t SliceVTMut_len(SliceVTMut const *const self)
{
    return SliceVMut_len(&self->inner, sizeof(T));
}

bool SliceVTMut_is_empty(SliceVTMut const *const self)
{
    return SliceVMut_is_empty(&self->inner);
}

bool SliceVTMut_try_subslice(SliceVTMut const *const self,
                             size_t b,
                             size_t e,
                             SliceVT *const dest,
                             Error *err)
{
    return SliceVMut_try_subslice(&self->inner, b, e, &dest->inner, err, sizeof(T));
}

void SliceVTMut_as_slice(SliceVTMut const *const self, SliceVT *const s)
{
    SliceVMut_as_slice(&self->inner, &s->inner);
}

bool WARN_UNUSED_RESULT SliceVTMut_try_move_from(SliceVTMut *const self,
                                                 SliceVTMut const *const src,
                                                 Error *err)
{
    return SliceVMut_try_move_from(&self->inner, &src->inner, err, sizeof(T));
}

bool WARN_UNUSED_RESULT SliceVTMut_try_copy_from(SliceVTMut *const self,
                                                 SliceVT const *const src,
                                                 Error *err)
{
    return SliceVMut_try_copy_from(&self->inner,
                                   &src->inner,
                                   err,
                                   sizeof(T),
                                   SliceVT_elem_try_copy,
                                   SliceVT_elem_destroy);
}

void SliceVTMut_iter(SliceVTMut const *const self, SliceVTIter *const it)
{
    SliceVMut_iter(&self->inner, &it->inner);
}

void SliceVTMut_iter_mut(SliceVTMut const *const self, SliceVTMutIter *const it)
{
    SliceVMut_iter_mut(&self->inner, &it->inner);
}

// ============================================================================
void SliceVTMutIter_wipe(SliceVTMutIter *const self)
{
    STRUCTWIPE(self);
}

void SliceVTMutIter_destroy(SliceVTMutIter *const self)
{
    SliceVMutIter_destroy(&self->inner);
}

void SliceVTMutIter_move(SliceVTMutIter *const self, SliceVTMutIter *const src)
{
    *self = *src;
    SliceVTMutIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVTMutIter_try_copy(SliceVTMutIter *const self,
                                                SliceVTMutIter const *const src,
                                                Error *const err)
{
    return SliceVMutIter_try_copy(&self->inner, &src->inner, err);
}

T *SliceVTMutIter_next(SliceVTMutIter *const self)
{
    return SliceVMutIter_next(&self->inner, sizeof(T));
}

void SliceVTMutIter_new(SliceVTMutIter *const self, T *const b, T *const e)
{
    SliceVMutIter_new(&self->inner, b, e);
}
