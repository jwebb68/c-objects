#include <cobj/baseless/cobj_carrayt.h>
#include <cobj/baseless/cobj_slicet.h>
#include <cobj/core/cobj_defs.h> // COBJ_UNUSED_ARG
#include <cobj/core/cobj_memory.h>
#include <memory.h> // memmove

static void cobj_SliceT_wipe(cobj_SliceT *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceT_destroy_member(cobj_SliceT *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceT_destroy(cobj_SliceT *const self)
{
    cobj_SliceT_destroy_member(self);
    cobj_SliceT_wipe(self);
}

void cobj_SliceT_move_member(cobj_SliceT *const self, cobj_SliceT *const src)
{
    *self = *src;
}
void cobj_SliceT_move(cobj_SliceT *const self, cobj_SliceT *const src)
{
    cobj_SliceT_move_member(self, src);
    cobj_SliceT_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceT_try_copy(cobj_SliceT *self,
                                             cobj_SliceT const *const src,
                                             cobj_Error *const err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void cobj_SliceT_new(cobj_SliceT *const self, T const *ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

size_t cobj_SliceT_len(cobj_SliceT const *const self)
{
    return self->len;
}

bool cobj_SliceT_is_empty(cobj_SliceT const *const self)
{
    return self->len == 0;
}

T const *cobj_SliceT_try_get(cobj_SliceT const *const self, size_t pos)
{
    return cobj_CArrayT_try_get(self->ptr, self->len, pos);
}

bool cobj_SliceT_try_subslice(cobj_SliceT const *const self,
                              size_t b,
                              size_t e,
                              cobj_SliceT *const dest,
                              cobj_Error *const err)
{
    if (b > e) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (b > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (e > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    cobj_SliceT_new(dest, self->ptr + b, e - b);
    return true;
}

void cobj_SliceT_iter(cobj_SliceT const *const self, cobj_SliceTIter *const it)
{
    cobj_SliceTIter_new(it, self->ptr, self->ptr + self->len);
}

// =============================================================================

static void cobj_SliceTIter_wipe(cobj_SliceTIter *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceTIter_destroy_member(cobj_SliceTIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceTIter_destroy(cobj_SliceTIter *const self)
{
    cobj_SliceTIter_destroy_member(self);
    cobj_SliceTIter_wipe(self);
}

void cobj_SliceTIter_move_member(cobj_SliceTIter *const self, cobj_SliceTIter *const src)
{
    *self = *src;
}
void cobj_SliceTIter_move(cobj_SliceTIter *const self, cobj_SliceTIter *const src)
{
    cobj_SliceTIter_move_member(self, src);
    cobj_SliceTIter_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceTIter_try_copy(cobj_SliceTIter *const self,
                                                 cobj_SliceTIter const *const src)
{
    *self = *src;
    return true;
}

T const *cobj_SliceTIter_next(cobj_SliceTIter *const self)
{
    if (self->p >= self->e) { return NULL; }
    T const *p = self->p;
    self->p += 1;
    return p;
}

void cobj_SliceTIter_new(cobj_SliceTIter *const self, T const *const b, T const *const e)
{
    self->p = b;
    self->e = e;
}

//============================================================================

void cobj_SliceTMut_wipe(cobj_SliceTMut *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceTMut_destroy_member(cobj_SliceTMut *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceTMut_destroy(cobj_SliceTMut *const self)
{
    cobj_SliceTMut_destroy_member(self);
    cobj_SliceTMut_wipe(self);
}

void cobj_SliceTMut_move_member(cobj_SliceTMut *const self, cobj_SliceTMut *const src)
{
    *self = *src;
}
void cobj_SliceTMut_move(cobj_SliceTMut *const self, cobj_SliceTMut *const src)
{
    cobj_SliceTMut_move_member(self, src);
    cobj_SliceTMut_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceTMut_try_copy(cobj_SliceTMut *self,
                                                cobj_SliceTMut const *const src,
                                                cobj_Error *const err)
{
    COBJ_UNUSED_ARG(err);
    *self = *src;
    return true;
}

void cobj_SliceTMut_new(cobj_SliceTMut *const self, T *const ptr, size_t len)
{
    self->ptr = ptr;
    self->len = len;
}

size_t cobj_SliceTMut_len(cobj_SliceTMut const *const self)
{
    return self->len;
}

bool cobj_SliceTMut_is_empty(cobj_SliceTMut const *const self)
{
    return self->len == 0;
}

T *cobj_SliceTMut_try_get(cobj_SliceTMut const *const self, size_t pos)
{
    return cobj_CArrayTMut_try_get(self->ptr, self->len, pos);
}

bool cobj_SliceTMut_try_subslice(cobj_SliceTMut const *const self,
                                 size_t b,
                                 size_t e,
                                 cobj_SliceT *const dest,
                                 cobj_Error *const err)
{
    if (b > e) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (b > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (e > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    cobj_SliceT_new(dest, self->ptr + b, e - b);
    return true;
}

bool cobj_SliceTMut_try_subslice_mut(cobj_SliceTMut const *const self,
                                     size_t b,
                                     size_t e,
                                     cobj_SliceTMut *const dest,
                                     cobj_Error *const err)
{
    if (b > e) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (b > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    if (e > self->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }
    cobj_SliceTMut_new(dest, self->ptr + b, e - b);
    return true;
}

void cobj_SliceTMut_as_cobj_SliceT(cobj_SliceTMut const *const self, cobj_SliceT *const s)
{
    cobj_SliceT_new(s, self->ptr, self->len);
}

bool WARN_UNUSED_RESULT cobj_SliceTMut_try_move_from(cobj_SliceTMut *const self,
                                                     cobj_SliceTMut const *const src,
                                                     cobj_Error *const err)
{
    // move contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (self->len != src->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }

    cobj_CArrayTMut_move(self->ptr, self->len, src->ptr);
    return true;
}

bool WARN_UNUSED_RESULT cobj_SliceTMut_try_copy_from(cobj_SliceTMut *const self,
                                                     cobj_SliceT const *const src,
                                                     cobj_Error *const err)
{
    // copy contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (self->len != src->len) { return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EFAIL); }

    return cobj_CArrayTMut_try_copy(self->ptr, self->len, src->ptr, err);
}

void cobj_SliceTMut_iter(cobj_SliceTMut const *const self, cobj_SliceTIter *const it)
{
    cobj_SliceTIter_new(it, self->ptr, self->ptr + self->len);
}

void cobj_SliceTMut_iter_mut(cobj_SliceTMut const *const self, cobj_SliceTMutIter *const it)
{
    cobj_SliceTMutIter_new(it, self->ptr, self->ptr + self->len);
}

// ============================================================================
void cobj_SliceTMutIter_wipe(cobj_SliceTMutIter *const self)
{
    STRUCTWIPE(self);
}

void cobj_SliceTMutIter_destroy_member(cobj_SliceTMutIter *const self)
{
    COBJ_UNUSED_ARG(self);
}
void cobj_SliceTMutIter_destroy(cobj_SliceTMutIter *const self)
{
    cobj_SliceTMutIter_destroy_member(self);
    cobj_SliceTMutIter_wipe(self);
}

void cobj_SliceTMutIter_move_member(cobj_SliceTMutIter *const self, cobj_SliceTMutIter *const src)
{
    *self = *src;
}
void cobj_SliceTMutIter_move(cobj_SliceTMutIter *const self, cobj_SliceTMutIter *const src)
{
    cobj_SliceTMutIter_move_member(self, src);
    cobj_SliceTMutIter_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_SliceTMutIter_try_copy(cobj_SliceTMutIter *const self,
                                                    cobj_SliceTMutIter const *const src)
{
    *self = *src;
    return true;
}

T *cobj_SliceTMutIter_next(cobj_SliceTMutIter *const self)
{
    if (self->p >= self->e) { return NULL; }
    T *p = self->p;
    self->p += 1;
    return p;
}

void cobj_SliceTMutIter_new(cobj_SliceTMutIter *const self, T *const b, T *const e)
{
    self->p = b;
    self->e = e;
}
