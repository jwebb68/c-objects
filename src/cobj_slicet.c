#include "cobj_slicet.h"

#include "cobj_memory.h"
#include "cobj_defs.h" // UNUSED_ARG

#include <memory.h> // memmove


void SliceT_destroy(SliceT *const self) {
    STRUCTWIPE(self);
}


void SliceT_move(SliceT *const self, SliceT *const src) {
    *self = *src;
    STRUCTWIPE(src);
}


bool WARN_UNUSED_RESULT SliceT_try_copy(SliceT *self, SliceT const *const src, Error *err) {
    UNUSED_ARG(err);
    *self = *src;
    return true;
}


void SliceT_new(SliceT *const self, T const *arr, size_t len) {
    self->arr = arr;
    self->arr_end = arr + len;
}


// T const *SliceT_ptr(SliceT const *const self) {
//     return self->arr;
// }


// static T const *SliceT_end(SliceT const *const self) {
//     return self->arr_end;
// }


size_t SliceT_len(SliceT const *const self) {
    return self->arr_end - self->arr;
}


bool SliceT_is_empty(SliceT const *const self) {
    return self->arr_end == self->arr;
}


T const *SliceT_get_item_at(SliceT const *const self, size_t pos) {
    T const *p = self->arr + pos;
    if (p >= self->arr_end) { return NULL; }
    return p;
}


