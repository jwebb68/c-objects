#if !defined(COBJ_VECTORVT_H)
#    define COBJ_VECTORVT_H

/**
 * cobj_VectorV<T>
 *
 * A container of dynamically set initialised Ts
 *
 * This is the non-heap version, ( well a private heap).
 *
 * The Array owns the set values, so when destroyed, will destroy the Ts set into it.
 *
 * This version is the veneer layer using cobj_VectorV as the actual implementation.
 * Used to reduce the code footprint if using it.
 * But when debugging, the value won't be visible in the debugger (it'll be a block of bytes,
 * not deconstructed as most debuggers are able to do).
 * It treats the value as a block of ram, so specialisations of operations
 * operating on T are passed in.
 * This may prevent some optimisations that are available, compared to cobj_VectorT.
 *
 * WARNING: might not be viable in macro form as support funcs are needed on a per-type basis.
 */

typedef struct cobj_VectorVT_s cobj_VectorVT;

#    include <cobj/based/cobj_slicevt.h>
#    include <cobj/based/cobj_vectorv.h>
#    include <cobj/t.h>

struct cobj_VectorVT_s {
    cobj_VectorV inner;
};

void cobj_VectorVT_destroy_member(cobj_VectorVT *const self);
void cobj_VectorVT_destroy(cobj_VectorVT *const self);
void cobj_VectorVT_move_member(cobj_VectorVT *const self, cobj_VectorVT *const src);
void cobj_VectorVT_move(cobj_VectorVT *const self, cobj_VectorVT *const src);
// void cobj_VectorVT_new(cobj_VectorVT *const self, T *const b, T *const e);
void cobj_VectorVT_new(cobj_VectorVT *const self, T *const ptr, size_t alloc);
bool cobj_VectorVT_is_empty(cobj_VectorVT const *const self);
void cobj_VectorVT_clear(cobj_VectorVT *const self);
size_t cobj_VectorVT_len(cobj_VectorVT const *const self);
bool WARN_UNUSED_RESULT cobj_VectorVT_push_back(cobj_VectorVT *const self, T *const elem);
bool WARN_UNUSED_RESULT cobj_VectorVT_pop_back(cobj_VectorVT *const self, T *const elem);
T *cobj_VectorVT_try_get_mut(cobj_VectorVT const *const self, cobj_Index const pos);
T const *cobj_VectorVT_try_get(cobj_VectorVT const *const self, cobj_Index const pos);

void cobj_VectorVT_as_cobj_SliceT(cobj_VectorVT const *const self, cobj_SliceVT *const s);
void cobj_VectorVT_as_cobj_SliceTMut(cobj_VectorVT const *const self, cobj_SliceVTMut *const s);

void cobj_VectorVT_iter(cobj_VectorVT const *const self, cobj_SliceVTIter *const it);
void cobj_VectorVT_iter_mut(cobj_VectorVT const *const self, cobj_SliceVTMutIter *const it);

// the V/VT style means that I wouldn't see T in the debugger for the struct.
// Is there a way of keeping cobj_VectorT struct and using cobj_VectorV for functions?
// - maybe for some, but not for all.

// or as macros (if you must)
// #    define m_cobj_VectorVT(T, name) cobj_VectorV name;
// #    define m_cobj_VectorVT_destroy(T, self) cobj_VectorV_destroy((self), ##T_destroy)
// #    define m_cobj_VectorVT_move(T, self) cobj_VectorV_move((self), (src))
// #    define m_cobj_VectorVT_try_copy(T, self) cobj_VectorV_try_copy((self), (src), ##T_try_copy)
// #    define m_cobj_VectorVT_new(T, self, arr, len) cobj_VectorV_new((self), (arr), (len))
// #    define m_cobj_VectorVT_is_empty(T, self) cobj_VectorV_is_empty((self))
// #    define m_cobj_VectorVT_clear(T, self) cobj_VectorV_clear((self))
// #    define m_cobj_VectorVT_len(T, self) cobj_VectorV_len((self))
// #    define m_cobj_VectorVT_push_back(T, self, elem) cobj_VectorV_push_back((self), (elem),
// ##T_move) #    define m_cobj_VectorVT_pop_back(T, self, elem) cobj_VectorV_pop_back((self),
// (elem), ##T_move) #    define m_cobj_VectorVT_try_get_mut(T, self, pos)
// cobj_VectorV_try_get_mut((self), (pos)) #    define m_cobj_VectorVT_try_get(T, self, pos)
// cobj_VectorV_try_get((self), (pos)) #    define m_cobj_VectorVT_as_slice(T, self, s)
// cobj_VectorV_as_slice((self), (s)) #    define m_cobj_VectorVT_as_slice_mut(T, self, s)
// cobj_VectorV_as_slice_mut((self), (s)) #    define m_cobj_VectorVT_iter(T, self, it)
// cobj_VectorV_iter((self), (it)) #    define m_cobj_VectorVT_iter_mut(T, self, it)
// cobj_VectorV_as_slice_mut((self), (it))

#endif //! defined(COBJ_VECTORVT_H)
