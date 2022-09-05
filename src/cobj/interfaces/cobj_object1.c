#include <cobj/core/cobj_carray.h>
#include <cobj/interfaces/cobj_interface.h>
#include <cobj/interfaces/cobj_object1.h>
#include <inttypes.h>
#include <memory.h>
#include <stddef.h>

static cobj_InterfaceLookup const cobj_Object1_ifaces[] = {
    {&cobj_Interface1_ID, offsetof(cobj_Object1, iface1)},
    //{&cobj_Interface3_ID, offsetof(cobj_Object1, iface3)},
};
// TODO maybe wrap this in DEFINE_INTERFACEMAP(cls)/INTERFACE()/END_INTERFACEMAP macros

bool WARN_UNUSED_RESULT cobj_Object1_query_interface(cobj_Object1 const *const self,
                                                     cobj_InterfaceID const *const iid,
                                                     void const **const val,
                                                     cobj_Error *const err)
{
    // no guard needed here as not relying on values in the object struct instance.
    // although a guard could be added to verify it's an instance of the expected type.
    return cobj_Interface_query_interface(self,
                                          iid,
                                          val,
                                          err,
                                          COBJ_CARRAY_BEGIN(cobj_Object1_ifaces),
                                          COBJ_CARRAY_END(cobj_Object1_ifaces));
}

bool WARN_UNUSED_RESULT cobj_Object1_query_interface_mut(cobj_Object1 *const self,
                                                         cobj_InterfaceID const *const iid,
                                                         void **const val,
                                                         cobj_Error *const err)
{
    // no guard needed here as not relying on values in the object struct instance.
    // although a guard could be added to verify it's an instance of the expected type.
    return cobj_Interface_query_interface_mut(self,
                                              iid,
                                              val,
                                              err,
                                              COBJ_CARRAY_BEGIN(cobj_Object1_ifaces),
                                              COBJ_CARRAY_END(cobj_Object1_ifaces));
}

///////////////////////////////////////////////////////////////////////////////

static cobj_Object1 const *cobj_Object1_Interface1_get_objptr(cobj_Interface1 const *const self)
{
    // could recheck self->_vtbl again, on the rare/slim chance that it's been corrupted.
    // TODO: check self vtbl is expected one before accessing for the adjustment for self
    cobj_Object1 const *const oself = COBJ_INTERFACE_GET_OBJPTR(cobj_Object1, self);
    return oself;
}

static cobj_Object1 *cobj_Object1_Interface1_get_objptr_mut(cobj_Interface1 *const self)
{
    // could recheck self->_vtbl again, on the rare/slim chance that it's been corrupted.
    // TODO: check self vtbl is expected one before accessing for the adjustment for self
    cobj_Object1 *const oself = COBJ_INTERFACE_GET_OBJPTR_MUT(cobj_Object1, self);
    return oself;
}

static bool WARN_UNUSED_RESULT
cobj_Object1_Interface1_query_interface(cobj_Interface1 const *const self,
                                        cobj_InterfaceID const *const iid,
                                        void const **const val,
                                        cobj_Error *const err)
{
    cobj_Object1 const *const oself = cobj_Object1_Interface1_get_objptr(self);
    return cobj_Object1_query_interface(oself, iid, val, err);
}

static bool WARN_UNUSED_RESULT
cobj_Object1_Interface1_query_interface_mut(cobj_Interface1 *const self,
                                            cobj_InterfaceID const *const iid,
                                            void **const val,
                                            cobj_Error *const err)
{
    cobj_Object1 *const oself = cobj_Object1_Interface1_get_objptr_mut(self);
    return cobj_Object1_query_interface_mut(oself, iid, val, err);
}

// ===========================================================================

static void cobj_Object1_Interface1_foo(cobj_Interface1 *const self)
{
    cobj_Object1 *const oself = cobj_Object1_Interface1_get_objptr_mut(self);
    cobj_Object1_foo(oself);
}

static bool WARN_UNUSED_RESULT cobj_Object1_Interface1_foo2(cobj_Interface1 *const self,
                                                            cobj_Error *const err)
{
    cobj_Object1 *const oself = cobj_Object1_Interface1_get_objptr_mut(self);
    return cobj_Object1_foo2(oself, err);
}

static bool WARN_UNUSED_RESULT cobj_Object1_Interface1_foo3(cobj_Error *const err,
                                                            cobj_Interface1 *const self)
{
    cobj_Object1 *const oself = cobj_Object1_Interface1_get_objptr_mut(self);
    return cobj_Object1_foo3(err, oself);
}