bool SliceT_try_subslice(SliceT const *const self, size_t b, size_t e, SliceT *const dest, Error *err) {
    if (b > e) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (b > SliceT_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (e > SliceT_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    SliceT_new(dest, self->arr + b, e-b);
    return true;
}


// bool WARN_UNUSED_RESULT SliceT_try_move_from(SliceT *const self, SliceT *const src, Error *err) {
//     // move contents of one slice into another, both structs must be initialised., what structs refer to must be initialised?
//     // or dest must not be initialised.
//     if (SliceT_len(self) != SliceT_len(src)) {
//         return ERROR_RAISE(err, Error_EFAIL);
//     }

//     // for (T const *d_it = self->arr, *s_it = src->arr; d_it != self->arr_end; ++d_it, ++s_it) {
//     //     T_move(d_it, s_it);
//     // }
//     memmove(self->arr, src->arr, SliceT_len(self));
//     memwipe(src->arr, SliceT_len(src));
// }

// bool WARN_UNUSED_RESULT SliceT_try_copy_from(SliceT *const self, SliceT const *const src, Error *err) {
//     // copy contents of one slice into another, both structs must be initialised., what structs refer to must be initialised?
//     // or dest must not be initialised.
//     if (SliceT_len(self) != SliceT_len(src)) {
//         return ERROR_RAISE(err, Error_EFAIL);
//     }

//     bool ok = true;
//     T const *d_it = self->arr;
//     for (T const *s_it = src->arr; d_it != self->arr_end; ++d_it, ++s_it) {
//         ok = T_try_copy(d_it, s_it, err);
//         if (!ok) { break; }
//     }
//     if (!ok) {
//         for (T const *it = self->arr; it != d_it; ++it) {
//             T_destroy(it);
//         }
//     }
//     return ok;
// }


void SliceT_iter(SliceT const *const self, SliceTIter *const it) {
    SliceTIter_new(it, self->arr, self->arr_end);
}




void SliceTIter_destroy(SliceTIter *const self) {
    STRUCTWIPE(self);
}


void SliceTIter_move(SliceTIter *const self, SliceTIter *const src) {
    *self = *src;
    STRUCTWIPE(src);
}


bool WARN_UNUSED_RESULT SliceTIter_try_copy(SliceTIter *const self, SliceTIter const *const src) {
    *self = *src;
    return true;
}


T const *SliceTIter_next(SliceTIter *const self) {
    if (self->p != self->e) { return NULL; }
    self->p += 1;
    return self->p;
}


void SliceTIter_new(SliceTIter *const self, T const * const b, T const * const e) {
    self->p = b;
    self->e = e;
}




void SliceTMut_destroy(SliceTMut *const self) {
    STRUCTWIPE(self);
}


void SliceTMut_move(SliceTMut *const self, SliceTMut *const src) {
    *self = *src;
    STRUCTWIPE(src);
}


bool WARN_UNUSED_RESULT SliceTMut_try_copy(SliceTMut *self, SliceTMut const *const src, Error *err) {
    UNUSED_ARG(err);
    *self = *src;
    return true;
}


void SliceTMut_new(SliceTMut *const self, T *arr, size_t len) {
    self->arr = arr;
    self->arr_end = arr + len;
}


// T *SliceTMut_ptr(SliceTMut const *const self) {
//     return self->arr;
// }


// static T *SliceTMut_end(SliceTMut const *const self) {
//     return self->arr_end;
// }


size_t SliceTMut_len(SliceTMut const *const self) {
    return self->arr_end - self->arr;
}


bool SliceTMut_is_empty(SliceTMut const *const self) {
    return self->arr_end == self->arr;
}


bool SliceTMut_try_subslice(SliceTMut const *const self, size_t b, size_t e, SliceTMut *const dest, Error *err) {
    if (b > e) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (b > SliceTMut_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    if (e > SliceTMut_len(self)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }
    SliceTMut_new(dest, self->arr+b, e-b);
    return true;
}


void SliceTMut_as_slice(SliceTMut const *const self, SliceT *const s) {
    s->arr = self->arr;
    s->arr_end = self->arr_end;
}


bool WARN_UNUSED_RESULT SliceTMut_try_move_from(SliceTMut *const self, SliceTMut const *const src, Error *err) {
    // move contents of one slice into another, both structs must be initialised., what structs refer to must be initialised?
    // or dest must not be initialised.
    if (SliceTMut_len(self) != SliceTMut_len(src)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }

    // self/dest must be uninitialised
    // This will mean a mid-range move will have a very bad contents overall in the dest.
    // After the move, the src will be in a bad state if it's a midrange slice.
    // (a uninitialsed block in a block of initialised)
    // for (T *it = self->arr; it != self->arr_end; ++it) {
    //     T_destroy(it);
    // }
    // for (T const *d_it = self->arr, *s_it = src->arr; d_it != self->arr_end; ++d_it, ++s_it) {
    //     T_move(d_it, s_it);
    // }
    // use memmove to move the bits en masse, for speed.
    // bad: memmove src is const, when shouldn't be.
    memmove(self->arr, src->arr, SliceTMut_len(self));
    memwipe(src->arr, SliceTMut_len(src));
    return true;
}


bool WARN_UNUSED_RESULT SliceTMut_try_copy_from(SliceTMut *const self, SliceT const *const src, Error *err) {
    // copy contents of one slice into another, both structs must be initialised., what structs refer to must be initialised?
    // or dest must not be initialised.
    if (SliceTMut_len(self) != SliceT_len(src)) {
        return ERROR_RAISE(err, Error_EFAIL);
    }

    // If initially initialised, does the fail path preserve the orig data?
    // what if this is in the middle of a vec? == deinitialised items in array of initialised.
    // so, copy_from cannot be used to mutate contents of a vec.
    // copy_from cannot be used to populate array
    // This will mean a mid-range copy will have a very bad contents overall in the dest.
    // (a uninitialsed block in a block of initialised)
    // for (T *it = self->arr; it != self->arr_end; ++it) {
    //     T_destroy(it);
    // }
    bool ok = true;
    T *d_it = self->arr;
    for (T const *s_it = src->arr; d_it != self->arr_end; ++d_it, ++s_it) {
        ok = T_try_copy(d_it, s_it, err);
        if (!ok) { break; }
    }
    if (!ok) {
        for (T *it = self->arr; it != d_it; ++it) {
            T_destroy(it);
        }
    }
    return ok;
}


void SliceTMut_iter(SliceTMut const *const self, SliceTIter *const it) {
    SliceTIter_new(it, self->arr, self->arr_end);
}


void SliceTMut_iter_mut(SliceTMut const *const self, SliceTMutIter *const it) {
    SliceTMutIter_new(it, self->arr, self->arr_end);
}




void SliceTMutIter_destroy(SliceTMutIter *const self) {
    STRUCTWIPE(self);
}


void SliceTMutIter_move(SliceTMutIter *const self, SliceTMutIter *const src) {
    *self = *src;
    STRUCTWIPE(src);
}


bool WARN_UNUSED_RESULT SliceTMutIter_try_copy(SliceTMutIter *const self, SliceTMutIter const *const src) {
    *self = *src;
    return true;
}


T *SliceTMutIter_next(SliceTMutIter *const self) {
    if (self->p != self->e) { return NULL; }
    self->p += 1;
    return self->p;
}


void SliceTMutIter_new(SliceTMutIter *const self, T * const b, T * const e) {
    self->p = b;
    self->e = e;
}
