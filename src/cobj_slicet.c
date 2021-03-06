#include "cobj_slicet.h"

#include "cobj_defs.h" // UNUSED_ARG
#include "cobj_memory.h"

#include <memory.h> // memmove

static void SliceT_wipe(SliceT *const self)
{
    STRUCTWIPE(self);
}

void SliceT_destroy(SliceT *const self)
{
    SliceT_wipe(self);
}

void SliceT_move(SliceT *const self, SliceT *const src)
{
    *self = *src;
    SliceT_wipe(src);
}

bool WARN_UNUSED_RESULT SliceT_try_copy(SliceT *self, SliceT const *const src, Error *const err)
{
    UNUSED_ARG(err);
    *self = *src;
    return true;
}

void SliceT_new(SliceT *const self, T const *b, T const *e)
{
    self->b = b;
    self->e = e;
}

size_t SliceT_len(SliceT const *const self)
{
    return self->e - self->b;
}

bool SliceT_is_empty(SliceT const *const self)
{
    return self->e == self->b;
}

T const *SliceT_get_item_at(SliceT const *const self, size_t pos)
{
    T const *p = self->b + pos;
    if (p >= self->e) {
        return NULL;
    }
    return p;
}

bool SliceT_try_subslice(SliceT const *const self,
                         size_t b,
                         size_t e,
                         SliceT *const dest,
                         Error *const err)
{
    if (b > e) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (b > SliceT_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (e > SliceT_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    SliceT_new(dest, self->b + b, self->b + (e - b));
    return true;
}

void SliceT_iter(SliceT const *const self, SliceTIter *const it)
{
    SliceTIter_new(it, self->b, self->e);
}

// =============================================================================

static void SliceTIter_wipe(SliceTIter *const self)
{
    STRUCTWIPE(self);
}

void SliceTIter_destroy(SliceTIter *const self)
{
    SliceTIter_wipe(self);
}

void SliceTIter_move(SliceTIter *const self, SliceTIter *const src)
{
    *self = *src;
    SliceTIter_wipe(src);
}

bool WARN_UNUSED_RESULT SliceTIter_try_copy(SliceTIter *const self, SliceTIter const *const src)
{
    *self = *src;
    return true;
}

T const *SliceTIter_next(SliceTIter *const self)
{
    T const *p = self->p;
    if (p == self->e) {
        return NULL;
    }
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

void SliceTMut_destroy(SliceTMut *const self)
{
    SliceTMut_wipe(self);
}

void SliceTMut_move(SliceTMut *const self, SliceTMut *const src)
{
    *self = *src;
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

void SliceTMut_new(SliceTMut *const self, T *const b, T *const e)
{
    self->b = b;
    self->e = e;
}

size_t SliceTMut_len(SliceTMut const *const self)
{
    return self->e - self->b;
}

bool SliceTMut_is_empty(SliceTMut const *const self)
{
    return self->e == self->b;
}

bool SliceTMut_try_subslice(SliceTMut const *const self,
                            size_t b,
                            size_t e,
                            SliceT *const dest,
                            Error *const err)
{
    if (b > e) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (b > SliceTMut_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (e > SliceTMut_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    SliceT_new(dest, self->b + b, self->b + (e - b));
    return true;
}

bool SliceTMut_try_subslice_mut(SliceTMut const *const self,
                                size_t b,
                                size_t e,
                                SliceTMut *const dest,
                                Error *const err)
{
    if (b > e) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (b > SliceTMut_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (e > SliceTMut_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    SliceTMut_new(dest, self->b + b, self->b + (e - b));
    return true;
}

void SliceTMut_as_slice(SliceTMut const *const self, SliceT *const s)
{
    SliceT_new(s, self->b, self->e);
}

bool WARN_UNUSED_RESULT SliceTMut_try_move_from(SliceTMut *const self,
                                                SliceTMut const *const src,
                                                Error *const err)
{
    // move contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (SliceTMut_len(self) != SliceTMut_len(src)) {
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
    memmove(self->b, src->b, (self->e - self->b) * sizeof(T));
    memwipe(src->b, (src->e - src->b) * sizeof(T));
    return true;
}

bool WARN_UNUSED_RESULT SliceTMut_try_copy_from(SliceTMut *const self,
                                                SliceT const *const src,
                                                Error *const err)
{
    // copy contents of one slice into another, both structs must be initialised., what structs
    // refer to must be initialised? or dest must not be initialised.
    if (SliceTMut_len(self) != SliceT_len(src)) {
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
    T *d_it = self->b;
    for (T const *s_it = src->b; d_it != self->e; ++d_it, ++s_it) {
        ok = T_try_copy(d_it, s_it, err);
        if (!ok) {
            break;
        }
    }
    if (!ok) {
        for (T *it = self->b; it != d_it; ++it) {
            T_destroy(it);
        }
    }
    return ok;
}

void SliceTMut_iter(SliceTMut const *const self, SliceTIter *const it)
{
    SliceTIter_new(it, self->b, self->e);
}

void SliceTMut_iter_mut(SliceTMut const *const self, SliceTMutIter *const it)
{
    SliceTMutIter_new(it, self->b, self->e);
}

// ============================================================================
void SliceTMutIter_wipe(SliceTMutIter *const self)
{
    STRUCTWIPE(self);
}

void SliceTMutIter_destroy(SliceTMutIter *const self)
{
    SliceTMutIter_wipe(self);
}

void SliceTMutIter_move(SliceTMutIter *const self, SliceTMutIter *const src)
{
    *self = *src;
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
    T *p = self->p;
    if (p == self->e) {
        return NULL;
    }
    self->p += 1;
    return p;
}

void SliceTMutIter_new(SliceTMutIter *const self, T *const b, T *const e)
{
    self->p = b;
    self->e = e;
}
