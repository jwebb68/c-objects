#if !defined(COBJ_BOXV_H)
#define COBJ_BOXV_H

//forward declares
typedef struct BoxVMut_ BoxVMut;


// includes

//defines

struct BoxVMut_ {
    void *elem;
};
void BoxVMut_destroy(BoxVMut *const self, void (*elem_destroy)(void *const elem));
void BoxVMut_move(BoxVMut *const self, BoxVMut *const src);
void *BoxVMut_deref(BoxVMut const *const self);

// create and take ownsership of existing heap obj
// no disown as that is an implicit destroy.
void BoxVMut_new_own(BoxVMut *const self, void *const p);
//void BoxVMut_new_(BoxVMut_ *const self, ...);

// hmm, using the void * as implementation would prevent debug lookups.
// also, the two implementations look the same so won't get any code size reductions.
// prob simpler/easier to go with the non-V version


#endif//!defined(COBJ_BOXV_H)
