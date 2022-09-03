#include <cobj/based/cobj_carrayvt.h>
#include <cobj/based/cobj_slicevt.h>
#include <cobj/core/cobj_defs.h> // COBJ_UNUSED_ARG
#include <cobj/core/cobj_memory.h>
#include <memory.h> // memmove

static void const *cobj_SliceVT_elem_ptr(void const *const arr, size_t idx)
{
    return &((T const *)arr)[idx];
}

static void *cobj_SliceVT_elem_ptr_mut(void *const arr, size_t idx)
{
    return &((T *)arr)[idx];
}

static void cobj_SliceVT_elem_destroy(void *const elem)
{
    T_destroy_member((T *)elem);
}

static bool
cobj_SliceVT_elem_try_copy(void *const elem, void const *const src, cobj_Error *const err)
{
    return T_try_copy((T *)elem, (T const *)src, err);
}

static void cobj_SliceVT_wipe(cobj_SliceVT *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceVT_destroy_member(cobj_SliceVT *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceVT_destroy(cobj_SliceVT *const self)
{
    cobj_SliceVT_destroy_member(self);
    cobj_SliceVT_wipe(self);
}

void cobj_SliceVT_move_member(cobj_SliceVT *const self, cobj_SliceVT *const src)
{
    *self = *src;
}
void cobj_SliceVT_move(cobj_SliceVT *const self, cobj_SliceVT *const src)
{
    cobj_SliceVT_move_member(self, src);
    cobj_SliceVT_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceVT_try_copy(cobj_SliceVT *const self,
                                              cobj_SliceVT const *const src,
                                              cobj_Error *const err)
{
    return cobj_SliceV_try_copy(&self->inner, &src->inner, err);
}

void cobj_SliceVT_new(cobj_SliceVT *const self, T const *ptr, size_t len)
{
    cobj_SliceV_new(&self->inner, ptr, len);
}

size_t cobj_SliceVT_len(cobj_SliceVT const *const self)
{
    return cobj_SliceV_len(&self->inner);
}

bool cobj_SliceVT_is_empty(cobj_SliceVT const *const self)
{
    return cobj_SliceV_is_empty(&self->inner);
}

T const *cobj_SliceVT_try_get(cobj_SliceVT const *const self, size_t pos)
{
    return cobj_SliceV_try_get(&self->inner, pos, cobj_SliceVT_elem_ptr);
}

bool cobj_SliceVT_try_subslice(cobj_SliceVT const *const self,
                               size_t b,
                               size_t e,
                               cobj_SliceVT *const dest,
                               cobj_Error *const err)
{
    return cobj_SliceV_try_subslice(&self->inner, b, e, &dest->inner, err, cobj_SliceVT_elem_ptr);
}

void cobj_SliceVT_iter(cobj_SliceVT const *const self, cobj_SliceVTIter *const it)
{
    cobj_SliceV_iter(&self->inner, &it->inner, cobj_SliceVT_elem_ptr);
}

// =============================================================================

static void cobj_SliceVTIter_wipe(cobj_SliceVTIter *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceVTIter_destroy_member(cobj_SliceVTIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceVTIter_destroy(cobj_SliceVTIter *const self)
{
    cobj_SliceVTIter_destroy_member(self);
    cobj_SliceVTIter_wipe(self);
}

void cobj_SliceVTIter_move_member(cobj_SliceVTIter *const self, cobj_SliceVTIter *const src)
{
    *self = *src;
}
void cobj_SliceVTIter_move(cobj_SliceVTIter *const self, cobj_SliceVTIter *const src)
{
    cobj_SliceVTIter_move_member(self, src);
    cobj_SliceVTIter_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceVTIter_try_copy(cobj_SliceVTIter *const self,
                                                  cobj_SliceVTIter const *const src,
                                                  cobj_Error *const err)
{
    return cobj_SliceVIter_try_copy(&self->inner, &src->inner, err);
}

T const *cobj_SliceVTIter_next(cobj_SliceVTIter *const self)
{
    return cobj_SliceVIter_next(&self->inner, sizeof(T));
}

void cobj_SliceVTIter_new(cobj_SliceVTIter *const self, T const *const b, T const *const e)
{
    cobj_SliceVIter_new(&self->inner, b, e);
}

//============================================================================

static void *cobj_SliceVTMut_elem_ptr(void *const arr, size_t idx)
{
    return &((T *)arr)[idx];
}

void cobj_SliceVTMut_wipe(cobj_SliceVTMut *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceVTMut_destroy_member(cobj_SliceVTMut *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceVTMut_destroy(cobj_SliceVTMut *const self)
{
    cobj_SliceVTMut_destroy_member(self);
    cobj_SliceVTMut_wipe(self);
}

void cobj_SliceVTMut_move_member(cobj_SliceVTMut *const self, cobj_SliceVTMut *const src)
{
    *self = *src;
}
void cobj_SliceVTMut_move(cobj_SliceVTMut *const self, cobj_SliceVTMut *const src)
{
    cobj_SliceVTMut_move_member(self, src);
    cobj_SliceVTMut_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceVTMut_try_copy(cobj_SliceVTMut *self,
                                                 cobj_SliceVTMut const *const src,
                                                 cobj_Error *const err)
{
    return cobj_SliceVMut_try_copy(&self->inner, &src->inner, err);
}

void cobj_SliceVTMut_new(cobj_SliceVTMut *const self, T *const ptr, size_t len)
{
    cobj_SliceVMut_new(&self->inner, ptr, len);
}

size_t cobj_SliceVTMut_len(cobj_SliceVTMut const *const self)
{
    return cobj_SliceVMut_len(&self->inner);
}

bool cobj_SliceVTMut_is_empty(cobj_SliceVTMut const *const self)
{
    return cobj_SliceVMut_is_empty(&self->inner);
}

T *cobj_SliceVTMut_try_get(cobj_SliceVTMut const *const self, size_t pos)
{
    return cobj_SliceVMut_try_get(&self->inner, pos, cobj_SliceVTMut_elem_ptr);
}

bool cobj_SliceVTMut_try_subslice(cobj_SliceVTMut const *const self,
                                  size_t b,
                                  size_t e,
                                  cobj_SliceVT *const dest,
                                  cobj_Error *const err)
{
    return cobj_SliceVMut_try_subslice(&self->inner,
                                       b,
                                       e,
                                       &dest->inner,
                                       err,
                                       cobj_SliceVTMut_elem_ptr);
}

bool cobj_SliceVTMut_try_subslice_mut(cobj_SliceVTMut const *const self,
                                      size_t b,
                                      size_t e,
                                      cobj_SliceVTMut *const dest,
                                      cobj_Error *const err)
{
    return cobj_SliceVMut_try_subslice_mut(&self->inner,
                                           b,
                                           e,
                                           &dest->inner,
                                           err,
                                           cobj_SliceVTMut_elem_ptr);
}

void cobj_SliceVTMut_as_cobj_SliceVT(cobj_SliceVTMut const *const self, cobj_SliceVT *const s)
{
    cobj_SliceVMut_as_cobj_SliceV(&self->inner, &s->inner);
}

bool WARN_UNUSED_RESULT cobj_SliceVTMut_try_move_from(cobj_SliceVTMut const *const self,
                                                      cobj_SliceVTMut const *const src,
                                                      cobj_Error *const err)
{
    // move content of one slice into another.
    // before: src must be initialised pointing to initialised, self must be initialised to
    // uninitialised. after: src is initialised to de-initialised, self is initialised to
    // initialised. both must be same size..
    return cobj_SliceVMut_try_move_from(&self->inner, &src->inner, err, cobj_SliceVTMut_elem_ptr);
}

bool WARN_UNUSED_RESULT cobj_SliceVTMut_try_copy_from(cobj_SliceVTMut const *const self,
                                                      cobj_SliceVT const *const src,
                                                      cobj_Error *const err)
{
    // copy content of one slice into another.
    // before: src must be initialised pointing to initialised, self must be initialised to
    // uninitialised. after: src is initialised to initialised, self is initialised to initialised.
    // both must be same size..
    return cobj_SliceVMut_try_copy_from(&self->inner,
                                        &src->inner,
                                        err,
                                        cobj_SliceVT_elem_ptr,
                                        cobj_SliceVT_elem_ptr_mut,
                                        cobj_SliceVT_elem_try_copy,
                                        cobj_SliceVT_elem_destroy);
}

void cobj_SliceVTMut_iter(cobj_SliceVTMut const *const self, cobj_SliceVTIter *const it)
{
    cobj_SliceVMut_iter(&self->inner, &it->inner, cobj_SliceVTMut_elem_ptr);
}

void cobj_SliceVTMut_iter_mut(cobj_SliceVTMut const *const self, cobj_SliceVTMutIter *const it)
{
    cobj_SliceVMut_iter_mut(&self->inner, &it->inner, cobj_SliceVTMut_elem_ptr);
}

// ============================================================================
void cobj_SliceVTMutIter_wipe(cobj_SliceVTMutIter *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceVTMutIter_destroy_member(cobj_SliceVTMutIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceVTMutIter_destroy(cobj_SliceVTMutIter *const self)
{
    cobj_SliceVTMutIter_destroy_member(self);
    cobj_SliceVTMutIter_wipe(self);
}

void cobj_SliceVTMutIter_move_member(cobj_SliceVTMutIter *const self,
                                     cobj_SliceVTMutIter *const src)
{
    *self = *src;
}
void cobj_SliceVTMutIter_move(cobj_SliceVTMutIter *const self, cobj_SliceVTMutIter *const src)
{
    cobj_SliceVTMutIter_move_member(self, src);
    cobj_SliceVTMutIter_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceVTMutIter_try_copy(cobj_SliceVTMutIter *const self,
                                                     cobj_SliceVTMutIter const *const src,
                                                     cobj_Error *const err)
{
    return cobj_SliceVMutIter_try_copy(&self->inner, &src->inner, err);
}

T *cobj_SliceVTMutIter_next(cobj_SliceVTMutIter *const self)
{
    return cobj_SliceVMutIter_next(&self->inner, sizeof(T));
}

void cobj_SliceVTMutIter_new(cobj_SliceVTMutIter *const self, T *const b, T *const e)
{
    cobj_SliceVMutIter_new(&self->inner, b, e);
}
