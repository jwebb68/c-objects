#include <cobj/core/cobj_defs.h> // COBJ_UNUSED_ARG
#include <cobj/core/cobj_memory.h> // STRUCTWIPE
#include <cobj/t.h>
#include <stdlib.h> // malloc/free

static void T_wipe(T *const self)
{
    STRUCTWIPE(self);
}

void T_destroy_member(T *const self)
{
    COBJ_UNUSED_ARG(self);
}
void T_destroy(T *const self)
{
    T_destroy_member(self);
    T_wipe(self);
}

void T_move_member(T *const self, T *const src)
{
    *self = *src;
}
void T_move(T *const self, T *const src)
{
    T_move_member(self, src);
    T_wipe(src);
}

bool WARN_UNUSED_RESULT T_try_copy_member(T *const self, T const *const src, Error *const err)
{
    *self = *src;
    // and/or copy any members in a by-member fashion,
    // mainly those that have special copy semantics and a simple bit-copy won't do.
    COBJ_UNUSED_ARG(err);
    return true;
    // or
    // return ERROR_RAISE(err, Error_ENOTIMPL);
}
bool WARN_UNUSED_RESULT T_try_copy(T *const self, T const *const src, Error *const err)
{
    if (!T_try_copy_member(self, src, err)) {
        T_wipe(self);
        return false;
    }
    return true;
}

bool T_is_eq(T const *const self, T const *const b)
{
    return self->dummy == b->dummy;
}

bool T_is_ne(T const *const self, T const *const b)
{
    return self->dummy != b->dummy;
}

bool T_is_lt(T const *const self, T const *const b)
{
    return self->dummy < b->dummy;
}

bool T_is_gt(T const *const self, T const *const b)
{
    return self->dummy > b->dummy;
}

bool T_is_le(T const *const self, T const *const b)
{
    return self->dummy <= b->dummy;
}

bool T_is_ge(T const *const self, T const *const b)
{
    return self->dummy >= b->dummy;
}

void T_new(T *const self, int v)
{
    self->dummy = v;
}

void T_default(T *const self)
{
    self->dummy = 0;
}

// /*
//  * not sure about malloc/free functions.
//  * Do want the T_malloc to just alloc the memory, with T_free to free it (even though it's a call
//  to
//  * free()); Then it's: T *p = T_malloc(); T_new(p, ...); i.e. in-place initialise, with malloc
//  NOT
//  * zapping memory to 0 as it's going to be fully initialised in the new()
//  * - no point wasting cycles (there's enough of that going on anyway).
//  * then later
//  *   T_destroy(p);
//  *   T_free(p);
//  * *
//  * But then is a better soln to use a BoxT ? (somewhat equiv of a unique_ptr<T> in C++).
//  *
//  * note: had tried creating malloc_new* functions as helpers, but although it looks neater
//  *       it was a lot of duplication.
//  *       Also if malloc_new, then what is the destroy..? free_destroy()?
//  *       the T_free became a T_destroy + free() which disturbed the symmetry of the design.
//  *       in that free did more than just free.
//  *       Simpler to use have a T_malloc and T_free and call new/destroy as needed.
//  */
// T *T_try_malloc(Error *const err)
// {
//     return (T *)try_malloc(sizeof(T), err);
// }

// void T_free(T *const self)
// {
//     // assumes self is already uninitialised.
//     free(self);
// }
