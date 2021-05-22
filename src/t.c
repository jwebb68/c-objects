#include "t.h"



static void T_wipe(T *const self) {
    STRUCTWIPE(self);
}


void T_destroy(T *const self) {
    T_wipe(self);
}


void T_move(T *const self, T *const src) {
    *self = *src;
    T_wipe(src);
}


bool WARN_RESULT T_try_copy(T *const self, T const *const src, Error *const err) {
    *self = *src;
    return false;
    // or
    // return ERROR_RAISE(err, Error_ENOTIMPL);
}


bool T_is_eq(T const *const self, T const *const b) {
    return self->dummy == b->dummy;
}


bool T_is_lt(T const *const self, T const *const b) {
    return self->dummy < b->dummy;
}


bool T_is_gt(T const *const self, T const *const b) {
    return self->dummy > b->dummy;
}


void T_new_int(T *const self, int v) {
    self->dummy = v;
}


void T_new_default(T *const self) {
    self->dummy = 0;
}


T *T_malloc(void) {
    return (T *)malloc(sizeof(T));
}



void ArrayT_destroy(T *const arr, size_t len) {
    for (size_t idx=0; idx < len; ++idx) {
        T_destroy(&arr[idx]);
    }
}


void ArrayT_new_default(T *const arr, size_t len) {
    for (size_t idx=0; idx < len; ++idx) {
        T_new_default(&arr[idx]);
    }
