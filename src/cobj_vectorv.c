#include "cobj_vectorv.h"


static VectorV_destroy_elems(VectorV *const self, size_t elem_size, void (*elem_destroy)(void *const)) {
    uint8_t const *const end = self->buf + (self->len * elem_size);
    for (uint8_t *it = self->buf; it != end; it += elem_size) {
        destroy_elem(it);
    }
}


static VectorV_wipe(VectorV *const self) {
    STRUCTWIPE(self);
}


void VectorV_destroy(VectorV *const self, size_t elem_size, void (*elem_destroy)(void *const)) {
    VectorV_destroy_elems(self, elem_size, elem_destroy);
    VectorV_wipe(self);
}

void VectorV_move(VectorV *const self, VectorV *const src) {
    *self = *src;
    VectorV_wipe(src);
}


void VectorV_new(VectorV *const self, uint8_t *const arr, size_t len) {
    self->buf = arr;
    self->alloc = len;
    self->len = 0;
}


bool VectorV_is_empty(VectorV const *const self) {
    return self->len == 0;
}


void VectorV_clear(VectorV *const self, size_t elem_size, void (*elem_destroy)(void * const)) {
    VectorV_destroy_elems(self, elem_size, elem_destroy);
    self->len = 0;
}


size_t VectorV_len(VectorV const *self) {
    return self->len;
}


bool WARN_RESULT VectorV_push_back(VectorV *self, void *elem, size_t elem_size, void (*elem_move)(void * const, void * const)) {
    //moves, not copies
    if (self->len >= self->alloc) { return false; }
    void *p = self->buf + (self->len * elem_size);
    elem_move(p, elem);
    self->len += 1;
    return true;
}


bool WARN_RESULT VectorV_pop_back(VectorV *self, void *elem, size_t elem_size, void (*elem_move)(void * const, void * const)) {
    //moves, not copies
    if (self->len == 0) { return false; }
    void *p = self->buf + (self->len* elem_size);
    elem_move(elem, p);
    self->len -= 1;
    return true;
}


void *VectorV_get_item_mut(VectorV *self, Index pos, size_t elem_size ) {
    // null if no item at pos
    if (pos >= self->len) { return NULL; }
    void *p = self->buf + (self->len * elem_size);
    return p;
}


void const *VectorV_get_item(VectorV *self, Index pos, size_t elem_size) {
    // null if no item at pos
    if (pos >= self->len) { return NULL; }
    void *p = self->buf + (self->len * elem_size);
    return p;
}


void VectorV_iter(VectorV const *const self, VectorVIter *const it) {
    VectorVIter_new(it, self);
}


void VectorV_iter_mut(VectorV const *const self, VectorVIterMut *const it) {
    VectorVIterMut_new(it, self);
}


// ==========================================================================

static void VectorVIter_wipe(VectorVIter *const self) {
    STRUCTWIPE(self);
}


void VectorVIter_destroy(VectorVIter *const self) {
    VectorVIter_wipe(self);
}


void VectorVIter_move(VectorVIter *const self, VectorVIter *const src)
{
    *self = *src;
    VectorVIter_wipe(src);
}


void VectorVIter_new(VectorVIter *const self, VectorV const *const vec) {
    self->vec = vec;
    self->it = vec->buf;
}


void const *VectorVIter_next(VectorVIter *const self, size_t elem_size) {
    uint8_t const *const end = self->vec->buf + (self->vec->len * elem_size);
    // if (self->it == end) { return NULL; }
    // ++self->it;
    // if (self->it == end) { return NULL; }
    // return self->it;
    uint8_t const *it = self->it + elem_size;
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


// ==========================================================================

static void VectorVIterMut_wipe(VectorVIterMut *const self) {
    STRUCTWIPE(self);
}


void VectorVIterMut_destroy(VectorVIterMut *const self) {
    VectorVIterMut_wipe(self);
}


void VectorVIterMut_move(VectorVIterMut *const self, VectorVIterMut *const src)
{
    *self = *src;
    VectorVIterMut_wipe(src);
}


void VectorVIterMut_new(VectorVIterMut *const self, VectorV const *const vec) {
    self->vec = vec;
    self->it = vec->buf;
}


void *VectorVIterMut_next(VectorVIterMut *const self, size_t elem_size) {
    uint8_t *const end = self->vec->buf + (self->vec->len * elem_size);
    // if (self->it == end) { return NULL; }
    // self->it +=  elem_size;
    // if (self->it == end) { return NULL; }
    // return self->it;
    uint8_t *it = self->it + elem_size;
    if (it >= end) { return NULL; }
    self->it = it;
    return it;
    // if (self->pos >= self->vec->len) { return NULL; }
    // self->pos += 1;
    // if (self->pos >= self->vec->len) { return NULL; }
    // return &self->vec->arr[self->pos];
    // size_t pos = self->pos + 1;
    // if (pos >= self->vec->len) { return NULL; }
    // self->pos = pos;
    // return &self->vec->arr[self->pos];
}
