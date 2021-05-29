#if !defined(COBJ_BOXV_H)
#    define COBJ_BOXV_H

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

void BoxV_destroy(BoxV *const self, void (*elem_destroy)(void *const elem));
void BoxV_move(BoxV *const self, BoxV *const src);
bool WARN_UNUSED_RESULT BoxV_try_copy(BoxV *const self,
                                      BoxV const *const v,
                                      Error *err,
                                      size_t elem_size,
                                      bool (*elem_try_copy)(void *elem,
                                                            void const *elem_src,
                                                            Error *err));

void const *BoxV_deref(BoxV const *const self);
void *BoxV_deref_mut(BoxV *const self);

// create and take ownsership of existing heap obj
// no disown as that is an implicit destroy.
void BoxV_new_own(BoxV *const self, void *const p);
// void BoxV_new_(BoxV_ *const self, ...);

bool WARN_UNUSED_RESULT BoxV_try_new_from(BoxV *const self,
                                          void *const elem,
                                          Error *const err,
                                          size_t elem_size,
                                          void (*elem_move)(void *elem, void *src));

// hmm, using the void * as implementation would prevent debug lookups.
// also, the two implementations look the same so won't get any code size reductions.
// prob simpler/easier to go with the non-V version

#endif //! defined(COBJ_BOXV_H)
