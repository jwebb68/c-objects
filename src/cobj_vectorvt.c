#include "cobj_vectorvt.h"

#include "cobj_memory.h" // STRUCTWIPE


static void VectorVT_move_elem(void *const self, void *const src) {
    T_move((T *)self, (T *)src);
}


static void VectorVT_destroy_elem(void *const self) {
    T_destroy((T *)self);
}


// static void VectorVT_wipe(VectorVT *const self) {
//     STRUCTWIPE(self);
// }


void VectorVT_destroy(VectorVT *const self) {
    VectorV_destroy(&self->inner, sizeof(T), VectorVT_destroy_elem);
    // do we need to wipe if only 1 struct in struct with own wipe?
}


void VectorVT_move(VectorVT *const self, VectorVT *const src) {
    VectorV_move(&self->inner, &src->inner);
    // do we need to wipe if only 1 struct in struct with own wipe?
}


void VectorVT_new(VectorVT *const self,  T *const arr, size_t const len) {
    VectorV_new(&self->inner, (uint8_t *)arr, (uint8_t *)(arr + len));
}


bool VectorVT_is_empty(VectorVT const *const self) {
    return VectorV_is_empty(&self->inner);
}


void VectorVT_clear(VectorVT *const self) {
    VectorV_clear(&self->inner, sizeof(T), VectorVT_destroy_elem);
}


size_t VectorVT_len(VectorVT const *const self) {
    return VectorV_len(&self->inner, sizeof(T));
}


bool WARN_UNUSED_RESULT VectorVT_push_back(VectorVT *const self, T *const elem) {
    return VectorV_push_back(&self->inner, elem, sizeof(T), VectorVT_move_elem);
}


bool WARN_UNUSED_RESULT VectorVT_pop_back(VectorVT *const self, T *const elem) {
    return VectorV_pop_back(&self->inner, elem, sizeof(T), VectorVT_move_elem);
}


T *VectorVT_get_item_at_mut(VectorVT *const self, Index const pos) {
    return (T *)VectorV_get_item_at_mut(&self->inner, pos, sizeof(T));
}


T const *VectorVT_get_item_at(VectorVT *const self, Index const pos) {
    return (T const *)VectorV_get_item_at(&self->inner, pos, sizeof(T));
}


//===========================================================================

// static void VectorVTIter_wipe(VectorVTIter *const self) {
//     STRUCTWIPE(self);
// }


void VectorVTIter_destroy(VectorVTIter *const self) {
    VectorVIter_destroy(&self->inner);
    // do we need to wipe if only 1 struct in struct with own wipe?
}


void VectorVTIter_move(VectorVTIter *const self, VectorVTIter *const src) {
    VectorVIter_move(&self->inner, &src->inner);
    // do we need to wipe if only 1 struct in struct with own wipe?
}


void VectorVTIter_new(VectorVTIter *const self, VectorVT const *const vec) {
    VectorVIter_new(&self->inner, &vec->inner);
}


T const *WARN_UNUSED_RESULT VectorVTIter_next(VectorVTIter *const self) {
    return (T const *)VectorVIter_next(&self->inner, sizeof(T));
}


// ========================================================================
// static void VectorVTIterMut_wipe(VectorVTIterMut *const self) {
//     STRUCTWIPE(self);
// }


void VectorVTIterMut_destroy(VectorVTIterMut *const self) {
    VectorVIterMut_destroy(&self->inner);
    // do we need to wipe if only 1 struct in struct with own wipe?
}


void VectorVTIterMut_move(VectorVTIterMut *const self, VectorVTIterMut *const src) {
    VectorVIterMut_move(&self->inner, &src->inner);
    // do we need to wipe if only 1 struct in struct with own wipe?
}


void VectorVTIterMut_new(VectorVTIterMut *const self, VectorVT *const vec) {
    VectorVIterMut_new(&self->inner, &vec->inner);
}


T *WARN_UNUSED_RESULT VectorVTIterMut_next(VectorVTIterMut *const self) {
    return (T *)VectorVIterMut_next(&self->inner, sizeof(T));
}
