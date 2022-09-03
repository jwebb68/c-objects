#if !defined(COBJ_VECTORVT_H)
#    define COBJ_VECTORVT_H

/**
 * VectorV<T>
 *
 * A container of dynamically set initialised Ts
 *
 * This is the non-heap version, ( well a private heap).
 *
 * The Array owns the set values, so when destroyed, will destroy the Ts set into it.
 *
 * This version is the veneer layer using VectorV as the actual implementation.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to VectorT.
 *
 * WARNING: might not be viable in macro form as support funcs are needed on a per-type basis.
 */

typedef struct VectorVT_ VectorVT;

#    include <cobj/based/cobj_slicevt.h>
#    include <cobj/based/cobj_vectorv.h>
#    include <cobj/t.h>

struct VectorVT_ {
    VectorV inner;
};

void VectorVT_destroy_member(VectorVT *const self);
void VectorVT_destroy(VectorVT *const self);
void VectorVT_move_member(VectorVT *const self, VectorVT *const src);
void VectorVT_move(VectorVT *const self, VectorVT *const src);
// void VectorVT_new(VectorVT *const self, T *const b, T *const e);
void VectorVT_new(VectorVT *const self, T *const ptr, size_t alloc);
bool VectorVT_is_empty(VectorVT const *const self);
void VectorVT_clear(VectorVT *const self);
size_t VectorVT_len(VectorVT const *const self);
bool WARN_UNUSED_RESULT VectorVT_push_back(VectorVT *const self, T *const elem);
bool WARN_UNUSED_RESULT VectorVT_pop_back(VectorVT *const self, T *const elem);
T *VectorVT_try_get_mut(VectorVT const *const self, cobj_Index const pos);
T const *VectorVT_try_get(VectorVT const *const self, cobj_Index const pos);

void VectorVT_as_SliceT(VectorVT const *const self, SliceVT *const s);
void VectorVT_as_SliceTMut(VectorVT const *const self, SliceVTMut *const s);

void VectorVT_iter(VectorVT const *const self, SliceVTIter *const it);
void VectorVT_iter_mut(VectorVT const *const self, SliceVTMutIter *const it);

// the V/VT style means that I wouldn't see T in the debugger for the struct.
// Is there a way of keeping VectorT struct and using VectorV for functions?
// - maybe for some, but not for all.

// or as macros (if you must)
// #    define m_VectorVT(T, name) VectorV name;
// #    define m_VectorVT_destroy(T, self) VectorV_destroy((self), ##T_destroy)
// #    define m_VectorVT_move(T, self) VectorV_move((self), (src))
// #    define m_VectorVT_try_copy(T, self) VectorV_try_copy((self), (src), ##T_try_copy)
// #    define m_VectorVT_new(T, self, arr, len) VectorV_new((self), (arr), (len))
// #    define m_VectorVT_is_empty(T, self) VectorV_is_empty((self))
// #    define m_VectorVT_clear(T, self) VectorV_clear((self))
// #    define m_VectorVT_len(T, self) VectorV_len((self))
// #    define m_VectorVT_push_back(T, self, elem) VectorV_push_back((self), (elem), ##T_move)
// #    define m_VectorVT_pop_back(T, self, elem) VectorV_pop_back((self), (elem), ##T_move)
// #    define m_VectorVT_try_get_mut(T, self, pos) VectorV_try_get_mut((self), (pos))
// #    define m_VectorVT_try_get(T, self, pos) VectorV_try_get((self), (pos))
// #    define m_VectorVT_as_slice(T, self, s) VectorV_as_slice((self), (s))
// #    define m_VectorVT_as_slice_mut(T, self, s) VectorV_as_slice_mut((self), (s))
// #    define m_VectorVT_iter(T, self, it) VectorV_iter((self), (it))
// #    define m_VectorVT_iter_mut(T, self, it) VectorV_as_slice_mut((self), (it))

#endif //! defined(COBJ_VECTORVT_H)
