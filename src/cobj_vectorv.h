#if !defined(CBOJ_VECTORV_H)
#    define CBOJ_VECTORV_H

///////////////////////////////////////////////////////////////////////////////
// can vector internals be commoned across all vectors in C?
// yes, but still adds typing for each specialisation
// and adds 2ptrs+a size_t for each instance.
// and as 2 func ptrs are used, adds unsafeness as they are stored in ram for
// the lifetime of the vector.
// but, idea is to keep functions small so they'll be inlined so
// the templated version should not appear as sep functions.
typedef struct VectorV_ VectorV;
typedef struct VectorVIter_ VectorVIter;
typedef struct VectorVIterMut_ VectorVIterMut;

#    include "cobj_defs.h" // WARN_RESULT

#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

struct VectorV_ {
    uint8_t *buf;
    uint8_t *buf_end;
    uint8_t *buf_pos;
};

void VectorV_destroy(VectorV *const self, size_t elem_size, void (*elem_destroy)(void *const));
void VectorV_move(VectorV *const self, VectorV *const src);
void VectorV_new(VectorV *const self, uint8_t *const arr, uint8_t *const arr_e);
bool VectorV_is_empty(VectorV const *const self);
void VectorV_clear(VectorV *const self, size_t elem_size, void (*elem_destroy)(void *const));
size_t VectorV_len(VectorV const *self, size_t elem_size);
bool WARN_UNUSED_RESULT VectorV_push_back(VectorV *const self,
                                          void *const elem,
                                          size_t elem_size,
                                          void (*elem_move)(void *const, void *const));
bool WARN_UNUSED_RESULT VectorV_pop_back(VectorV *const self,
                                         void *const elem,
                                         size_t elem_size,
                                         void (*elem_move)(void *const, void *const));
void *VectorV_get_item_at_mut(VectorV const *const self, Index pos, size_t elem_size);
void const *VectorV_get_item_at(VectorV const *const self, Index pos, size_t elem_size);
void VectorV_iter(VectorV const *const self, VectorVIter *const it);
void VectorV_iter_mut(VectorV const *const self, VectorVIterMut *const it);

struct VectorVIter_ {
    uint8_t const *p;
    uint8_t const *e;
};
void VectorVIter_destroy(VectorVIter *const self);
void VectorVIter_move(VectorVIter *const self, VectorVIter *const src);
void const *WARN_UNUSED_RESULT VectorVIter_next(VectorVIter *const self, size_t elem_size);
void VectorVIter_new(VectorVIter *const self, void const *const b, void const *const e);

struct VectorVIterMut_ {
    uint8_t *p;
    uint8_t *e;
};
void VectorVIterMut_destroy(VectorVIterMut *const self);
void VectorVIterMut_move(VectorVIterMut *const self, VectorVIterMut *const src);
void *WARN_UNUSED_RESULT VectorVIterMut_next(VectorVIterMut *const self, size_t elem_size);
void VectorVIterMut_new(VectorVIterMut *const self, void *const b, void *const e);

#endif //! defined(CBOJ_VECTORV_H)
