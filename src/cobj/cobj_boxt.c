#include "cobj_boxt.h"

#include "cobj_memory.h" // memwipe/STRUCTWIPE

#include <stddef.h> // NULL

static void BoxT_wipe(BoxT *const self)
{
    STRUCTWIPE(self);
}

void BoxT_destroy_member(BoxT *const self)
{
    T_destroy_member(self->elem);
    free(self->elem);
}

void BoxT_destroy(BoxT *const self)
{
    BoxT_destroy_member(self);
    BoxT_wipe(self);
}

void BoxT_move(BoxT *const self, BoxT *const src)
{
    *self = *src;
    BoxT_wipe(src);
}

bool WARN_UNUSED_RESULT BoxT_try_copy_member(BoxT *const self,
                                             BoxT const *const src,
                                             Error *const err)
{
    T *const p = try_malloc(sizeof(T), err);
    if (p == NULL) { return false; }

    bool ok;
    ok = T_try_copy_member(p, src->elem, err);
    if (!ok) {
        free(p);
        // no need to wipe self as didn't write to it.
        return false;
    }
    BoxT_own(self, p);
    return true;
}
bool WARN_UNUSED_RESULT BoxT_try_copy(BoxT *const self, BoxT const *const src, Error *const err)
{
    if (!BoxT_try_copy_member(self, src, err)) {
        BoxT_wipe(self);
        return false;
    }
    return true;
}

T const *BoxT_deref(BoxT const *const self)
{
    return self->elem;
}

T *BoxT_deref_mut(BoxT *const self)
{
    return self->elem;
}

bool BoxT_is_eq(BoxT const *const self, BoxT const *const b)
{
    return T_is_eq(self->elem, b->elem);
}

bool BoxT_is_ne(BoxT const *const self, BoxT const *const b)
{
    return T_is_ne(self->elem, b->elem);
}

bool BoxT_is_lt(BoxT const *const self, BoxT const *const b)
{
    return T_is_lt(self->elem, b->elem);
}

bool BoxT_is_gt(BoxT const *const self, BoxT const *const b)
{
    return T_is_gt(self->elem, b->elem);
}

bool BoxT_is_le(BoxT const *const self, BoxT const *const b)
{
    return T_is_le(self->elem, b->elem);
}

bool BoxT_is_ge(BoxT const *const self, BoxT const *const b)
{
    return T_is_ge(self->elem, b->elem);
}

bool BoxT_contains(BoxT const *const self, T const *const v)
{
    return T_is_eq(self->elem, v);
}

void BoxT_own(BoxT *const self, T *const p)
{
    self->elem = p;
}

T *BoxT_disown(BoxT *const self)
{
    T *p = self->elem;
    BoxT_wipe(self);
    return p;
}
void BoxT_swap(BoxT *const self, BoxT *const other)
{
    ptrswap((void **)&self->elem, (void **)&other->elem);
    // memswap(self, other, sizeof(*self));
    //  memberwise does not copy the padding.
    //  T *p = self->elem;
    //  self->elem = other->elem;
    //  other->elem = p;
    //  BoxT v;
    //  BoxT_move_member(&v, other);
    //  BoxT_move_member(other, self);
    //  BoxT_move_member(self, &v);
}

// T *BoxT_elem_try_alloc(BoxT *const self, Error *const err)
// {
//     T *const p = T_try_malloc(err);
//     return p;
// }
// void BoxT_elem_free(BoxT *const self, T *const p)
// {
//     T_free(p);
// }

bool WARN_UNUSED_RESULT BoxT_try_from_T(BoxT *const self, T *const v, Error *const err)
{
    T *const p = try_malloc(sizeof(T), err);
    if (p == NULL) { return false; }
    T_move(p, v);
    BoxT_own(self, p);
    return true;
}

void BoxT_into_T(BoxT *const self, T *const v)
{
    T *p = BoxT_disown(self);
    T_move_member(v, p);
    free(p);
}

// bool WARN_UNUSED_RESULT BoxT_try_copy_T(BoxT *const self, T const *const v, Error *const err)
// {
//     T *const p = BoxT_elem_try_alloc(self, err);
//     if (p == NULL) { return false; }

//     bool ok;
//     ok = T_try_copy(p, v, err);
//     if (!ok) {
//         BoxT_elem_free(self);
//         return false;
//     }

//     return true;
// }

// hmm, BoxT look like unique_ptr implementation
