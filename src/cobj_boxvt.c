#include "cobj_boxvt.h"


static void BoxVTMut_destroy_elem(void *const self) {
    T_destroy((T *)self);
}


void BoxVTMut_destroy(BoxVTMut *const self) {
    BoxVMut_destroy(&self->inner, BoxVTMut_destroy_elem);
}


void BoxVTMut_move(BoxVTMut *const self, BoxVTMut *const src) {
    BoxVMut_move(&self->inner, &src->inner);
}


T *BoxVTMut_deref(BoxVTMut const *const self) {
    return (T *)BoxVMut_deref(&self->inner);
}


void BoxVTMut_new_own(BoxVTMut *const self, T *const p) {
    BoxVMut_new_own(&self->inner, p);
}


//void BoxVTMut_new_(BoxVTMut_ *const self, ...) {
//  T_new(&self->elem, ...);
//}
