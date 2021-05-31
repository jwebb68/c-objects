#include "cobj_slicev.h"

#include "cobj_defs.h" // UNUSED_ARG
#include "cobj_memory.h"

#include <inttypes.h> //  uint8_t
#include <memory.h> // memmove

static void SliceV_wipe(SliceV *const self)
{
    STRUCTWIPE(self);
}

void SliceV_destroy(SliceV *const self)
{
    SliceV_wipe(self);
}

void SliceV_move(SliceV *const self, SliceV *const src)
{
    *self = *src;
    SliceV_wipe(src);
}

bool WARN_UNUSED_RESULT SliceV_try_copy(SliceV *self, SliceV const *const src, Error *err)
{
    UNUSED_ARG(err);
    *self = *src;
    return true;
}

void SliceV_new(SliceV *const self, void const *b, void const *e)
{
    self->b = (uint8_t const *)b;
    self->e = (uint8_t const *)e;
}

size_t SliceV_len(SliceV const *const self, size_t elem_size)
{
    return (self->e - self->b) / elem_size;
}

bool SliceV_is_empty(SliceV const *const self)
{
    return self->e == self->b;
}

void const *SliceV_get_item_at(SliceV const *const self, size_t pos, size_t elem_size)
{
    uint8_t const *p = self->b + (pos * elem_size);
    if (p >= self->e) {
        return NULL;
    }
    return p;
}

