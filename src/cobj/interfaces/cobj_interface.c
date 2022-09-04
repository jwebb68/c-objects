#include <cobj/core/cobj_panic.h>
#include <cobj/interfaces/cobj_interface.h>

bool cobj_Interface_query_interface(void const *inst,
                                    cobj_Error *const err,
                                    cobj_InterfaceID const *const iid,
                                    void const **val,
                                    cobj_InterfaceLookup const *begin,
                                    cobj_InterfaceLookup const *end)
{
    for (cobj_InterfaceLookup const *it = begin; it != end; ++it) {
        if (it->iid == iid) {
            // nasty, casting away type.
            // but how else to get it generic in C..
            // also nastiness, adjusting pointers by bytes.
            *val = (void const *)((uint8_t const *)inst + it->offset);
            return true;
        }
    }
    return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EINVALID);
}

bool cobj_Interface_query_interface_mut(void *inst,
                                        cobj_Error *const err,
                                        cobj_InterfaceID const *const iid,
                                        void **val,
                                        cobj_InterfaceLookup const *begin,
                                        cobj_InterfaceLookup const *end)
{
    for (cobj_InterfaceLookup const *it = begin; it != end; ++it) {
        if (it->iid == iid) {
            // nasty, casting away type.
            // but how else to get it generic in C..
            // also nastiness, adjusting pointers by bytes.
            *val = (void *)((uint8_t *)inst + it->offset);
            return true;
        }
    }
    return COBJ_ERROR_RAISE(err, cobj_ErrorCode_EINVALID);
}
