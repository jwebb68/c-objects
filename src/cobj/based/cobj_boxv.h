#if !defined(COBJ_BOXV_H)
#    define COBJ_BOXV_H

/**
 * cobj_BoxV: object created and managed on the heap, without sharing.
 *
 * Box contents owned by the box and are not shared (use RCT for shared objects).
 * inherently a Mut; cannot move a const value into it, cannot copy into into a const box.
 *
 * This version is the underlying implementation for all cobj_BoxV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to cobj_BoxT.
 */
// hmm, using the void * as implementation would prevent debug lookups.
// also, the two implementations look the same so won't get any code size reductions.
// prob simpler/easier to go with the non-V version

// forward declares
typedef struct cobj_BoxV_s cobj_BoxV;

// includes
#    include <cobj/core/cobj_error.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

// defines

struct cobj_BoxV_s {
    void *elem;
};

void cobj_BoxV_destroy_member(cobj_BoxV *const self,
                              void (*elem_destroy)(void *const elem),
                              void (*elem_free)(void *elem));
void cobj_BoxV_destroy(cobj_BoxV *const self,
                       void (*elem_destroy)(void *const elem),
                       void (*elem_free)(void *elem));

void cobj_BoxV_move_member(cobj_BoxV *const self, cobj_BoxV *const src);
void cobj_BoxV_move(cobj_BoxV *const self, cobj_BoxV *const src);

bool WARN_UNUSED_RESULT cobj_BoxV_try_copy_member(cobj_BoxV *const self,
                                                  cobj_BoxV const *const v,
                                                  cobj_Error *const err,
                                                  void *(*elem_try_alloc)(cobj_Error *const err),
                                                  void (*elem_free)(void *elem),
                                                  bool (*elem_try_copy)(void *const elem,
                                                                        void const *const elem_src,
                                                                        cobj_Error *const err));
bool WARN_UNUSED_RESULT cobj_BoxV_try_copy(cobj_BoxV *const self,
                                           cobj_BoxV const *const v,
                                           cobj_Error *const err,
                                           void *(*elem_try_alloc)(cobj_Error *const err),
                                           void (*elem_free)(void *elem),
                                           bool (*elem_try_copy)(void *const elem,
                                                                 void const *const elem_src,
                                                                 cobj_Error *const err));

void const *cobj_BoxV_deref(cobj_BoxV const *const self);
void *cobj_BoxV_deref_mut(cobj_BoxV *const self);

// create and take ownsership of existing heap obj
// no NULLs
void cobj_BoxV_own(cobj_BoxV *const self, void *const p);
// hand ownership of heap obj to caller, destroys self
void *cobj_BoxV_disown_member(cobj_BoxV *const self);
void *cobj_BoxV_disown(cobj_BoxV *const self);

void cobj_BoxV_swap(cobj_BoxV *const self, cobj_BoxV *const other);

// move elem into box.
bool WARN_UNUSED_RESULT cobj_BoxV_try_from_V(cobj_BoxV *const self,
                                             void *const elem,
                                             cobj_Error *const err,
                                             void *(*elem_try_alloc)(cobj_Error *const err),
                                             void (*elem_move)(void *const elem, void *const src));
void cobj_BoxV_into_V(cobj_BoxV *const self,
                      void *const elem,
                      void (*elem_move)(void *const elem, void *const src),
                      void (*elem_free)(void *elem));

bool WARN_UNUSED_RESULT cobj_BoxV_try_copy_V(cobj_BoxV *const self,
                                             void const *const elem,
                                             cobj_Error *const err,
                                             void *(*elem_try_alloc)(cobj_Error *const err),
                                             void (*elem_free)(void *elem),
                                             bool (*elem_try_copy)(void *const elem,
                                                                   void const *const src,
                                                                   cobj_Error *const err));

#endif //! defined(COBJ_BOXV_H)