static cobj_Interface1Vtbl const cobj_Object1_Interface1_vtbl = {
    &cobj_Object1_Interface1_vtbl,
    offsetof(cobj_Object1, iface1),
    cobj_Object1_Interface1_query_interface,
    cobj_Object1_Interface1_query_interface_mut,
    cobj_Object1_Interface1_foo,
    cobj_Object1_Interface1_foo2,
    cobj_Object1_Interface1_foo3,
};

// ===========================================================================
#if 0
static cobj_Object1 const *cobj_Object1_Interface3_get_objptr(
    cobj_Interface3 const *const self)
{
    // could recheck self->_vtbl again, on the rare/slim chance that it's been corrupted.
    // TODO: check self vtbl is expected one before accessing for the adjustment for self
    cobj_Object1 const *const oself = COBJ_INTERFACE_GET_OBJPTR(cobj_Object1, self);
    return oself;
}


static cobj_Object1 *cobj_Object1_Interface3_get_objptr_mut(
    cobj_Interface3 *const self)
{
    // could recheck self->_vtbl again, on the rare/slim chance that it's been corrupted.
    // TODO: check self vtbl is expected one before accessing for the adjustment for self
    cobj_Object1 *const oself = COBJ_INTERFACE_GET_OBJPTR_MUT(cobj_Object1, self);
    return oself;
}

static bool WARN_UNUSED_RESULT cobj_Object1_Interface3_query_interface(
    cobj_Interface3 const *const self,
    cobj_Error *const ret,
    cobj_InterfaceID const *const iid,
    void const **const val
)
{
    cobj_Object1_query_interface(cobj_Object1_Interface3_get_objptr(self),
                                 ret,
                                 iid,
                                 val);
}

static bool WARN_UNUSED_RESULT cobj_Object1_Interface3_query_interface_mut(
    cobj_Interface3 *const self,
    cobj_Error *const ret,
    cobj_InterfaceID const *const iid,
    void **const val
)
{
    cobj_Object1_query_interface_mut(
        cobj_Object1_Interface3_get_objptr_mut(self),
        ret,
        iid,
        val);
}

static void cobj_Object1_Interface3_get_value1(
    cobj_Interface3 const *const self,
    cobj_Error *const ret,
    cobj_Value *const val)
{
    cobj_Object1_get_value1(cobj_Object1_Interface3_get_objptr(self), ret, val);
}


static void cobj_Object1_Interface3_set_value1(
    cobj_Interface3 *const self,
    cobj_Error *const ret,
    cobj_Value const *const arg
)
{
    cobj_Object1_set_value1(cobj_Object1_Interface3_get_objptr_mut(self), ret, arg);
}

static void cobj_Object1_Interface3_get_value2(
    cobj_Interface3 const *const self,
    cobj_Error *const ret,
    cobj_Value *const val
)
{
    cobj_Object1_get_value2(cobj_Object1_Interface3_get_objptr(self), ret, val);
}


static void cobj_Object1_Interface3_set_value2(
    cobj_Interface3 *const self,
    cobj_Error *const ret,
    cobj_Value const *const arg
)
{
    cobj_Object1_set_value2(cobj_Object1_Interface3_get_objptr_mut(self), ret, arg);
}


static void cobj_Object1_Interface3_get_value3(
    cobj_Interface3 const *const self,
    cobj_Error *const ret,
    cobj_Value *const val)
{
    cobj_Object1_get_value3(cobj_Object1_Interface3_get_objptr(self), ret, val);
}


static void cobj_Object1_Interface3_set_value3(
    cobj_Interface3 *const self,
    cobj_Error *const ret,
    cobj_Value const *const arg
)
{
    cobj_Object1_set_value3(cobj_Object1_Interface3_get_objptr_mut(self), ret, arg);
}

static void cobj_Object1_Interface3_action1(
    cobj_Interface3 *const self,
    cobj_Error *const ret,
    cobj_Value const *const arg,
    cobj_Value *const out
)
{
    cobj_Object1_action1(cobj_Object1_Interface3_get_objptr_mut(self), ret, arg, out);
}


static void cobj_Object1_Interface3_action2(
    cobj_Interface3 *const self,
    cobj_Error *const ret,
    cobj_Value const *const arg,
    cobj_Value *const out
)
{
    cobj_Object1_action2(cobj_Object1_Interface3_get_objptr_mut(self), ret, arg, out);
}


