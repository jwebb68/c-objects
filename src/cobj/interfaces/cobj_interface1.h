#if !defined(COBJ_INTERFACE1_H)
#    define COBJ_INTERFACE1_H

typedef struct cobj_Interface1Vtbl_s cobj_Interface1Vtbl;
typedef struct cobj_Interface1_s cobj_Interface1;
struct cobj_Interface1Vtbl_s;
struct cobj_Interface1_s;

#    include <cobj/interfaces/cobj_interface.h>
#    include <stddef.h>

// each interface needs:
// 1. an offset of iface member of class implementing interface (see object1.c)
// 2. a function to allow finding other interfaces from any interface (see object1.c)
// 3. an id of the interface, unique across all interfaces.

// pub
struct cobj_Interface1Vtbl_s {
    // for embedded, give ability to check that vtbl is the expected one
    // and the vtbl pointer has not been corrupted, which, if left unchecked
    // would result in calling into some random part of the address space.
    // It's something, albeit weak.
    cobj_Interface1Vtbl const *const token;

    // Offset from interace location in struct to the location of the containing struct.
    size_t offset;

    // TODO: create separate struct containing just the methods..?
    // can this be the interface?
    // doesn't matter, interface does not have functions on it
    // but vtable ptr does.

    /// the 'method' that needs to be on all interfaces.
    // a bit nasty in using void ** for the out-param, would prefer it to be
    // typed with the expected type, but it's C and you cannot do that..
    // Or I could use a Specific cobj_Result struct ( Result<void *, ErrorType>) -
    // nice although that is, it adds an extra struct + funcs to maintain
    // and results is manual generics expansion..
    bool WARN_UNUSED_RESULT (*query_interface)(cobj_Interface1 const *const self,
                                               cobj_Error *const err,
                                               cobj_InterfaceID const *const iid,
                                               void const **const val);

    bool WARN_UNUSED_RESULT (*query_interface_mut)(cobj_Interface1 *const self,
                                                   cobj_Error *const err,
                                                   cobj_InterfaceID const *const iid,
                                                   void **const val);

    ////
    // example of some 'methods' that are implemented in this interface.
    // there is an argument to be had regarding keeping implementation out of interfaces.
    //
    // They should always have interface ptr as first,
    // Maybe result as second (although seriously thinking of always having a result, even if unused
    // as this would prevent some register reallocations but would limit number of args that would
    // go in regs).
    void (*foo)(cobj_Interface1 *const self);

    bool (*foo2)(cobj_Interface1 *const self, cobj_Error *const err);

    bool (*foo3)(cobj_Error *const ret, cobj_Interface1 *const self);
};

// pub
struct cobj_Interface1_s {
    // for embedded (and indeed non-) vtbl must be const
    // this also complies with MISRA.
    cobj_Interface1Vtbl const *const _vtbl;
};

// pub
extern cobj_InterfaceID const cobj_Interface1_ID;

// pub
//  I want it inlined for speed/reduced code side
//  I want it a function, for type-safety reasons
void cobj_Interface1_new(cobj_Interface1 *const self, cobj_Interface1Vtbl const *const vtbl);

// Return the interface ptr for the interface identified by iid.
// if one such exists, return ok(), and ptr in *value (will need casting).
// if does not exist or on error, return Err().
// pub
bool WARN_UNUSED_RESULT cobj_Interface1_query_interface(cobj_Interface1 const *const self,
                                                        cobj_Error *const err,
                                                        cobj_InterfaceID const *const iid,
                                                        void const **const value);

// pub
bool WARN_UNUSED_RESULT cobj_Interface1_query_interface_mut(cobj_Interface1 *const self,
                                                            cobj_Error *const err,
                                                            cobj_InterfaceID const *const iid,
                                                            void **const value);

// pub
void cobj_Interface1_foo(cobj_Interface1 *const self);

// pub
bool WARN_UNUSED_RESULT cobj_Interface1_foo2(cobj_Interface1 *const self, cobj_Error *const err);

// pub
bool WARN_UNUSED_RESULT cobj_Interface1_foo3(cobj_Error *const err, cobj_Interface1 *const self);

#endif // defined(COBJ_INTERFACE1_H)
