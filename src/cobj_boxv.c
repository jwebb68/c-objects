#include "cobj_boxv.h"
#include "cobj_memory.h"  // STRUCTWIPE


static void BoxVMut_wipe(BoxVMut *const self) {
    STRUCTWIPE(self);
}


void BoxVMut_destroy(BoxVMut *const self, void (*elem_destroy)(void *const elem)) {
    elem_destroy(&self->elem);
    BoxVMut_wipe(self);
}


void BoxVMut_move(BoxVMut *const self, BoxVMut *const src) {
    *self = *src;
    BoxVMut_wipe(src);
}


void *BoxVMut_deref(BoxVMut const *const self) {
    return self->elem;
}


// create and take ownsership of existing heap obj
// no disown as that is an implicit destroy.
void BoxVMut_new_own(BoxVMut *const self, void *const p) {
    self->elem = p;
}


//void BoxVMut_new_(BoxVMut_ *const self, ...) {
//  T_new(&self->elem, ...);
//}