static cobj_Interface3Vtbl const cobj_Object1_Interface3_vtbl = {
    &cobj_Object1_Interface3_vtbl,
    offsetof(cobj_Object1, iface3),
    cobj_Object1_Interface3_query_interface,
    cobj_Object1_Interface3_query_interface_mut,
    cobj_Interface3_get,
    cobj_Interface3_set,
    cobj_Interface3_action,
    cobj_Object1_Interface3_get_value1,
    cobj_Object1_Interface3_set_value1,
    cobj_Object1_Interface3_get_value2,
    cobj_Object1_Interface3_set_value2,
    cobj_Object1_Interface3_get_value3,
    cobj_Object1_Interface3_set_value3,
    cobj_Object1_Interface3_action1,
    cobj_Object1_Interface3_action2,
};

#endif

///////////////////////////////////////////////////////////////////////////////

void cobj_Object1_destroy(cobj_Object1 *const self)
{
    // completely corrupt object, including interface vtlb pointers (oops).
    // should be able to use any value here, but
    // on amd64 without a os level function to determine readability, then
    // use 0x0 (NULL)
    memset(self, 0xa5, sizeof(*self));
}

void cobj_Object1_new(cobj_Object1 *const self)
{
    cobj_Interface1_new(&self->iface1, &cobj_Object1_Interface1_vtbl);
    // cobj_Value_new(&self->value);
    //  self->value1 = 0;
    //  self->value2 = 1;
    //  self->value3 = 2;
}

void cobj_Object1_foo(cobj_Object1 *const self)
{
    COBJ_UNUSED_ARG(self);
}

bool WARN_UNUSED_RESULT cobj_Object1_foo2(cobj_Object1 *const self, cobj_Error *const err)
{
    COBJ_UNUSED_ARG(self);
    return COBJ_ERROR_RAISE(err, cobj_ErrorCode_ENOTIMPL);
}

bool WARN_UNUSED_RESULT cobj_Object1_foo3(cobj_Error *const err, cobj_Object1 *const self)
{
    COBJ_UNUSED_ARG(self);
    return COBJ_ERROR_RAISE(err, cobj_ErrorCode_ENOTIMPL);
}

#if 0
bool WARN_UNUSED_RESULT cobj_Object1_get_value1(
    cobj_Object1 const *const self,
    cobj_Error *const err,
    cobj_Value *const val
)
{
    *val = self->value1;
    return true;
}


bool WARN_UNUSED_RESULT cobj_Object1_set_value1(
    cobj_Object1 *const self,
    cobj_Error *const err,
    cobj_Value const *const arg
)
{
    self->value1 = *arg;
    return true;
}

bool WARN_UNUSED_RESULT cobj_Object1_get_value2(
    cobj_Object1 const *const self,
    cobj_Error *const err,
    cobj_Value *const val
)
{
    *val = self->value2;
    return true;
}


bool WARN_UNUSED_RESULT cobj_Object1_set_value2(
    cobj_Object1 *const self,
    cobj_Error *const err,
    cobj_Value const *const arg
)
{
    self->value2 = *arg;
    return true;
}

bool WARN_UNUSED_RESULT cobj_Object1_get_value3(
    cobj_Object1 const *const self,
    cobj_Error *const err,
    cobj_Value *const val
)
{
    *val = self->value3;
    return true;
}


bool WARN_UNUSED_RESULT cobj_Object1_set_value3(
    cobj_Object1 *const self,
    cobj_Error *const err,
    cobj_Value const *const arg
)
{
    self->value3 = *arg;
    return true;
}

bool WARN_UNUSED_RESULT cobj_Object1_action1(
    cobj_Object1 *const self,
    cobj_Error *const err,
    cobj_Value const *const arg,
    cobj_Value *const out
)
{
    *out = self->value1 + *arg + 23;
    return true;
}

bool WARN_UNUSED_RESULT cobj_Object1_action2(
    cobj_Object1 *const self,
    cobj_Error *const err,
    cobj_Value const *const arg,
    cobj_Value *const out
)
{
    UNUSED_ARG(self);
    UNUSED_ARG(arg);
    UNUSED_ARG(out);
    return COBJ_ERROR_RAISE(err, cobj_ErrorCode_ENOTIMPL);
}
#endif
