#if !defined(COBJ_BOXVT_H)
#    define COBJ_BOXVT_H

/**
 * cobj_BoxV<T>: object created and managed on the heap, without sharing.
 *
 * Box contents owned by the box and are not shared (use RCT for shared objects).
 * inherently a Mut; cannot move a const value into it, cannot copy into into a const box.
 *
 * This version is the veneer layer for when using cobj_BoxV as the actual implementation.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to cobj_BoxT.
 *
 * Warning: some of the  new functions may prevent this being a viable pattern.
 * As I've yet to find a solution that is a single func call to cobj_BoxV (as the rest of them are).
 */

// forward declares
typedef struct cobj_BoxVT_ cobj_BoxVT;

// includes
#    include <cobj/based/cobj_boxv.h>
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

// defines
struct cobj_BoxVT_ {
    cobj_BoxV inner;
};

void cobj_BoxVT_destroy_member(cobj_BoxVT *const self);
void cobj_BoxVT_destroy(cobj_BoxVT *const self);

void cobj_BoxVT_move_member(cobj_BoxVT *const self, cobj_BoxVT *const src);
void cobj_BoxVT_move(cobj_BoxVT *const self, cobj_BoxVT *const src);

bool WARN_UNUSED_RESULT cobj_BoxVT_try_copy_member(cobj_BoxVT *const self,
                                                   cobj_BoxVT const *const v,
                                                   cobj_Error *const err);
bool WARN_UNUSED_RESULT cobj_BoxVT_try_copy(cobj_BoxVT *const self,
                                            cobj_BoxVT const *const v,
                                            cobj_Error *const err);

T const *cobj_BoxVT_deref(cobj_BoxVT const *const self);
T *cobj_BoxVT_deref_mut(cobj_BoxVT *const self);

// comparisons
bool cobj_BoxVT_is_eq(cobj_BoxVT const *const self, cobj_BoxVT const *const b);
bool cobj_BoxVT_is_ne(cobj_BoxVT const *const self, cobj_BoxVT const *const b);
bool cobj_BoxVT_is_lt(cobj_BoxVT const *const self, cobj_BoxVT const *const b);
bool cobj_BoxVT_is_gt(cobj_BoxVT const *const self, cobj_BoxVT const *const b);
bool cobj_BoxVT_is_le(cobj_BoxVT const *const self, cobj_BoxVT const *const b);
bool cobj_BoxVT_is_ge(cobj_BoxVT const *const self, cobj_BoxVT const *const b);

// does box contain value..
bool cobj_BoxVT_contains(cobj_BoxVT const *const self, T const *const v);

void cobj_BoxVT_own(cobj_BoxVT *const self, T *const p);
T *cobj_BoxVT_disown(cobj_BoxVT *const self);

bool WARN_UNUSED_RESULT cobj_BoxVT_try_from_T(cobj_BoxVT *const self,
                                              T *const v,
                                              cobj_Error *const err);
void cobj_BoxVT_into_T(cobj_BoxVT *const self, T *const v);

bool WARN_UNUSED_RESULT cobj_BoxVT_try_copy_T(cobj_BoxVT *const self,
                                              T const *const v,
                                              cobj_Error *const err);

// developer needs to add own inplace new functions per type.
// void cobj_BoxVT_new...(cobj_BoxVT_ *const self, ...);

#endif //! defined(COBJ_BOXVT_H)
