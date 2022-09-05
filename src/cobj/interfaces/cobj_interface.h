#if !defined(COBJ_INTERFACE_H)
#    define COBJ_INTERFACE_H

typedef struct cobj_InterfaceLookup_s cobj_InterfaceLookup;
struct cobj_InterfaceLookup_s;

#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <cobj/core/cobj_panic.h>
#    include <inttypes.h>
#    include <stddef.h>

// maybe use a uuid, then fixed size regardless
// but need some sort of reg to map uuid to string name.
typedef char const *cobj_InterfaceID;

// check the self does indeed point to an interface. panic if not.
// I want the jump through the fn-ptr to be small/fast.
// So: when inlined, I don't want the arg regs and callstack to be changed in any way.
// the compare of the vtbl and the token needs to happen before pushing any args
// so cannot be a function (even an inline one).
// otherwise function prolog/epilog is emitted and breaks the no callstack/reg change mandate.
#    define cobj_Interface_check(self) \
        cobj_panic_if(self->_vtbl == self->_vtbl->token, "Bad Interface")

// TODO: might be messy
#    define cobj_Interface_try_check(self, err) \
        COBJ_ERROR_RAISE_IF(self->_vtbl == self->_vtbl->token, err, cobj_ErrorCode_EINVAL)

struct cobj_InterfaceLookup_s {
    cobj_InterfaceID const *iid;
    size_t offset;
};

bool WARN_UNUSED_RESULT cobj_Interface_query_interface(void const *inst,
                                                       cobj_InterfaceID const *const iid,
                                                       void const **const val,
                                                       cobj_Error *const err,
                                                       cobj_InterfaceLookup const *begin,
                                                       cobj_InterfaceLookup const *end);

bool WARN_UNUSED_RESULT cobj_Interface_query_interface_mut(void *inst,
                                                           cobj_InterfaceID const *const iid,
                                                           void **const val,
                                                           cobj_Error *const err,
                                                           cobj_InterfaceLookup const *begin,
                                                           cobj_InterfaceLookup const *end);

#    define COBJ_INTERFACE_GET_OBJPTR(objtype, ifaceptr) \
        ((objtype const *)((uint8_t const *)ifaceptr - ifaceptr->_vtbl->offset))
#    define COBJ_INTERFACE_GET_OBJPTR_MUT(objtype, ifaceptr) \
        ((objtype *)((uint8_t *)ifaceptr - ifaceptr->_vtbl->offset))

#    if 1
#        define COBJ_INTERFACE_DECL(iname) \
            typedef struct iname##_VTBL_s iname##_VTBL; \
            typedef struct iname##_s iname;

#        define COBJ_INTERFACE_DEF(iname) \
            extern cobj_InterfaceID const iname##_id; \
            struct iname##_s { \
                iname##_VTBL const *const _vtbl; \
            }; \
            void iname##_new(iname *const self, iname##_VTBL const *const vtbl); \
            struct iname##_VTBL_s { \
                iname##_VTBL const *const token; \
                size_t offset; \
                bool (*query_interface)(iname const *const self, \
                                        cobj_InterfaceID const *const iid, \
                                        void const **const val, \
                                        cobj_Error *const err); \
                bool (*query_interface_mut)(iname *const self, \
                                            cobj_InterfaceID const *const iid, \
                                            void **const val, \
                                            cobj_Error *const err);

#        define COBJ_INTERFACE_ENDDEF() \
            } \
            ;

#        define COBJ_INTERFACE_GET_VTBL(iname, self) iname##_get_vtbl(self)

#        define COBJ_INTERFACE_IMPL(iname) \
            void iname##_new(iname *const self, iname##_VTBL const *const vtbl) \
            { \
                *((iname##_VTBL const **)&self->_vtbl) = vtbl; \
            } \
            cobj_InterfaceID const iname##_id = #iname; \
            static iname##_VTBL const *iname##_get_vtbl(iname const *const self) \
            { \
                cobj_Interface_check(self); \
                return self->_vtbl; \
            } \
            bool WARN_UNUSED_RESULT iname##_query_interface(iname const *const self, \
                                                            cobj_InterfaceID const *const iid, \
                                                            void const **const val, \
                                                            cobj_Error *const err) \
            { \
                iname##_VTBL const *const vtbl = COBJ_INTERFACE_GET_VTBL(iname, self); \
                return vtbl->query_interface(self, iid, val, err); \
            } \
            bool WARN_UNUSED_RESULT iname##_query_interface_mut(iname *const self, \
                                                                cobj_InterfaceID const *const iid, \
                                                                void **const val, \
                                                                cobj_Error *const err) \
            { \
                iname##_VTBL const *const vtbl = COBJ_INTERFACE_GET_VTBL(iname, self); \
                return vtbl->query_interface_mut(self, iid, val, err); \
            }

#        define COBJ_INTERFACE_DECL_FOR(sname, iname) iname _##iname;

#        define COBJ_INTERFACE_IMPL_FOR(sname, iname) \
            bool WARN_UNUSED_RESULT \
                sname##_##iname##_query_interface(iname const *const self, \
                                                  cobj_InterfaceID const *const iid, \
                                                  void const **const val, \
                                                  cobj_Error *const err) \
            { \
                sname const *const oself = COBJ_INTERFACE_GET_OBJPTR(sname, self); \
                return sname##_query_interface(oself, iid, val, err); \
            } \
            bool WARN_UNUSED_RESULT \
                sname##_##iname##_query_interface_mut(iname *const self, \
                                                      cobj_InterfaceID const *const iid, \
                                                      void **const val, \
                                                      cobj_Error *const err) \
            { \
                sname *const oself = COBJ_INTERFACE_GET_OBJPTR_MUT(sname, self); \
                return sname##_query_interface_mut(oself, iid, val, err); \
            } \
            static iname##_VTBL const iname##_vtbl = {.token = &iname##_vtbl, \
                                                      .offset = offsetof(sname, _##iname), \
                                                      sname##_##iname##_query_interface, \
                                                      sname##_##iname##_query_interface_mut,

#        define COBJ_INTERFACE_IMPL_FOR_ENTRY(nam) nam,
#        define COBJ_INTERFACE_IMPL_ENDFOR() \
            } \
            ;

#        define COBJ_INTERFACE_MAP_BEGIN(sname) \
            static cobj_InterfaceLookup const sname##_ifaces[] = {
#        define COBJ_INTERFACE_MAP(sname, iname) {&iname##_id, offsetof(sname, _##iname)},

#        define COBJ_INTERFACE_MAP_END() \
            } \
            ;
#    endif

#endif // defined(COBJ_INTERFACE_H)