bool SliceV_try_subslice(SliceV const *const self,
                         size_t b,
                         size_t e,
                         SliceV *const dest,
                         Error *err,
                         size_t elem_size)
{
    if (b > e) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (b > SliceV_len(self, elem_size)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (e > SliceV_len(self, elem_size)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    SliceV_new(dest, self->b + (b * elem_size), self->b + ((e - b) * elem_size));
    return true;
}

void SliceV_iter(SliceV const *const self, SliceVIter *const it)
{
    SliceVIter_new(it, self->b, self->e);
}

// =============================================================================

static void SliceVIter_wipe(SliceVIter *const self)
{
    STRUCTWIPE(self);
}

void SliceVIter_destroy(SliceVIter *const self)
{
    SliceVIter_wipe(self);
}

void SliceVIter_move(SliceVIter *const self, SliceVIter *const src)
{
    *self = *src;
    SliceVIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVIter_try_copy(SliceVIter *const self, SliceVIter const *const src)
{
    *self = *src;
    return true;
}

void const *SliceVIter_next(SliceVIter *const self, size_t elem_size)
{
    uint8_t const *p = self->p;
    if (p == self->e) {
        return NULL;
    }
    self->p += elem_size;
    return p;
}

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

void SliceVMut_destroy(SliceVMut *const self)
{
    SliceVMut_wipe(self);
}

void SliceVMut_move(SliceVMut *const self, SliceVMut *const src)
{
    *self = *src;
    SliceVMut_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVMut_try_copy(SliceVMut *self, SliceVMut const *const src, Error *err)
{
    UNUSED_ARG(err);
    *self = *src;
    return true;
}

void SliceVMut_new(SliceVMut *const self, void *const b, void *const e)
{
    self->b = (uint8_t *)b;
    self->e = (uint8_t *)e;
}

size_t SliceVMut_len(SliceVMut const *const self, size_t elem_size)
{
    return (self->e - self->b) / elem_size;
}

bool SliceVMut_is_empty(SliceVMut const *const self)
{
    return self->e == self->b;
}

bool SliceVMut_try_subslice(SliceVMut const *const self,
                            size_t b,
                            size_t e,
                            SliceV *const dest,
                            Error *err,
                            size_t elem_size)
{
    if (b > e) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (b > SliceVMut_len(self, elem_size)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (e > SliceVMut_len(self, elem_size)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    SliceV_new(dest, self->b + (b * elem_size), self->b + (e - b) * elem_size);
    return true;
}

bool SliceVMut_try_subslice_mut(SliceVMut const *const self,
                                size_t b,
                                size_t e,
                                SliceVMut *const dest,
                                Error *err,
                                size_t elem_size)
{
    if (b > e) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (b > SliceVMut_len(self, elem_size)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (e > SliceVMut_len(self, elem_size)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    SliceVMut_new(dest, self->b + (b * elem_size), self->b + (e - b) * elem_size);
    return true;
}

void SliceVMut_as_slice(SliceVMut const *const self, SliceV *const s)
{
    SliceV_new(s, self->b, self->e);
}

void SliceVMut_as_slice_mut(SliceVMut const *const self, SliceVMut *const s)
{
    SliceVMut_new(s, self->b, self->e);
}

bool WARN_UNUSED_RESULT SliceVMut_try_move_from(SliceVMut *const self,
                                                SliceVMut const *const src,
                                                Error *err,
                                                size_t elem_size)
{
    // move contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (SliceVMut_len(self, elem_size) != SliceVMut_len(src, elem_size)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }

    // self/dest must be uninitialised
    // This will mean a mid-range move will have a very bad contents overall in the dest.
    // After the move, the src will be in a bad state if it's a midrange slice.
    // (an uninitialised block in a block of initialised)
    // for (T *it = self->arr; it != self->arr_end; ++it) {
    //     T_destroy(it);
    // }
    // for (T const *d_it = self->arr, *s_it = src->arr; d_it != self->arr_end; ++d_it, ++s_it)
    // {
    //     T_move(d_it, s_it);
    // }
    // use memmove to move the bits en masse, for speed.
    // bad: memmove src is const, when shouldn't be.
    memmove(self->b, src->b, self->e - self->b);
    memwipe(src->b, src->e - src->b);
    return true;
}

bool WARN_UNUSED_RESULT SliceVMut_try_copy_from(SliceVMut *const self,
                                                SliceV const *const src,
                                                Error *err,
                                                size_t elem_size,
                                                bool (*elem_try_copy)(void *,
                                                                      void const *,
                                                                      Error *),
                                                void (*elem_destroy)(void *))
{
    // copy contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (SliceVMut_len(self, elem_size) != SliceV_len(src, elem_size)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }

    // If initially initialised, does the fail path preserve the orig data?
    // what if this is in the middle of a vec? == deinitialised items in array of initialised.
    // so, copy_from cannot be used to mutate contents of a vec.
    // copy_from cannot be used to populate array
    // This will mean a mid-range copy will have a very bad contents overall in the dest.
    // (an uninitialised block in a block of initialised)
    // for (T *it = self->arr; it != self->arr_end; ++it) {
    //     T_destroy(it);
    // }
    bool ok = true;
    uint8_t *d_it = self->b;
    for (uint8_t const *s_it = src->b; d_it != self->e; d_it += elem_size, s_it += elem_size) {
        ok = elem_try_copy(d_it, s_it, err);
        if (!ok) {
            break;
        }
    }
    if (!ok) {
        for (uint8_t *it = self->b; it != d_it; it += elem_size) {
            elem_destroy(it);
        }
    }
    return ok;
}

void SliceVMut_iter(SliceVMut const *const self, SliceVIter *const it)
{
    SliceVIter_new(it, self->b, self->e);
}

void SliceVMut_iter_mut(SliceVMut const *const self, SliceVMutIter *const it)
{
    SliceVMutIter_new(it, self->b, self->e);
}

// ============================================================================
void SliceVMutIter_wipe(SliceVMutIter *const self)
{
    STRUCTWIPE(self);
}

void SliceVMutIter_destroy(SliceVMutIter *const self)
{
    SliceVMutIter_wipe(self);
}

void SliceVMutIter_move(SliceVMutIter *const self, SliceVMutIter *const src)
{
    *self = *src;
    SliceVMutIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceVMutIter_try_copy(SliceVMutIter *const self,
                                               SliceVMutIter const *const src,
                                               Error *err)
{
    UNUSED_ARG(err);
    *self = *src;
    return true;
}

void *SliceVMutIter_next(SliceVMutIter *const self, size_t elem_size)
{
    uint8_t *p = self->p;
    if (p == self->e) {
        return NULL;
    }
    self->p += elem_size;
    return p;
}

void SliceVMutIter_new(SliceVMutIter *const self, void *const b, void *const e)
{
    self->p = b;
    self->e = e;
}
