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
                                                       cobj_Error *const err,
                                                       cobj_InterfaceID const *const iid,
                                                       void const **const val,
                                                       cobj_InterfaceLookup const *begin,
                                                       cobj_InterfaceLookup const *end);

bool WARN_UNUSED_RESULT cobj_Interface_query_interface_mut(void *inst,
                                                           cobj_Error *const err,
                                                           cobj_InterfaceID const *const iid,
                                                           void **const val,
                                                           cobj_InterfaceLookup const *begin,
                                                           cobj_InterfaceLookup const *end);

#    define COBJ_INTERFACE_GET_OBJPTR(objtype, ifaceptr) \
        ((objtype const *)((uint8_t const *)ifaceptr - ifaceptr->_vtbl->offset))
#    define COBJ_INTERFACE_GET_OBJPTR_MUT(objtype, ifaceptr) \
        ((objtype *)((uint8_t *)ifaceptr - ifaceptr->_vtbl->offset))

#    if 0
#        define FDECL_INTERFACE(iname) \
            typedef struct##iname_VTBL_s##iname_VTBL; \
            typedef struct##iname_s##iname

#        define DEF_INTERFACE(iname) \
            struct##iname_s { \
                ##iname_VTBL const *const _vtbl; \
            }; \
            extern InterfaceID const *const##iname_iid;
struct ##iname_VTBL_s {
    \
##iname_VTBL const * const token; \
        size_t adj;

#        define ENDDEF_INTERFACE }

#        define IMPL_INTERFACE(iname) InterfaceID const##iname_iid = "##iname"

#        define DECL_INTERFACE_FOR(iname, sname) ##iname _##iname

#        define IMPL_INTERFACE_FOR(iname, sname) \
            static##iname_VTBL const##iname = {.token = &##iname,
    .adj = offsetof(sname, _##iname),

#        define ENDIMPL_INTERFACE }

#        define IMPL_INTERFACE_MAP_FOR(sname) static InterfaceEntry const##sname_ifacemap[] = {
#        define INTERFACE_ENTRY_FOR(iid, iname) {iid, offsetof(sname, _##iname)},

#        define END_INTERFACE_MAP_FOR \
            } \
            ;
#    endif

#endif // defined(COBJ_INTERFACE_H)
