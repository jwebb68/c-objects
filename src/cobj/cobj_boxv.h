#if !defined(COBJ_BOXV_H)
#    define COBJ_BOXV_H

/**
 * BoxV: object created and managed on the heap, without sharing.
 *
 * Box contents owned by the box and are not shared (use RCT for shared objects).
 * inherently a Mut; cannot move a const value into it, cannot copy into into a const box.
 *
 * This version is the underlying implementation for all BoxV<T> types.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to BoxT.
 */
// hmm, using the void * as implementation would prevent debug lookups.
// also, the two implementations look the same so won't get any code size reductions.
// prob simpler/easier to go with the non-V version

// forward declares
typedef struct BoxV_ BoxV;

// includes
#    include "cobj_error.h"

#    include <stdbool.h>
#    include <stddef.h> // size_t

// defines

struct BoxV_ {
    void *elem;
};

void BoxV_destroy_member(BoxV *const self,
                         void (*elem_destroy)(void *const elem),
                         void (*elem_free)(void *elem));
void BoxV_destroy(BoxV *const self,
                  void (*elem_destroy)(void *const elem),
                  void (*elem_free)(void *elem));

void BoxV_move_member(BoxV *const self, BoxV *const src);
void BoxV_move(BoxV *const self, BoxV *const src);

bool WARN_UNUSED_RESULT BoxV_try_copy_member(BoxV *const self,
                                             BoxV const *const v,
                                             Error *const err,
                                             void *(*elem_try_alloc)(Error *const err),
                                             void (*elem_free)(void *elem),
                                             bool (*elem_try_copy)(void *const elem,
                                                                   void const *const elem_src,
                                                                   Error *const err));
bool WARN_UNUSED_RESULT BoxV_try_copy(BoxV *const self,
                                      BoxV const *const v,
                                      Error *const err,
                                      void *(*elem_try_alloc)(Error *const err),
                                      void (*elem_free)(void *elem),
                                      bool (*elem_try_copy)(void *const elem,
                                                            void const *const elem_src,
                                                            Error *const err));

void const *BoxV_deref(BoxV const *const self);
void *BoxV_deref_mut(BoxV *const self);

// create and take ownsership of existing heap obj
// no NULLs
void BoxV_own(BoxV *const self, void *const p);
// hand ownership of heap obj to caller, destroys self
void *BoxV_disown_member(BoxV *const self);
void *BoxV_disown(BoxV *const self);

void BoxV_swap(BoxV *const self, BoxV *const other);

// why these public?
// void *
// BoxV_elem_try_alloc(BoxV *const self, Error *const err, void *(*elem_try_alloc)(Error *const
// err)); void BoxV_elem_free(BoxV *const self, void (*elem_free)(void *elem));

// move elem into box.
bool WARN_UNUSED_RESULT BoxV_try_from_V(BoxV *const self,
                                        void *const elem,
                                        Error *const err,
                                        void *(*elem_try_alloc)(Error *const err),
                                        void (*elem_move)(void *const elem, void *const src));
void BoxV_into_V(BoxV *const self,
                 void *const elem,
                 void (*elem_move)(void *const elem, void *const src),
                 void (*elem_free)(void *elem));

bool WARN_UNUSED_RESULT BoxV_try_copy_V(BoxV *const self,
                                        void const *const elem,
                                        Error *const err,
                                        void *(*elem_try_alloc)(Error *const err),
                                        void (*elem_free)(void *elem),
                                        bool (*elem_try_copy)(void *const elem,
                                                              void const *const src,
                                                              Error *const err));

// void BoxV_new...(BoxV_ *const self, ...);

#endif //! defined(COBJ_BOXV_H)
