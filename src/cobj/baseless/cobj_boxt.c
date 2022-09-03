#include <cobj/baseless/cobj_boxt.h>
#include <cobj/core/cobj_memory.h> // memwipe/STRUCTWIPE
#include <stddef.h> // NULL

static void cobj_BoxT_wipe(cobj_BoxT *const self)
{
    STRUCTWIPE(self);
}

void cobj_BoxT_destroy_member(cobj_BoxT *const self)
{
    T_destroy_member(self->elem);
    free(self->elem);
}

void cobj_BoxT_destroy(cobj_BoxT *const self)
{
    cobj_BoxT_destroy_member(self);
    cobj_BoxT_wipe(self);
}

void cobj_BoxT_move(cobj_BoxT *const self, cobj_BoxT *const src)
{
    *self = *src;
    cobj_BoxT_wipe(src);
}

bool WARN_UNUSED_RESULT cobj_BoxT_try_copy_member(cobj_BoxT *const self,
                                                  cobj_BoxT const *const src,
                                                  cobj_Error *const err)
{
    T *const p = cobj_try_malloc(sizeof(T), err);
    if (p == NULL) { return false; }

    bool ok;
    ok = T_try_copy_member(p, src->elem, err);
    if (!ok) {
        free(p);
        // no need to wipe self as didn't write to it.
        return false;
    }
    cobj_BoxT_own(self, p);
    return true;
}
bool WARN_UNUSED_RESULT cobj_BoxT_try_copy(cobj_BoxT *const self,
                                           cobj_BoxT const *const src,
                                           cobj_Error *const err)
{
    if (!cobj_BoxT_try_copy_member(self, src, err)) {
        cobj_BoxT_wipe(self);
        return false;
    }
    return true;
}

T const *cobj_BoxT_deref(cobj_BoxT const *const self)
{
    return self->elem;
}

T *cobj_BoxT_deref_mut(cobj_BoxT *const self)
{
    return self->elem;
}

bool cobj_BoxT_is_eq(cobj_BoxT const *const self, cobj_BoxT const *const b)
{
    return T_is_eq(self->elem, b->elem);
}

bool cobj_BoxT_is_ne(cobj_BoxT const *const self, cobj_BoxT const *const b)
{
    return T_is_ne(self->elem, b->elem);
}

bool cobj_BoxT_is_lt(cobj_BoxT const *const self, cobj_BoxT const *const b)
{
    return T_is_lt(self->elem, b->elem);
}

bool cobj_BoxT_is_gt(cobj_BoxT const *const self, cobj_BoxT const *const b)
{
    return T_is_gt(self->elem, b->elem);
}

bool cobj_BoxT_is_le(cobj_BoxT const *const self, cobj_BoxT const *const b)
{
    return T_is_le(self->elem, b->elem);
}

bool cobj_BoxT_is_ge(cobj_BoxT const *const self, cobj_BoxT const *const b)
{
    return T_is_ge(self->elem, b->elem);
}

bool cobj_BoxT_contains(cobj_BoxT const *const self, T const *const v)
{
    return T_is_eq(self->elem, v);
}

void cobj_BoxT_own(cobj_BoxT *const self, T *const p)
{
    self->elem = p;
}

T *cobj_BoxT_disown(cobj_BoxT *const self)
{
    T *p = self->elem;
    cobj_BoxT_wipe(self);
    return p;
}
void cobj_BoxT_swap(cobj_BoxT *const self, cobj_BoxT *const other)
{
    ptrswap((void **)&self->elem, (void **)&other->elem);
    // memswap(self, other, sizeof(*self));
    //  memberwise does not copy the padding.
    //  T *p = self->elem;
    //  self->elem = other->elem;
    //  other->elem = p;
    //  cobj_BoxT v;
    //  cobj_BoxT_move_member(&v, other);
    //  cobj_BoxT_move_member(other, self);
    //  cobj_BoxT_move_member(self, &v);
}

bool WARN_UNUSED_RESULT cobj_BoxT_try_from_T(cobj_BoxT *const self,
                                             T *const v,
                                             cobj_Error *const err)
{
    T *const p = cobj_try_malloc(sizeof(T), err);
    if (p == NULL) { return false; }
    T_move(p, v);
    cobj_BoxT_own(self, p);
    return true;
}

void cobj_BoxT_into_T(cobj_BoxT *const self, T *const v)
{
    T *p = cobj_BoxT_disown(self);
    T_move_member(v, p);
    free(p);
}

// bool WARN_UNUSED_RESULT cobj_BoxT_try_copy_T(cobj_BoxT *const self, T const *const v, cobj_Error
// *const err)
// {
//     T *const p = cobj_BoxT_elem_try_alloc(self, err);
//     if (p == NULL) { return false; }

//     bool ok;
//     ok = T_try_copy(p, v, err);
//     if (!ok) {
//         cobj_BoxT_elem_free(self);
//         return false;
//     }

//     return true;
// }

// hmm, cobj_BoxT look like unique_ptr implementation
