#include <cobj/core/cobj_carray.h>
#include <cobj/interfaces/cobj_interface.h>
#include <cobj/interfaces/cobj_interface1.h>
//#include <cobj/cobj_ptrvalidate.h>
#include <cobj/core/cobj_panic.h>

cobj_InterfaceID const cobj_Interface1_ID = "cobj_Interface1";

void cobj_Interface1_new(cobj_Interface1 *const self, cobj_Interface1Vtbl const *const vtbl)
{
    // nasty, casting away const to set the vtbl
    cobj_Interface1Vtbl const **const p = (cobj_Interface1Vtbl const **)&self->_vtbl;
    *p = vtbl;
}

static cobj_Interface1Vtbl const *cobj_Interface1_get_vtbl(cobj_Interface1 const *const self)
{
    cobj_Interface_check(self);
    return self->_vtbl;
}

bool WARN_UNUSED_RESULT cobj_Interface1_query_interface(cobj_Interface1 const *const self,
                                                        cobj_Error *const err,
                                                        cobj_InterfaceID const *const iid,
                                                        void const **const val)
{
    return cobj_Interface1_get_vtbl(self)->query_interface(self, err, iid, val);
}

bool WARN_UNUSED_RESULT cobj_Interface1_query_interface_mut(cobj_Interface1 *const self,
                                                            cobj_Error *const err,
                                                            cobj_InterfaceID const *const iid,
                                                            void **const val)
{
    return cobj_Interface1_get_vtbl(self)->query_interface_mut(self, err, iid, val);
}

/////////////////////////////////////////////////////////////////////////////

void cobj_Interface1_foo(cobj_Interface1 *const self)
{
    cobj_Interface1_get_vtbl(self)->foo(self);
}

bool cobj_Interface1_foo2(cobj_Interface1 *const self, cobj_Error *const err)
{
    return cobj_Interface1_get_vtbl(self)->foo2(self, err);
}

bool cobj_Interface1_foo3(cobj_Error *const err, cobj_Interface1 *const self)
{
    return cobj_Interface1_get_vtbl(self)->foo3(err, self);
}
