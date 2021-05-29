#if !defined(COBJ_BOXVT_H)
#define COBJ_BOXVT_H

// forward declares
typedef struct BoxVT_ BoxVT;

// includes
#include "t.h"
#include "cobj_boxv.h"
#include "cobj_error.h"

#include <stdbool.h>
#include <stddef.h> // size_t

// defines
struct BoxVT_ {
    BoxV inner;
};

void BoxVT_destroy(BoxVT *const self);
void BoxVT_move(BoxVT *const self, BoxVT *const src);
bool WARN_UNUSED_RESULT BoxVT_try_copy(BoxVT *const self, BoxVT const *const v, Error *err);

T const *BoxVT_deref(BoxVT const *const self);
T *BoxVT_deref_mut(BoxVT *const self);

void BoxVT_new_own(BoxVT *const self, T *const p);
//void BoxVT_new_(BoxVT_ *const self, ...);

bool WARN_UNUSED_RESULT BoxVT_try_new_from_T(BoxVT *const self, T *const v, Error *err);


#endif//!defined(COBJ_BOXVT_H)
