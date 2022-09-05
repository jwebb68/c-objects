#if !defined(COBJ_INTERFACE2_H)
#    define COBJ_INTERFACE2_H

#    include <cobj/interfaces/cobj_interface.h>

COBJ_INTERFACE_DECL(cobj_Interface2)

COBJ_INTERFACE_DEF(cobj_Interface2)
void (*spam)(cobj_Interface2 *const self);
COBJ_INTERFACE_ENDDEF()

#endif // defined(COBJ_INTERFACE2_H)
