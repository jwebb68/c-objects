#include "cobj_box.h"

#include "cobj_memory.h"  // memwipe/STRUCTWIPE

#include <stddef.h>  // NULL


static void BoxT_wipe(BoxT *const self) {
    STRUCTWIPE(self);
}


void BoxT_destroy(BoxT *const self) {
    T_destroy(&self->elem);
    free(&self->elem);
    BoxT_wipe(self);
}


void BoxT_move(BoxT *const self, BoxT *const src) {
    *self = *src;
    BoxT_wipe(src);
}


T const *BoxT_deref(BoxT const *const self) {
    return self->elem;
}


bool BoxT_is_none(BoxT const *const self) {
    return self->elem == NULL;
}


bool BoxT_is_eq(BoxT const *const self, BoxT const *const b) {
    return T_is_eq(self->elem, b->elem);
}


bool BoxT_is_lt(BoxT const *const self, BoxT const *const b) {
    return T_is_lt(self->elem, b->elem);
}


bool BoxT_is_gt(BoxT const *const self, BoxT const *const b) {
    return T_is_gt(self->elem, b->elem);
}


void BoxT_new_own(BoxT *const self, T const *const p) {
    self->elem = p;
}


bool BoxT_try_new_int(BoxT *const self, int v, Error *err) {
    // err, malloc can fail, so new can fail
    // try_new ?
    T *const p = T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }
    T_new_int(p, v);
    self->elem = p;
    return true;
}


bool BoxT_try_new_from_T(BoxT *const self, T *const v, Error *err) {
    T *const p = T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }
    T_new_from_T(p, v);
    self->elem = p;
    return true;
}


bool BoxT_try_new_copy_T(BoxT *const self, T const *const v, Error *err) {
    T *const p = T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }
    bool ok;
    ok = T_new_copy_T(p, v, err);
    if (!ok) {
        free(p);
        return false;
    } else {
        self->elem = p;
        return true;
    }
}

// and now lost the WARN_RESULT checking..
// void BoxT_try_new_copy_T(BoxT *const self, T const *const v, Status *ret) {
//     T *const p = T_malloc();
//     if (p == NULL) {
//         STATUS_RAISE(ret, Error_ENOMEM);
//         return;
//     }
//     T_new_copy_T(p, v, ret);
//     if (Status_is_err(ret)) {
//         free(p);
//     } else {
//         self->elem = p;
//     }
// }
// and now lost the WARN_RESULT checking..
// void BoxT_try_new_copy_T(BoxT *const self, T const *const v, Result *ret) {
//     T *const p = T_malloc();
//     if (p == NULL) {
//         RESULT_ERR(ret, Error_ENOMEM);
//         return;
//     }
//     T_new_copy_T(p, v, ret);
//     if (Result_is_err(ret)) {
//         free(p);
//     } else {
//         self->elem = p;
//     }
// }

// ===========================================================================

static void BoxTMut_wipe(BoxTMut *const self) {
    STRUCTWIPE(self);
}


void BoxTMut_destroy(BoxTMut *const self) {
    T_destroy(&self->elem);
    free(&self->elem);
    BoxTMut_wipe(self);
}


void BoxTMut_move(BoxTMut *const self, BoxTMut *const src) {
    *self = *src;
    BoxTMut_wipe(src);
}


T *BoxTMut_deref_mut(BoxTMut const *const self) {
    return self->elem;
}


T const *BoxTMut_deref(BoxTMut const *const self) {
    return self->elem;
}


bool BoxTMut_is_none(BoxTMut const *const self) {
    return self->elem == NULL;
}


bool BoxTMut_is_eq(BoxTMut const *const self, BoxTMut const *const b) {
    return T_is_eq(self->elem, b->elem);
}


bool BoxTMut_is_lt(BoxTMut const *const self, BoxTMut const *const b) {
    return T_is_lt(self->elem, b->elem);
}


bool BoxTMut_is_gt(BoxTMut const *const self, BoxTMut const *const b) {
    return T_is_gt(self->elem, b->elem);
}


// create and take ownsership of existing heap obj
// no disown as that is an implicit destroy.
void BoxTMut_new_own(BoxTMut *const self, T *const p) {
    self->elem = p;
}


// create in-place and take ownership
// one new_ for each new_ on T
bool BoxTMut_try_new_int(BoxTMut *const self, int v, Error *const err) {
    T *p = T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }
    T_new_v(p, v);
    self->elem = p;
    return true;
}


// create and take ownership of existing T
bool BoxTMut_try_new_from_T(BoxTMut *const self, T *const v, Error *const err) {
    T *p = T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }
    T_move(p, v);
    self->elem = p;
    return true;
}


// create and take ownership of copy of existing T
// direct into dest.
bool BoxTMut_try_new_copy_T(BoxTMut *const self, T const *const v, Error *const err) {
    T *p = T_malloc();
    if (p == NULL) {
        return ERROR_RAISE(err, Error_ENOMEM);
    }
    bool ok;
    ok = T_try_copy(p, v, err);
    if (!ok) {
        free(p);
        return false;
    } else {
        self->elem = p;
        return true;
    }
}


// hmm, BoxT and BoxTMut look like unique_ptr implementations
// but without the release
