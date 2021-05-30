#if !defined(COBJ_VECTORVT_H)
#    define COBJ_VECTORVT_H

typedef struct VectorVT_ VectorVT;

#    include "cobj_slicevt.h"
#    include "cobj_vectorv.h"
#    include "t.h"

// in typed vector:
// the same as above, still messy, too much typing.
// macro each fn? - will dupe code at call site.
// macro whole thing? akk code in macros..
// would be nicer to use jinja2 to template vector and create as needed,
// then wouldn't need to split into a generic one, but maybe do so for space saving
//(no duplicate code at runtime?)
struct VectorVT_ {
    VectorV inner;
};
void VectorVT_destroy(VectorVT *const self);
void VectorVT_move(VectorVT *const self, VectorVT *const src);
void VectorVT_new(VectorVT *const self, T *const b, T *const e);
bool VectorVT_is_empty(VectorVT const *const self);
void VectorVT_clear(VectorVT *const self);
size_t VectorVT_len(VectorVT const *const self);
bool WARN_UNUSED_RESULT VectorVT_push_back(VectorVT *const self, T *const elem);
bool WARN_UNUSED_RESULT VectorVT_pop_back(VectorVT *const self, T *const elem);
T *VectorVT_get_item_at_mut(VectorVT const *const self, Index const pos);
T const *VectorVT_get_item_at(VectorVT const *const self, Index const pos);

void VectorVT_as_slice(VectorVT const *const self, SliceVT *const s);
void VectorVT_as_slice_mut(VectorVT const *const self, SliceVTMut *const s);

void VectorVT_iter(VectorVT const *const self, SliceVTIter *const it);
void VectorVT_iter_mut(VectorVT const *const self, SliceVTMutIter *const it);

// the V/VT style means that I wouldn't see T in the debugger for the struct.
// Is there a way of keeping VectorT struct and using VectorV for functions?
// - maybe for some, but not for all.

// or as macros (if you must)
#    define m_VectorVT(T, name) VectorV name;
#    define m_VectorVT_destroy(T, self) VectorV_destroy((self), ##T_destroy)
#    define m_VectorVT_move(T, self) VectorV_move((self), (src))
#    define m_VectorVT_try_copy(T, self) VectorV_try_copy((self), (src), ##T_try_copy)
#    define m_VectorVT_new(T, self, arr, len) VectorV_new((self), (arr), (len))
#    define m_VectorVT_is_empty(T, self) VectorV_is_empty((self))
#    define m_VectorVT_clear(T, self) VectorV_clear((self))
#    define m_VectorVT_len(T, self) VectorV_len((self))
#    define m_VectorVT_push_back(T, self, elem) VectorV_push_back((self), (elem), ##T_move)
#    define m_VectorVT_pop_back(T, self, elem) VectorV_pop_back((self), (elem), ##T_move)
#    define m_VectorVT_get_item_at_mut(T, self, pos) VectorV_get_item_at_mut((self), (pos))
#    define m_VectorVT_get_item_at(T, self, pos) VectorV_get_item_at((self), (pos))
#    define m_VectorVT_as_slice(T, self, s) VectorV_as_slice((self), (s))
#    define m_VectorVT_as_slice_mut(T, self, s) VectorV_as_slice_mut((self), (s))
#    define m_VectorVT_iter(T, self, it) VectorV_iter((self), (it))
#    define m_VectorVT_iter_mut(T, self, it) VectorV_as_slice_mut((self), (it))

#endif //! defined(COBJ_VECTORVT_H)
