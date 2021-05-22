#if !defined(COBJ_BOXVT_H)
#define COBJ_BOXVT_H

// forward declares
typedef struct BoxVTMut_ BoxVTMut;

// includes
#include "t.h"
#include "cobj_boxv.h"

// defines
struct BoxVTMut_ {
    BoxVMut inner;
};
void BoxVTMut_destroy(BoxVTMut *const self);
void BoxVTMut_move(BoxVTMut *const self, BoxVTMut *const src);
T *BoxVTMut_deref(BoxVTMut const *const self);
void BoxVTMut_new_own(BoxVTMut *const self, T *const p);
//void BoxVTMut_new_(BoxVTMut_ *const self, ...);


#endif//!defined(COBJ_BOXVT_H)
