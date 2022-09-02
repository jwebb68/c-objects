#include "cobj_slicet.h"

#include "cobj_carrayt.h"
#include "cobj_defs.h" // UNUSED_ARG
#include "cobj_memory.h"

#include <memory.h> // memmove

static void SliceT_wipe(SliceT *const self)
{
    STRUCTWIPE(self);
}

void SliceT_destroy_member(SliceT *const self)
{
    UNUSED_ARG(self);
}
void SliceT_destroy(SliceT *const self)
{
    SliceT_destroy_member(self);
    SliceT_wipe(self);
}

void SliceT_move_member(SliceT *const self, SliceT *const src)
{
    *self = *src;
}
void SliceT_move(SliceT *const self, SliceT *const src)
{
    SliceT_move_member(self, src);
    SliceT_wipe(src);
}

bool WARN_UNUSED_RESULT SliceT_try_copy(SliceT *self, SliceT const *const src, Error *const err)
{
    UNUSED_ARG(err);
    *self = *src;
    return true;
}

void SliceT_new(SliceT *const self, T const *ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

size_t SliceT_len(SliceT const *const self)
{
    return self->len;
}

bool SliceT_is_empty(SliceT const *const self)
{
    return self->len == 0;
}

T const *SliceT_try_get(SliceT const *const self, size_t pos)
{
    return CArrayT_try_get(self->ptr, self->len, pos);
}

bool SliceT_try_subslice(SliceT const *const self,
                         size_t b,
                         size_t e,
                         SliceT *const dest,
                         Error *const err)
{
    if (b > e) { return ERROR_RAISE(err, Error_EFAIL); }
    if (b > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    if (e > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    SliceT_new(dest, self->ptr + b, e - b);
    return true;
}

void SliceT_iter(SliceT const *const self, SliceTIter *const it)
{
    SliceTIter_new(it, self->ptr, self->ptr + self->len);
}

// =============================================================================

static void SliceTIter_wipe(SliceTIter *const self)
{
    STRUCTWIPE(self);
}

void SliceTIter_destroy_member(SliceTIter *const self)
{
    UNUSED_ARG(self);
}
void SliceTIter_destroy(SliceTIter *const self)
{
    SliceTIter_destroy_member(self);
    SliceTIter_wipe(self);
}

void SliceTIter_move_member(SliceTIter *const self, SliceTIter *const src)
{
    *self = *src;
}
void SliceTIter_move(SliceTIter *const self, SliceTIter *const src)
{
    SliceTIter_move_member(self, src);
    SliceTIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceTIter_try_copy(SliceTIter *const self, SliceTIter const *const src)
{
    *self = *src;
    return true;
}

T const *SliceTIter_next(SliceTIter *const self)
{
    if (self->p >= self->e) { return NULL; }
    T const *p = self->p;
    self->p += 1;
    return p;
}

void SliceTIter_new(SliceTIter *const self, T const *const b, T const *const e)
{
    self->p = b;
    self->e = e;
}

//============================================================================

void SliceTMut_wipe(SliceTMut *const self)
{
    STRUCTWIPE(self);
}

void SliceTMut_destroy_member(SliceTMut *const self)
{
    UNUSED_ARG(self);
}
void SliceTMut_destroy(SliceTMut *const self)
{
    SliceTMut_destroy_member(self);
    SliceTMut_wipe(self);
}

void SliceTMut_move_member(SliceTMut *const self, SliceTMut *const src)
{
    *self = *src;
}
void SliceTMut_move(SliceTMut *const self, SliceTMut *const src)
{
    SliceTMut_move_member(self, src);
    SliceTMut_wipe(src);
}

bool WARN_UNUSED_RESULT SliceTMut_try_copy(SliceTMut *self,
                                           SliceTMut const *const src,
                                           Error *const err)
{
    UNUSED_ARG(err);
    *self = *src;
    return true;
}

void SliceTMut_new(SliceTMut *const self, T *const ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

size_t SliceTMut_len(SliceTMut const *const self)
{
    return self->len;
}

bool SliceTMut_is_empty(SliceTMut const *const self)
{
    return self->len == 0;
}

T *SliceTMut_try_get(SliceTMut const *const self, size_t pos)
{
    return CArrayTMut_try_get(self->ptr, self->len, pos);
}

bool SliceTMut_try_subslice(SliceTMut const *const self,
                            size_t b,
                            size_t e,
                            SliceT *const dest,
                            Error *const err)
{
    if (b > e) { return ERROR_RAISE(err, Error_EFAIL); }
    if (b > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    if (e > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    SliceT_new(dest, self->ptr + b, e - b);
    return true;
}

bool SliceTMut_try_subslice_mut(SliceTMut const *const self,
                                size_t b,
                                size_t e,
                                SliceTMut *const dest,
                                Error *const err)
{
    if (b > e) { return ERROR_RAISE(err, Error_EFAIL); }
    if (b > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    if (e > self->len) { return ERROR_RAISE(err, Error_EFAIL); }
    SliceTMut_new(dest, self->ptr + b, e - b);
    return true;
}

void SliceTMut_as_SliceT(SliceTMut const *const self, SliceT *const s)
{
    SliceT_new(s, self->ptr, self->len);
}

bool WARN_UNUSED_RESULT SliceTMut_try_move_from(SliceTMut *const self,
                                                SliceTMut const *const src,
                                                Error *const err)
{
    // move contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (self->len != src->len) { return ERROR_RAISE(err, Error_EFAIL); }

    CArrayTMut_move(self->ptr, self->len, src->ptr);
    return true;
}

bool WARN_UNUSED_RESULT SliceTMut_try_copy_from(SliceTMut *const self,
                                                SliceT const *const src,
                                                Error *const err)
{
    // copy contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (self->len != src->len) { return ERROR_RAISE(err, Error_EFAIL); }

    return CArrayTMut_try_copy(self->ptr, self->len, src->ptr, err);
}

void SliceTMut_iter(SliceTMut const *const self, SliceTIter *const it)
{
    SliceTIter_new(it, self->ptr, self->ptr + self->len);
}

void SliceTMut_iter_mut(SliceTMut const *const self, SliceTMutIter *const it)
{
    SliceTMutIter_new(it, self->ptr, self->ptr + self->len);
}

// ============================================================================
void SliceTMutIter_wipe(SliceTMutIter *const self)
{
    STRUCTWIPE(self);
}

void SliceTMutIter_destroy_member(SliceTMutIter *const self)
{
    UNUSED_ARG(self);
}
void SliceTMutIter_destroy(SliceTMutIter *const self)
{
    SliceTMutIter_destroy_member(self);
    SliceTMutIter_wipe(self);
}

void SliceTMutIter_move_member(SliceTMutIter *const self, SliceTMutIter *const src)
{
    *self = *src;
}
void SliceTMutIter_move(SliceTMutIter *const self, SliceTMutIter *const src)
{
    SliceTMutIter_move_member(self, src);
    SliceTMutIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceTMutIter_try_copy(SliceTMutIter *const self,
                                               SliceTMutIter const *const src)
{
    *self = *src;
    return true;
}

T *SliceTMutIter_next(SliceTMutIter *const self)
{
    if (self->p >= self->e) { return NULL; }
    T *p = self->p;
    self->p += 1;
    return p;
}

void SliceTMutIter_new(SliceTMutIter *const self, T *const b, T *const e)
{
    self->p = b;
    self->e = e;
}
