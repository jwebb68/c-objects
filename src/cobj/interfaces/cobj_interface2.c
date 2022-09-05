#include <cobj/interfaces/cobj_interface2.h>

COBJ_INTERFACE_IMPL(cobj_Interface2)

void cobj_Interface2_spam(cobj_Interface2 *const self)
{
    COBJ_INTERFACE_GET_VTBL(cobj_Interface2, self)->spam(self);
}
