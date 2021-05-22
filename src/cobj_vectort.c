#include "cobj_vectort.h"

#include "cobj_memory.h" //STRUCTWIPE


static void VectorT_wipe(VectorT *const self) {
    STRUCTWIPE(self);
}


void VectorT_destroy(VectorT *const self) {
    ArrayT_destroy(self->arr, self->len);
    VectorT_wipe(self);
}


void VectorT_move(VectorT *const self, VectorT *const src) {
    *self = *src;
    VectorT_wipe(self);
}


void VectorT_new(VectorT *const self, T *const arr, size_t len) {
    // don't use init lists, they are inefficient.
    self->arr = arr;
    self->alloc = len;
    self->len = 0;
}


void VectorT_clear(VectorT *const self) {
    ArrayT_destroy(self->arr, self->len);
    self->len = 0;
}


bool VectorT_is_empty(VectorT const *const self) {
    return self->len == 0;
}


size_t VectorT_len(VectorT const *const self) {
    return self->len;
}


size_t VectorT_alloc(VectorT const *const self) {
    return self->alloc;
}


bool VectorT_realloc(VectorT *const self, size_t newalloc) {
    // heapless vector cannot realloc?
    // can it realloc smaller?
    // cannot realloc larger.
    return false;
}


bool WARN_RESULT VectorT_push_back(VectorT *const self, T *const item) {
    // moves item
    if (self->len >= self->alloc) { return false; }
    T_move(&self->arr[self->len], item);
    self->len += 1;
    return true;
}


bool WARN_RESULT VectorT_pop_back(VectorT *const self, T *const item) {
    // moves item
    if (self->len == 0) { return false; }
    T_move(item, &self->arr[self->len]);
    self->len -= 1;
    return true;
}


T *VectorT_get_item_at_mut(VectorT *const self, Index pos) {
    // NULL if not exist
    if (pos >= self->len) { return NULL; }
    return &self->arr[pos];
}


T const *VectorT_get_item_at(VectorT *const self, Index pos) {
    // NULL if not exist
    if (pos >= self->len) { return NULL; }
    return &self->arr[pos];
}


void VectorT_iter(VectorT const *const self, VectorTIter *const it) {
    VectorTIter_new(it, self);
}


void VectorT_iter_mut(VectorT *const self, VectorTIterMut *const it) {
    VectorTIterMut_new(it, self);
}


// maybe vector can take a private heap struct, (if private heap is needed elsewhere)
// TODO: VectorT_malloc/VectorT_free/VectorT_malloc_new/VectorT_free_destroy.

// =========================================================================

static void VectorTIterMut_wipe(VectorTIterMut *const self) {
    STRUCTWIPE(self);
}


void VectorTIterMut_destroy(VectorTIterMut *const self) {
    VectorTIterMut_wipe(self);
}


void VectorTIterMut_move(VectorTIterMut *const self, VectorTIterMut *const src) {
    *self = *src;
    VectorTIterMut_wipe(self);
}


// initialises iter, sets start at first item
void VectorTIterMut_new(VectorTIterMut *const self, VectorT *const vec) {
    self->vec = vec;
    self->it = vec->alloc;
}


// moves to next item, returns ptr to item or NULL if no more items.
T *VectorTIterMut_next(VectorTIterMut *const self) {
    T *const end = self->vec->arr + self->vec->len;
    // if (self->it == end) { return NULL; }
    // ++self->it;
    // if (self->it == end) { return NULL; }
    // return self->it;
    T *it = self->it + 1;
    if (it >= end) { return NULL; }
    self->it = it;
    return it;
    // if (self->pos >= self->vec->len) { return NULL; }
    // self->pos += 1;
    // if (self->pos >= self->vec->len) { return NULL; }
    // return &self->vec->arr[self->pos];
    // size_t pos = self->pos + 1;
    // if (self->pos >= self->vec->len) { return NULL; }
    // self->pos = pos;
    // return &self->vec->arr[self->pos];
}
// annoyingly, it's 2 compares per call + 1 in the caller in the for-loop
// 3 ifs..


// =========================================================================

static void VectorTIter_wipe(VectorTIter *const self) {
    STRUCTWIPE(self);
}


void VectorTIter_destroy(VectorTIter *const self) {
    VectorTIter_wipe(self);
}


void VectorTIter_move(VectorTIter *const self, VectorTIter *const src) {
    *self = *src;
    VectorTIter_wipe(self);
}


void VectorTIter_new(VectorTIter *const self, VectorT const *const vec) {
    self->vec = vec;
    self->it = vec->alloc;
}


T const *VectorTIter_next(VectorTIter *const self) {
    T const *const end = self->vec->arr + self->vec->len;
    // if (self->it == end) { return NULL; }
    // ++self->it;
    // if (self->it == end) { return NULL; }
    // return self->it;
    T const *it = self->it + 1;
    if (it >= end) { return NULL; }
    self->it = it;
    return it;
    // if (self->pos >= self->vec->len) { return NULL; }
    // self->pos += 1;
    // if (self->pos >= self->vec->len) { return NULL; }
    // return &self->vec->arr[self->pos];
    // size_t pos = self->pos + 1;
    // if (self->pos >= self->vec->len) { return NULL; }
    // self->pos = pos;
    // return &self->vec->arr[self->pos];
}
// annoyingly, it's 2 compares per call + 1 in the caller in the for-loop
// 3 ifs..
