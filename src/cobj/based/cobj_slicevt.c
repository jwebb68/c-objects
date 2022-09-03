#include <cobj/based/cobj_carrayvt.h>
#include <cobj/based/cobj_slicevt.h>
#include <cobj/core/cobj_defs.h> // COBJ_UNUSED_ARG
#include <cobj/core/cobj_memory.h>
#include <memory.h> // memmove

static void const *SliceVT_elem_ptr(void const *const arr, size_t idx)
{
    return &((T const *)arr)[idx];
}

static void *SliceVT_elem_ptr_mut(void *const arr, size_t idx)
{
    return &((T *)arr)[idx];
}

static void SliceVT_elem_destroy(void *const elem)
{
    T_destroy_member((T *)elem);
}

static bool SliceVT_elem_try_copy(void *const elem, void const *const src, Error *const err)
{
    return T_try_copy((T *)elem, (T const *)src, err);
}

static void SliceVT_wipe(SliceVT *const self)
{
    STRUCTWIPE(self);
}

void SliceVT_destroy_member(SliceVT *const self)
{
    COBJ_UNUSED_ARG(self);
}
void SliceVT_destroy(SliceVT *const self)
{
    SliceVT_destroy_member(self);
    SliceVT_wipe(self);
}

void SliceVT_move_member(SliceVT *const self, SliceVT *const src)
{
    *self = *src;
}
void SliceVT_move(SliceVT *const self, SliceVT *const src)
{
    SliceVT_move_member(self, src);
    SliceVT_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVT_try_copy(SliceVT *const self,
                                         SliceVT const *const src,
                                         Error *const err)
{
    return SliceV_try_copy(&self->inner, &src->inner, err);
}

void SliceVT_new(SliceVT *const self, T const *ptr, size_t len)
{
    SliceV_new(&self->inner, ptr, len);
}

size_t SliceVT_len(SliceVT const *const self)
{
    return SliceV_len(&self->inner);
}

bool SliceVT_is_empty(SliceVT const *const self)
{
    return SliceV_is_empty(&self->inner);
}

T const *SliceVT_try_get(SliceVT const *const self, size_t pos)
{
    return SliceV_try_get(&self->inner, pos, SliceVT_elem_ptr);
}

bool SliceVT_try_subslice(SliceVT const *const self,
                          size_t b,
                          size_t e,
                          SliceVT *const dest,
                          Error *const err)
{
    return SliceV_try_subslice(&self->inner, b, e, &dest->inner, err, SliceVT_elem_ptr);
}

void SliceVT_iter(SliceVT const *const self, SliceVTIter *const it)
{
    SliceV_iter(&self->inner, &it->inner, SliceVT_elem_ptr);
}

// =============================================================================

static void SliceVTIter_wipe(SliceVTIter *const self)
{
    STRUCTWIPE(self);
}

void SliceVTIter_destroy_member(SliceVTIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void SliceVTIter_destroy(SliceVTIter *const self)
{
    SliceVTIter_destroy_member(self);
    SliceVTIter_wipe(self);
}

void SliceVTIter_move_member(SliceVTIter *const self, SliceVTIter *const src)
{
    *self = *src;
}
void SliceVTIter_move(SliceVTIter *const self, SliceVTIter *const src)
{
    SliceVTIter_move_member(self, src);
    SliceVTIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVTIter_try_copy(SliceVTIter *const self,
                                             SliceVTIter const *const src,
                                             Error *const err)
{
    return SliceVIter_try_copy(&self->inner, &src->inner, err);
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

static void *SliceVTMut_elem_ptr(void *const arr, size_t idx)
{
    return &((T *)arr)[idx];
}

void SliceVTMut_wipe(SliceVTMut *const self)
{
    STRUCTWIPE(self);
}

void SliceVTMut_destroy_member(SliceVTMut *const self)
{
    COBJ_UNUSED_ARG(self);
}
void SliceVTMut_destroy(SliceVTMut *const self)
{
    SliceVTMut_destroy_member(self);
    SliceVTMut_wipe(self);
}

void SliceVTMut_move_member(SliceVTMut *const self, SliceVTMut *const src)
{
    *self = *src;
}
void SliceVTMut_move(SliceVTMut *const self, SliceVTMut *const src)
{
    SliceVTMut_move_member(self, src);
    SliceVTMut_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVTMut_try_copy(SliceVTMut *self,
                                            SliceVTMut const *const src,
                                            Error *const err)
{
    return SliceVMut_try_copy(&self->inner, &src->inner, err);
}

void SliceVTMut_new(SliceVTMut *const self, T *const ptr, size_t len)
{
    SliceVMut_new(&self->inner, ptr, len);
}

size_t SliceVTMut_len(SliceVTMut const *const self)
{
    return SliceVMut_len(&self->inner);
}

bool SliceVTMut_is_empty(SliceVTMut const *const self)
{
    return SliceVMut_is_empty(&self->inner);
}

T *SliceVTMut_try_get(SliceVTMut const *const self, size_t pos)
{
    return SliceVMut_try_get(&self->inner, pos, SliceVTMut_elem_ptr);
}

bool SliceVTMut_try_subslice(SliceVTMut const *const self,
                             size_t b,
                             size_t e,
                             SliceVT *const dest,
                             Error *const err)
{
    return SliceVMut_try_subslice(&self->inner, b, e, &dest->inner, err, SliceVTMut_elem_ptr);
}

bool SliceVTMut_try_subslice_mut(SliceVTMut const *const self,
                                 size_t b,
                                 size_t e,
                                 SliceVTMut *const dest,
                                 Error *const err)
{
    return SliceVMut_try_subslice_mut(&self->inner, b, e, &dest->inner, err, SliceVTMut_elem_ptr);
}

void SliceVTMut_as_SliceVT(SliceVTMut const *const self, SliceVT *const s)
{
    SliceVMut_as_SliceV(&self->inner, &s->inner);
}

bool WARN_UNUSED_RESULT SliceVTMut_try_move_from(SliceVTMut const *const self,
                                                 SliceVTMut const *const src,
                                                 Error *const err)
{
    // move content of one slice into another.
    // before: src must be initialised pointing to initialised, self must be initialised to
    // uninitialised. after: src is initialised to de-initialised, self is initialised to
    // initialised. both must be same size..
    return SliceVMut_try_move_from(&self->inner, &src->inner, err, SliceVTMut_elem_ptr);
}

bool WARN_UNUSED_RESULT SliceVTMut_try_copy_from(SliceVTMut const *const self,
                                                 SliceVT const *const src,
                                                 Error *const err)
{
    // copy content of one slice into another.
    // before: src must be initialised pointing to initialised, self must be initialised to
    // uninitialised. after: src is initialised to initialised, self is initialised to initialised.
    // both must be same size..
    return SliceVMut_try_copy_from(&self->inner,
                                   &src->inner,
                                   err,
                                   SliceVT_elem_ptr,
                                   SliceVT_elem_ptr_mut,
                                   SliceVT_elem_try_copy,
                                   SliceVT_elem_destroy);
}

void SliceVTMut_iter(SliceVTMut const *const self, SliceVTIter *const it)
{
    SliceVMut_iter(&self->inner, &it->inner, SliceVTMut_elem_ptr);
}

void SliceVTMut_iter_mut(SliceVTMut const *const self, SliceVTMutIter *const it)
{
    SliceVMut_iter_mut(&self->inner, &it->inner, SliceVTMut_elem_ptr);
}

// ============================================================================
void SliceVTMutIter_wipe(SliceVTMutIter *const self)
{
    STRUCTWIPE(self);
}

void SliceVTMutIter_destroy_member(SliceVTMutIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void SliceVTMutIter_destroy(SliceVTMutIter *const self)
{
    SliceVTMutIter_destroy_member(self);
    SliceVTMutIter_wipe(self);
}

void SliceVTMutIter_move_member(SliceVTMutIter *const self, SliceVTMutIter *const src)
{
    *self = *src;
}
void SliceVTMutIter_move(SliceVTMutIter *const self, SliceVTMutIter *const src)
{
    SliceVTMutIter_move_member(self, src);
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
