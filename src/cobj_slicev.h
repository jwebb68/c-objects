#if !defined(COBJ_SLICEV_H)
#    define COBJ_SLICEV_H

/**
 * SliceV, SliceV: The undelying implementation for a non-owned region of initialised Ts.
 *
 * This version is the underlying implementation for all SliceV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to SliceT.
 */

typedef struct SliceV_ SliceV;
typedef struct SliceVIter_ SliceVIter;

typedef struct SliceVMut_ SliceVMut;
typedef struct SliceVMutIter_ SliceVMutIter;

#    include "cobj_defs.h" // WARN_UNUSED_RESULT
#    include "cobj_error.h"

#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct SliceV_ {
    uint8_t const *b;
    uint8_t const *e;
};

void SliceV_destroy(SliceV *const self);
void SliceV_move(SliceV *const self, SliceV *const src);
bool WARN_UNUSED_RESULT SliceV_try_copy(SliceV *self, SliceV const *const src, Error *err);
void SliceV_new(SliceV *const self, void const *b, void const *e);

void const *SliceV_ptr(SliceV const *const self);
size_t SliceV_len(SliceV const *const self, size_t elem_size);
bool SliceV_is_empty(SliceV const *const self);

// null if not valid pos
void const *SliceV_get_item_at(SliceV const *const self, size_t pos, size_t elem_size);

bool SliceV_try_subslice(SliceV const *const self,
                         size_t b,
                         size_t e,
                         SliceV *const dest,
                         Error *err,
                         size_t elem_size);

void SliceV_iter(SliceV const *const self, SliceVIter *const it);

struct SliceVIter_ {
    uint8_t const *p;
    uint8_t const *e;
};

void SliceVIter_destroy(SliceVIter *const self);
void SliceVIter_move(SliceVIter *const self, SliceVIter *const src);
bool WARN_UNUSED_RESULT SliceVIter_try_copy(SliceVIter *const self, SliceVIter const *const src);
void const *SliceVIter_next(SliceVIter *const self, size_t elem_size);
void SliceVIter_new(SliceVIter *const self, void const *const b, void const *const e);

struct SliceVMut_ {
    uint8_t *b;
    uint8_t *e;
};

void SliceVMut_destroy(SliceVMut *const self);
void SliceVMut_move(SliceVMut *const self, SliceVMut *const src);
bool WARN_UNUSED_RESULT SliceVMut_try_copy(SliceVMut *self, SliceVMut const *const src, Error *err);
void SliceVMut_new(SliceVMut *const self, void *const arr, void *const e);

size_t SliceVMut_len(SliceVMut const *const self, size_t elem_size);
bool SliceVMut_is_empty(SliceVMut const *const self);

// null if not valid pos
void *SliceVMut_get_item_at(SliceVMut const *const self, size_t pos, size_t elem_size);

bool SliceVMut_try_subslice(SliceVMut const *const self,
                            size_t b,
                            size_t e,
                            SliceV *const dest,
                            Error *err,
                            size_t elem_size);
bool SliceVMut_try_subslice_mut(SliceVMut const *const self,
                                size_t b,
                                size_t e,
                                SliceVMut *const dest,
                                Error *err,
                                size_t elem_size);

void SliceVMut_as_slice(SliceVMut const *const self, SliceV *const s);

bool WARN_UNUSED_RESULT SliceVMut_try_move_from(SliceVMut *const self,
                                                SliceVMut const *const src,
                                                Error *err,
                                                size_t elem_size);
bool WARN_UNUSED_RESULT SliceVMut_try_copy_from(SliceVMut *const self,
                                                SliceV const *const src,
                                                Error *err,
                                                size_t elem_size,
                                                bool (*elem_try_copy)(void *,
                                                                      void const *,
                                                                      Error *),
                                                void (*elem_destroy)(void *));

void SliceVMut_iter(SliceVMut const *const self, SliceVIter *const it);
void SliceVMut_iter_mut(SliceVMut const *const self, SliceVMutIter *const it);

struct SliceVMutIter_ {
    uint8_t *p;
    uint8_t *e;
};

void SliceVMutIter_destroy(SliceVMutIter *const self);
void SliceVMutIter_move(SliceVMutIter *const self, SliceVMutIter *const src);
bool WARN_UNUSED_RESULT SliceVMutIter_try_copy(SliceVMutIter *const arr,
                                               SliceVMutIter const *const src,
                                               Error *const err);
void *SliceVMutIter_next(SliceVMutIter *const self, size_t elem_size);
void SliceVMutIter_new(SliceVMutIter *const self, void *const b, void *const e);

#endif //! defined(COBJ_SLICEV_H)
