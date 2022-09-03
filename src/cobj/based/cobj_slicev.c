#include <cobj/based/cobj_carrayv.h>
#include <cobj/based/cobj_slicev.h>
#include <cobj/core/cobj_defs.h> // COBJ_UNUSED_ARG
#include <cobj/core/cobj_memory.h>
#include <inttypes.h> //  uint8_t
#include <memory.h> // memmove

void SliceV_wipe(SliceV *const self)
{
    STRUCTWIPE(self);
}

void SliceV_destroy_member(SliceV *const self)
{
    COBJ_UNUSED_ARG(self);
}
void SliceV_destroy(SliceV *const self)
{
    SliceV_destroy_member(self);
    SliceV_wipe(self);
}

void SliceV_move_member(SliceV *const self, SliceV *const src)
{
    *self = *src;
}

void SliceV_move(SliceV *const self, SliceV *const src)
{
    SliceV_move_member(self, src);
    SliceV_wipe(src);
}

bool WARN_UNUSED_RESULT SliceV_try_copy(SliceV *self, SliceV const *const src, Error *err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void SliceV_new(SliceV *const self, void const *ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

void const *SliceV_ptr(SliceV const *const self)
{
    return self->ptr;
}
size_t SliceV_len(SliceV const *const self)
{
    return self->len;
}

bool SliceV_is_empty(SliceV const *const self)
{
    return self->len == 0;
}

void const *SliceV_try_get(SliceV const *const self,
                           size_t pos,
                           void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    if (pos > self->len) { return NULL; }
    return elem_ptr(self->ptr, pos);
}

bool SliceV_try_subslice(SliceV const *const self,
                         size_t b,
                         size_t e,
                         SliceV *const dest,
                         Error *err,
                         void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    if (b > e) { return ERROR_RAISE(err, Error_EFAIL); }
    if (b > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    if (e > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    SliceV_new(dest, elem_ptr(self->ptr, b), e - b);
    return true;
}

void SliceV_iter(SliceV const *const self,
                 SliceVIter *const it,
                 void const *(*elem_ptr)(void const *const arr, size_t idx))
{
    SliceVIter_new(it, self->ptr, elem_ptr(self->ptr, self->len));
}

// =============================================================================

void SliceVIter_wipe(SliceVIter *const self)
{
    STRUCTWIPE(self);
}

void SliceVIter_destroy_member(SliceVIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void SliceVIter_destroy(SliceVIter *const self)
{
    SliceVIter_destroy_member(self);
    SliceVIter_wipe(self);
}

void SliceVIter_move_member(SliceVIter *const self, SliceVIter *const src)
{
    *self = *src;
}
void SliceVIter_move(SliceVIter *const self, SliceVIter *const src)
{
    SliceVIter_move_member(self, src);
    SliceVIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVIter_try_copy(SliceVIter *const self,
                                            SliceVIter const *const src,
                                            Error *const err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void const *SliceVIter_next(SliceVIter *const self, size_t elem_size)
{
    if (self->p >= self->e) { return NULL; }
    void const *const p = self->p;
    self->p += elem_size;
    return p;
}

// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len

void SliceVIter_new(SliceVIter *const self, void const *const b, void const *const e)
{
    self->p = b;
    self->e = e;
}

//============================================================================

void SliceVMut_wipe(SliceVMut *const self)
{
    STRUCTWIPE(self);
}

void SliceVMut_destroy_member(SliceVMut *const self)
{
    COBJ_UNUSED_ARG(self);
}
void SliceVMut_destroy(SliceVMut *const self)
{
    SliceVMut_destroy_member(self);
    SliceVMut_wipe(self);
}

void SliceVMut_move_member(SliceVMut *const self, SliceVMut *const src)
{
    *self = *src;
}
void SliceVMut_move(SliceVMut *const self, SliceVMut *const src)
{
    SliceVMut_move_member(self, src);
    SliceVMut_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVMut_try_copy(SliceVMut *self, SliceVMut const *const src, Error *err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void SliceVMut_new(SliceVMut *const self, void *const ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

void *SliceVMut_ptr(SliceVMut const *const self)
{
    return self->ptr;
}

size_t SliceVMut_len(SliceVMut const *const self)
{
    return self->len;
}

bool SliceVMut_is_empty(SliceVMut const *const self)
{
    return self->len == 0;
}

void *SliceVMut_try_get(SliceVMut const *const self,
                        size_t idx,
                        void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    if (idx >= self->len) { return NULL; }
    return elem_ptr_mut(self->ptr, idx);
}

bool SliceVMut_try_subslice(SliceVMut const *const self,
                            size_t b,
                            size_t e,
                            SliceV *const dest,
                            Error *err,
                            void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    if (b > e) { return ERROR_RAISE(err, Error_EFAIL); }
    if (b > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    if (e > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    SliceV_new(dest, elem_ptr_mut(self->ptr, b), e - b);
    return true;
}

bool SliceVMut_try_subslice_mut(SliceVMut const *const self,
                                size_t b,
                                size_t e,
                                SliceVMut *const dest,
                                Error *err,
                                void *(*elem_ptr)(void *arr, size_t idx))
{
    if (b > e) { return ERROR_RAISE(err, Error_EFAIL); }
    if (b > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    if (e > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    SliceVMut_new(dest, elem_ptr(self->ptr, b), e - b);
    return true;
}

void SliceVMut_as_SliceV(SliceVMut const *const self, SliceV *const s)
{
    SliceV_new(s, self->ptr, self->len);
}

bool WARN_UNUSED_RESULT SliceVMut_try_move_from(SliceVMut const *const self,
                                                SliceVMut const *const src,
                                                Error *const err,
                                                void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    // move contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (self->len != src->len) { return ERROR_RAISE(err, Error_EFAIL); }

    CArrayV_move(self->ptr, self->len, src->ptr, elem_ptr_mut);
    return true;
}

bool WARN_UNUSED_RESULT
SliceVMut_try_copy_from(SliceVMut const *const self,
                        SliceV const *const src,
                        Error *const err,
                        void const *(*elem_get)(void const *const arr, size_t idx),
                        void *(*elem_get_mut)(void *const arr, size_t idx),
                        bool (*elem_try_copy)(void *const d, void const *const s, Error *err),
                        void (*elem_destroy)(void *))
{
    // copy contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (self->len != src->len) { return ERROR_RAISE(err, Error_EFAIL); }

    return CArrayV_try_copy(self->ptr,
                            self->len,
                            src->ptr,
                            err,
                            elem_get,
                            elem_get_mut,
                            elem_try_copy,
                            elem_destroy);
}

void SliceVMut_iter(SliceVMut const *const self,
                    SliceVIter *const it,
                    void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    SliceVIter_new(it, self->ptr, elem_ptr_mut(self->ptr, self->len));
}

void SliceVMut_iter_mut(SliceVMut const *const self,
                        SliceVMutIter *const it,
                        void *(*elem_ptr_mut)(void *const arr, size_t idx))
{
    SliceVMutIter_new(it, self->ptr, elem_ptr_mut(self->ptr, self->len));
}

// ============================================================================
void SliceVMutIter_wipe(SliceVMutIter *const self)
{
    STRUCTWIPE(self);
}

void SliceVMutIter_destroy_member(SliceVMutIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void SliceVMutIter_destroy(SliceVMutIter *const self)
{
    SliceVMutIter_destroy_member(self);
    SliceVMutIter_wipe(self);
}

void SliceVMutIter_move_member(SliceVMutIter *const self, SliceVMutIter *const src)
{
    *self = *src;
}
void SliceVMutIter_move(SliceVMutIter *const self, SliceVMutIter *const src)
{
    SliceVMutIter_move_member(self, src);
    SliceVMutIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVMutIter_try_copy(SliceVMutIter *const self,
                                               SliceVMutIter const *const src,
                                               Error *err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void *SliceVMutIter_next(SliceVMutIter *const self, size_t elem_size)
{
    if (self->p >= self->e) { return NULL; }
    void *const p = self->p;
    self->p += elem_size;
    return p;
}
// next is faster if using pointers (no mult every time)
// so for iters, use b+e not ptr+len

void SliceVMutIter_new(SliceVMutIter *const self, void *const b, void *const e)
{
    self->p = b;
    self->e = e;
}
