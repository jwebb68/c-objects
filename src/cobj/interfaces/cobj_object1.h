#if !defined(COBJ_OBJECT1_H)
#    define COBJ_OBJECT1_H

typedef struct cobj_Object1_s cobj_Object1;
struct cobj_Object1_s;

#    include <cobj/interfaces/cobj_interface.h>
#    include <cobj/interfaces/cobj_interface1.h>
//#include "cobj/interfaces/cobj_interface3.h"

struct cobj_Object1_s {
    // cobj_Value value1;
    // cobj_Value value2;
    // cobj_Value value3;

    // interface members, can go anywhere..
    cobj_Interface1 iface1;
    // cobj_Interface3 iface3;
    //  TODO: maybe wrap interface members in a DECLARE_INTERFACE(ifacecls) macro..
};

void cobj_Object1_destroy(cobj_Object1 *const self);

// struct must be bitwise copyable.. (why?)
// keep things off the heap if possible, that way stack/global usage is possible.
void cobj_Object1_new(cobj_Object1 *const self);

bool cobj_Object1_query_interface(cobj_Object1 const *const self,
                                  cobj_Error *const ret,
                                  cobj_InterfaceID const *const iid,
                                  void const **const out);

bool cobj_Object1_query_interface_mut(cobj_Object1 *const self,
                                      cobj_Error *const err,
                                      cobj_InterfaceID const *const iid,
                                      void **const out);

void cobj_Object1_foo(cobj_Object1 *const self);
bool cobj_Object1_foo2(cobj_Object1 *const self, cobj_Error *const err);
bool cobj_Object1_foo3(cobj_Error *const err, cobj_Object1 *const self);

// bool WARN_UNUSED_RESULT cobj_Object1_get_value1(
//     cobj_Object1 const *const self,
//     cobj_Error *const err,
//     cobj_Value *const val
// );
// bool WARN_UNUSED_RESULT cobj_Object1_set_value1(
//     cobj_Object1 *const self,
//     cobj_Error *const err,
//     cobj_Value const *const arg
// );
// bool WARN_UNUSED_RESULT cobj_Object1_get_value2(
//     cobj_Object1 const *const self,
//     cobj_Error *const err,
//     cobj_Value *const val
// );
// bool WARN_UNUSED_RESULT cobj_Object1_set_value2(
//     cobj_Object1 *const self,
//     cobj_Error *const err,
//     cobj_Value const *const arg
// );
// bool WARN_UNUSED_RESULT cobj_Object1_get_value3(
//     cobj_Object1 const *const self,
//     cobj_Error *const err,
//     cobj_Value *const val
// );
// bool WARN_UNUSED_RESULT cobj_Object1_set_value3(
//     cobj_Object1 *const self,
//     cobj_Error *const err,
//     cobj_Value const *const arg
// );
// bool WARN_UNUSED_RESULT cobj_Object1_action1(
//     cobj_Object1 *const self,
//     cobj_Error *const err,
//     cobj_Value const *const arg,
//     cobj_Value *const out
// );
// bool WARN_UNUSED_RESULT cobj_Object1_action2(
//     cobj_Object1 *const self,
//     cobj_Error *const err,
//     cobj_Value const *const arg,
//     cobj_Value *const out
// );

#endif // defined(COBJ_OBJECT1_H)
