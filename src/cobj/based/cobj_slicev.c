#include <cobj/based/cobj_carrayv.h>
#include <cobj/based/cobj_slicev.h>
#include <cobj/core/cobj_defs.h> // COBJ_UNUSED_ARG
#include <cobj/core/cobj_memory.h>
#include <inttypes.h> //  uint8_t
#include <memory.h> // memmove

void cobj_SliceV_wipe(cobj_SliceV *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceV_destroy_member(cobj_SliceV *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceV_destroy(cobj_SliceV *const self)
{
    cobj_SliceV_destroy_member(self);
    cobj_SliceV_wipe(self);
}

void cobj_SliceV_move_member(cobj_SliceV *const self, cobj_SliceV *const src)
{
    *self = *src;
}

void cobj_SliceV_move(cobj_SliceV *const self, cobj_SliceV *const src)
{
    cobj_SliceV_move_member(self, src);
    cobj_SliceV_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceV_try_copy(cobj_SliceV *self,
                                             cobj_SliceV const *const src,
                                             cobj_Error *err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void cobj_SliceV_new(cobj_SliceV *const self, void const *ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

void const *cobj_SliceV_ptr(cobj_SliceV const *const self)
{
    return self->ptr;
}
size_t cobj_SliceV_len(cobj_SliceV const *const self)
{
    return self->len;
}

bool cobj_SliceV_is_empty(cobj_SliceV const *const self)
{
    return self->len == 0;
}

void const *cobj_SliceV_try_get(cobj_SliceV const *const self,
                                size_t pos,
                                void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    if (pos > self->len) { return NULL; }
    return elem_ptr(self->ptr, pos);
}

bool cobj_SliceV_try_subslice(cobj_SliceV const *const self,
                              size_t b,
                              size_t e,
                              cobj_SliceV *const dest,
                              cobj_Error *err,
                              void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    if (b > e) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (b > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (e > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    cobj_SliceV_new(dest, elem_ptr(self->ptr, b), e - b);
    return true;
}

void cobj_SliceV_iter(cobj_SliceV const *const self,
                      cobj_SliceVIter *const it,
                      void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    cobj_SliceVIter_new(it, self->ptr, elem_ptr(self->ptr, self->len));
}

// =============================================================================

void cobj_SliceVIter_wipe(cobj_SliceVIter *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceVIter_destroy_member(cobj_SliceVIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceVIter_destroy(cobj_SliceVIter *const self)
{
    cobj_SliceVIter_destroy_member(self);
    cobj_SliceVIter_wipe(self);
}

void cobj_SliceVIter_move_member(cobj_SliceVIter *const self, cobj_SliceVIter *const src)
{
    *self = *src;
}
void cobj_SliceVIter_move(cobj_SliceVIter *const self, cobj_SliceVIter *const src)
{
    cobj_SliceVIter_move_member(self, src);
    cobj_SliceVIter_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceVIter_try_copy(cobj_SliceVIter *const self,
                                                 cobj_SliceVIter const *const src,
                                                 cobj_Error *const err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void const *cobj_SliceVIter_next(cobj_SliceVIter *const self, size_t elem_size)
{
    if (self->p >= self->e) { return NULL; }
    void const *const p = self->p;
    self->p += elem_size;
    return p;
}

// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len

void cobj_SliceVIter_new(cobj_SliceVIter *const self, void const *const b, void const *const e)
{
    self->p = b;
    self->e = e;
}

//============================================================================

void cobj_SliceVMut_wipe(cobj_SliceVMut *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceVMut_destroy_member(cobj_SliceVMut *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceVMut_destroy(cobj_SliceVMut *const self)
{
    cobj_SliceVMut_destroy_member(self);
    cobj_SliceVMut_wipe(self);
}

void cobj_SliceVMut_move_member(cobj_SliceVMut *const self, cobj_SliceVMut *const src)
{
    *self = *src;
}
void cobj_SliceVMut_move(cobj_SliceVMut *const self, cobj_SliceVMut *const src)
{
    cobj_SliceVMut_move_member(self, src);
    cobj_SliceVMut_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceVMut_try_copy(cobj_SliceVMut *self,
                                                cobj_SliceVMut const *const src,
                                                cobj_Error *err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void cobj_SliceVMut_new(cobj_SliceVMut *const self, void *const ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

void *cobj_SliceVMut_ptr(cobj_SliceVMut const *const self)
{
    return self->ptr;
}

size_t cobj_SliceVMut_len(cobj_SliceVMut const *const self)
{
    return self->len;
}

bool cobj_SliceVMut_is_empty(cobj_SliceVMut const *const self)
{
    return self->len == 0;
}

void *cobj_SliceVMut_try_get(cobj_SliceVMut const *const self,
                             size_t idx,
                             void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    if (idx >= self->len) { return NULL; }
    return elem_ptr_mut(self->ptr, idx);
}

bool cobj_SliceVMut_try_subslice(cobj_SliceVMut const *const self,
                                 size_t b,
                                 size_t e,
                                 cobj_SliceV *const dest,
                                 cobj_Error *err,
                                 void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    if (b > e) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (b > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (e > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    cobj_SliceV_new(dest, elem_ptr_mut(self->ptr, b), e - b);
    return true;
}

bool cobj_SliceVMut_try_subslice_mut(cobj_SliceVMut const *const self,
                                     size_t b,
                                     size_t e,
                                     cobj_SliceVMut *const dest,
                                     cobj_Error *err,
                                     void *(*elem_ptr)(void *arr, size_t idx))
{
    if (b > e) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (b > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (e > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    cobj_SliceVMut_new(dest, elem_ptr(self->ptr, b), e - b);
    return true;
}

void cobj_SliceVMut_as_cobj_SliceV(cobj_SliceVMut const *const self, cobj_SliceV *const s)
{
    cobj_SliceV_new(s, self->ptr, self->len);
}

bool WARN_UNUSED_RESULT cobj_SliceVMut_try_move_from(cobj_SliceVMut const *const self,
                                                     cobj_SliceVMut const *const src,
                                                     cobj_Error *const err,
                                                     void *(*elem_ptr_mut)(void *const arr,
                                                                           size_t idx))
{
    // move contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (self->len != src->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }

    cobj_CArrayV_move(self->ptr, self->len, src->ptr, elem_ptr_mut);
    return true;
}

bool WARN_UNUSED_RESULT cobj_SliceVMut_try_copy_from(
    cobj_SliceVMut const *const self,
    cobj_SliceV const *const src,
    cobj_Error *const err,
    void const *(*elem_get)(void const *const arr, size_t idx),
    void *(*elem_get_mut)(void *const arr, size_t idx),
    bool (*elem_try_copy)(void *const d, void const *const s, cobj_Error *err),
    void (*elem_destroy)(void *))
{
    // copy contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (self->len != src->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }

    return cobj_CArrayV_try_copy(self->ptr,
                                 self->len,
                                 src->ptr,
                                 err,
                                 elem_get,
                                 elem_get_mut,
                                 elem_try_copy,
                                 elem_destroy);
}

void cobj_SliceVMut_iter(cobj_SliceVMut const *const self,
                         cobj_SliceVIter *const it,
                         void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    cobj_SliceVIter_new(it, self->ptr, elem_ptr_mut(self->ptr, self->len));
}

void cobj_SliceVMut_iter_mut(cobj_SliceVMut const *const self,
                             cobj_SliceVMutIter *const it,
                             void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    cobj_SliceVMutIter_new(it, self->ptr, elem_ptr_mut(self->ptr, self->len));
}

// ============================================================================
void cobj_SliceVMutIter_wipe(cobj_SliceVMutIter *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceVMutIter_destroy_member(cobj_SliceVMutIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceVMutIter_destroy(cobj_SliceVMutIter *const self)
{
    cobj_SliceVMutIter_destroy_member(self);
    cobj_SliceVMutIter_wipe(self);
}

void cobj_SliceVMutIter_move_member(cobj_SliceVMutIter *const self, cobj_SliceVMutIter *const src)
{
    *self = *src;
}
void cobj_SliceVMutIter_move(cobj_SliceVMutIter *const self, cobj_SliceVMutIter *const src)
{
    cobj_SliceVMutIter_move_member(self, src);
    cobj_SliceVMutIter_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceVMutIter_try_copy(cobj_SliceVMutIter *const self,
                                                    cobj_SliceVMutIter const *const src,
                                                    cobj_Error *err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void *cobj_SliceVMutIter_next(cobj_SliceVMutIter *const self, size_t elem_size)
{
    if (self->p >= self->e) { return NULL; }
    void *const p = self->p;
    self->p += elem_size;
    return p;
}
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len

void cobj_SliceVMutIter_new(cobj_SliceVMutIter *const self, void *const b, void *const e)
{
    self->p = b;
    self->e = e;
}
