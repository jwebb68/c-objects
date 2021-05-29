#include "cobj_rcboxv.h"

#include "cobj_memory.h"  // STRUCTWIPE
#include "cobj_error.h"

#include <stdlib.h> // malloc/free
#include <inttypes.h> // uint8_t

// shared internal node struct between box and RCBox
// RCNode or SharedBox
// vs AutoBox/UniqueBox ?

typedef struct RCNodeV_ RCNodeV;
struct RCNodeV_ {
    size_t alloc;
    size_t rc;
    uint8_t val[];
};


void RCNodeV_wipe(RCNodeV *const self) {
    memwipe(self, sizeof(size_t)+self->alloc);
}


void RCNodeV_destroy(RCNodeV *const self, void (*elem_destroy)(void *elem)) {
    elem_destroy(self->val);
    RCNodeV_wipe(self);
}


void RCNodeV_grab(RCNodeV *const self) {
    self->rc += 1;
}


bool WARN_UNUSED_RESULT RCNodeV_release(RCNodeV *const self) {
    self->rc -= 1;
    return (self->rc != 0);
}


void const *RCNodeV_deref(RCNodeV const *const self) {
    return self->val;
}


void *RCNodeV_deref_mut(RCNodeV *const self) {
    return self->val;
}


RCNodeV *RCNodeV_malloc(size_t elem_size, Error *err) {
    size_t size = sizeof(size_t) + sizeof(size_t) + elem_size;
    RCNodeV *p = malloc(size);
    if (p == NULL) {
        bool ok;
        ok = ERROR_RAISE(err, Error_ENOMEM);
        UNUSED_ARG(ok);
        return p;
    }
    return p;
}


// void RCNodeV_new_int(RCNodeV *const self, int v) {
//     self->rc = 0;
//     T_new_int(&self->val, v);
// }


// void RCNodeV_new_from_T(RCNodeV *const self, T *const src) {
//     self->rc = 0;
//     T_move(&self->val, src);
// }


// bool RCNodeV_try_new_copy_T(RCNodeV *const self, T const *const src, Error *const err) {
//     self->rc = 0;
//     return T_try_copy(&self->val, src, err);
// }

// ==========================================================================

static void RCBoxV_wipe(RCBoxV *const self) {
    STRUCTWIPE(self);
}


void RCBoxV_destroy(RCBoxV *const self, void (*elem_destroy)(void *elem)) {
    if (!RCNodeV_release(self->node)) {
        RCNodeV_destroy(self->node, elem_destroy);
    }
    RCBoxV_wipe(self);
}


void RCBoxV_move(RCBoxV *const self, RCBoxV *const src) {
    *self = *src;
    RCBoxV_wipe(src);
}


bool RCBoxV_try_copy(RCBoxV *const self, RCBoxV const *const src, Error *err) {
    UNUSED_ARG(err);
    *self = *src;
    RCNodeV_grab(self->node);
    return true;
}


void const *RCBoxV_deref(RCBoxV const *const self) {
    return RCNodeV_deref(self->node);
}


void *RCBoxV_deref_mut(RCBoxV *const self) {
    return RCNodeV_deref_mut(self->node);
}


// bool WARN_UNUSED_RESULT RCBoxV_try_new_int(RCBoxV *const self, int v, Error *const err) {
//     RCNodeV *p = RCNodeV_malloc();
//     if (p == NULL) {
//         return ERROR_RAISE(err, Error_ENOMEM);
//     }

//     RCNodeV_new_int(p, v);
//     RCNodeV_grab(p);
//     self->node = p;
//     return true;
// }

bool WARN_UNUSED_RESULT RCBoxV_try_new_from(RCBoxV *const self, void *const v, Error *const err, size_t elem_size, bool (*elem_try_copy)(void *elem, void const *src, Error *err)) {
    RCNodeV *p = RCNodeV_malloc(elem_size, err);
    if (p == NULL) {
        return p;
    }

    bool ok;
    ok = elem_try_copy(RCNodeV_deref_mut(p), v, err);
    if (!ok) {
        free(p);
        return ERROR_RAISE(err, Error_EFAIL);
    }

    RCNodeV_grab(p);
    self->node = p;
    return true;
}

// bool WARN_UNUSED_RESULT RCBoxV_try_new_from_T(RCBoxT *const self, T *const v, Error *const err) {
//     RCNodeV *p = RCNodeV_malloc();
//     if (p == NULL) {
//         return ERROR_RAISE(err, Error_ENOMEM);
//     }

//     T_move(RCNodeV_deref_mut(p), v);
//     RCNodeV_grab(p);
//     self->node = p;
//     return true;
// }


// // new_copy variant? copy direct into dest without intermed storage?
// bool WARN_UNUSED_RESULT RCBoxT_try_new_copy_T(RCBoxT *const self, T const *const v, Error *const err) {
//     RCNodeV *p = RCNodeV_malloc();
//     if (p == NULL) {
//         return ERROR_RAISE(err, Error_ENOMEM);
//     }

//     bool ok;
//     ok = T_try_copy(RCNodeV_deref_mut(p), v, err);
//     if (!ok) {
//         // no destroy as node is not initialised and destroy wants it
//         // initialised.
//         free(p);
//         return false;
//     }

//     RCNodeV_grab(p);
//     self->node = p;
//     return true;
// }


// bool RCBoxV_is_eq(RCBoxV const *const self, RCBoxV const *const b) {
//     return T_is_eq(RCBoxT_deref(self), RCBoxT_deref(b));
// }


// bool RCBoxT_is_lt(RCBoxT const *const self, RCBoxT const *const b) {
//     return T_is_lt(RCBoxT_deref(self), RCBoxT_deref(b));
// }


// bool RCBoxT_is_gt(RCBoxT const *const self, RCBoxT const *const b) {
//     return T_is_gt(RCBoxT_deref(self), RCBoxT_deref(b));
// }

// no own of ptr: that leads to heap fragmentation
// no own of value: that's a new_default + T_destroy + T_move
// no disown of value: that requires only 1 ref so would be a try_disown, and also required as move.
