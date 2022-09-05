# How to set up interfaces on structs and then to use them.

## Create interface definition and implementation.
- For each interface create 2 structs:
  - the interface struct itself (so you can define ptrs to it),
  - the vtbl struct (contains the ptrs to functions, usually 'hidden')

- Each interface needs to have the `query_interface` 'method'.
- for each method, create a stub/skel function for callers (hides the vtbl access)
- Interfaces do not have non-function members - put those on the object implementing the interface.

- Create an ID for the interface (can be simple as a charstring, or a bytestring)
Must be unique across all interfaces (past as well as future).

note: The names of the functions emulating the methods must be unique in the
      interface, but can be the same across interfaces.

```
interface1.h:

typedef struct Interface1VTbl_s Interface1VTbl;
typedef struct Interface1_s Interface1;

#include <cobj/interfaces/cobj_interface.h>

extern cobj_InterfaceID const Interface1_ID;

struct Interface1Vtbl_s {
    Interface1Vtbl const *const token;
    size_t offset;
    ////
    bool WARN_UNUSED_RESULT (*query_interface)(Interface1 const *const self, cobj_InterfaceID const *const iid, void const **const val, cobj_Error *const err);
    bool WARN_UNUSED_RESULT (*query_interface_mut)(Interface1 *const self, cobj_InterfaceID const *const iid, void **const val, cobj_Error *const err);
}

struct Interface1_s {
    Interface1Vtbl _vtbl;
}

bool WARN_UNUSED_RESULT Interface1_query_interface(Interface1 const *const self, cobj_InterfaceID const *const iid, void const **const val, cobj_Error *const err);

bool WARN_UNUSED_RESULT Interface1_query_interface_mut(Interface1 *const self, cobj_InterfaceID const *const iid, void **const val, cobj_Error *const err);

void Interface1_new(Interface1 *const self, Interface1Vtbl const *const vtbl);

```
```
interface1.c:

cobj_InterfaceID const Interface1_ID = "Interface1";

void Interface1_new(Interface1 *const self, Interface1Vtbl const *const vtbl)
{
    // nasty, casting away const to set the vtbl
    Interface1Vtbl const **const p = (Interface1Vtbl const **)&self->_vtbl;
    *p = vtbl;
}

static cobj_Interface1Vtbl const *cobj_Interface1_get_vtbl(cobj_Interface1 const *const self)
{
    cobj_Interface_check(self);
    return self->_vtbl;
}

bool WARN_UNUSED_RESULT Interface1_query_interface(Interface1 const *const self, cobj_InterfaceID const *const iid, void const **const val, cobj_Error *const err) {
    return cobj_Interface1_get_vtbl(self)->query_interface(self, err, iid, val);
}

bool WARN_UNUSED_RESULT Interface1_query_interface_mut(Interface1 *const self, cobj_InterfaceID const *const iid, void **const val, cobj_Error *const err) {
    return cobj_Interface1_get_vtbl(self)->query_interface_mut(self, err, iid, val);
}

```

## Define/create struct if not already created.

```
mystruct1.h:

typedef struct MyStruct1_s MyStruct1;

struct MyStruct1_s {
}

void MyStruct1_delete(MyStruct1 *const self);
void MyStruct1_move(MyStruct1 *const self, MyStruct1 *const src);
void MyStruct1_new(MyStruct1 *const self);

```
```
mystruct1.c:

#include "mystruct1.h"

void MyStruct1_wipe(MyStruct1 *const self) {
    memset(self,  0xa5, sizeof(*self));
}

void MyStruct1_delete(MyStruct1 *const self) {
    MyStruct1_wipe(self);
}

void MyStruct1_move(MyStruct1 *const self, MyStruct1 *const src) {
    *self = *src;
    MyStruct1_wipe(src);
}

void MyStruct1_new(MyStruct1 *const self) {
}
```

## implement interface on struct.

- add the interface struct to it anywhere.
- add skel functions to be added to the vtbl (convert interface ptr to struct ptr, before calling forward), these call 'methods' on the struct.
- define the vtbl  for the interface for this struct using those skel functions.

```
mystruct1.h:
...
#include <interface1.h>
...
struct MyStruct1_s {
...
    Interface1 iface1;
...
}
...
bool WARN_UNUSED_RESULT MyStruct1_query_interface(MyStruct1 const *const self, cobj_InterfaceIID const *const iid, void const **const val, cobj_Error *const err);
bool WARN_UNUSED_RESULT MyStruct1_query_interface_mut(MyStruct1 *const self, cobj_InterfaceIID const *const iid, void **const val, cobj_Error *const err);
...
```
```
mystruct1.c:
...
static Interface1VTbl const MyStruct1_Interface1_vtbl;
...
void MyStruct1_new(MyStruct1 *const self) {
...
   Interface1_new(&self->iface1, &MyStruct1_Interface1_vtbl);
...
}
...
///////////////////////////////////////////////////////////////////////////////
static cobj_InterfaceLookup const MyStruct1_ifaces[] = {
    {&Interface1_ID, offsetof(MyStruct1, iface1)},
    //{&cobj_Interface3_ID, offsetof(MyStruct1, iface3)},
};

bool WARN_UNUSED_RESULT MyStruct1_query_interface(MyStruct1 const *const self, cobj_InterfaceIID const *const iid, void const **const val, cobj_Error *const err)
{
    return cobj_Interface_query_interface(self,
                                          err,
                                          iid,
                                          val,
                                          COBJ_CARRAY_BEGIN(MyStruct1_ifaces),
                                          COBJ_CARRAY_END(MyStruct1_ifaces));
}

bool WARN_UNUSED_RESULT MyStruct1_query_interface_mut(MyStruct1 *const self, cobj_InterfaceIID const *const iid, void **const val, cobj_Error *const err)
{
    return cobj_Interface_query_interface_mut(self,
                                          err,
                                          iid,
                                          val,
                                          COBJ_CARRAY_BEGIN(MyStruct1_ifaces),
                                          COBJ_CARRAY_END(MyStruct1_ifaces));
}

////////////////////////////////////////////////////////////////////////////////

static MyStruct1 const *MyStruct1_Interface1_get_objptr(Interface1 const *const self)
{
    MyStruct1 const *const oself = COBJ_INTERFACE_GET_OBJPTR(MyStruct1, self);
    return oself;
}

static MyStruct1 *MyStruct1_Interface1_get_objptr_mut(Interface1 *const self)
{
    MyStruct1 *const oself = COBJ_INTERFACE_GET_OBJPTR_MUT(MyStruct1, self);
    return oself;
}

static bool WARN_UNUSED_RESULT MyStruct1_Interface1_query_interface(Interface1 const *const self, cobj_InterfaceIID const *const iid, void const **const val, cobj_Error *const err)
{
    MyStruct1 const *const oself = MyStruct1_Interface1_get_objptr(self);
    return MyStruct1_query_interface(oself, err, iid, val);
}

static bool WARN_UNUSED_RESULT MyStruct1_Interface1_query_interface_mut(Interface1 *const self, cobj_InterfaceIID const *const iid, void **const val, cobj_Error *const err)
{
    MyStruct1 *const oself = MyStruct1_Interface1_get_objptr_mut(self);
    return MyStruct1_query_interface_mut(oself, err, iid, val);
}

static Interface1VTbl const MyStruct1_Interface1_vtbl = {
    &MyStruct1_Interface1_vtbl,
    offsetof(MyStruct, iface1),
    MyStruct1_Interface1_query_interface,
    MyStruct1_Interface1_query_interface_mut,
}
```


## Create instance of struct and call 'methods' on interfaces regardless of struct type.
- but note: you are still responsible for the lifetime and scope of the struct.
- interfaces can point to any struct and still be type-safe to the interface (equivalence principle - the L in SOLID).

```
main.c:

int main(int argc, char *argv[]) {

    MyStruct1 ms1;
    MyStruct1_new(&ms1);

    Error err;
    bool ok;

    // get interface using obj directly.
    Interface1 *i1_1;
    ok = MyStruct1_query_interface_mut(&ms1, &Interface1_ID, (void **)&i1_1, &err);

    // get interface from another interface.
    Interface1 const *i1_2;
    ok = Interface1_query_interface(i1_1, &Interface1_ID, (void const **)&i1_2, &err);

    MyStruct1_delete(&ms1);
}
```


## implement new methods on an interface:
- add method to interface definition (in vtbl and stub func).
- add implementation to each struct implementing it.
```
interface1.h:
...
struct Interface1VTbl_s {
...
    void (*foo)(Interface1 *const self);
...
}

...
void Interface1_foo(Interface1 *const self);
...
```
```
interface1.c:
...
void Interface1_foo(Interface1 *const self) {
    return cobj_Interface1_get_vtbl(self)->foo(self);
}
```

```
mystruct1.h:
// nothing to add, method is private
// keep interface methods private.
// can make methods on the struct public if needed (even those that are called from interfaces).
```
```
mystruct.c:
...
static void MyStruct1_foo(MyStruct1 *const self) {
   // some impl
}
...
void MyStruct1_Interface1_foo(Interface1 *const self) {
    MyStruct1 *const oself = COBJ_INTERFACE_GET_OBJPTR_MUT(MyStruct1, self);
    MyStruct1_foo(oself);
}
...
static Interface1VTbl const Interface1_vtbl = {
...
   MyStruct1_Interface1_foo,
...
}
```

## notes
- a lot of boilerplate per interface - TODO: use macros.
- not in the obj.method syntax - if you want that use C++/rust.
- no object inheritance, uses composition instead.
- no interface inheritance, use composition(? untested) or copying.
- variable access needs to be composed into a getter/setter.
- it's arguable that the destroy function should go in the vtbl as well..
- on SOLID:
  - S: pass: up to how the struct/methods are designed.
  - O: maybe fail: cannot extend once created, only by composition (but then not really extending, neither is inheritance).
  - L: pass: use interfaces, (can subst obj impl interface as needed, not concreted to obj1 type).
  - I: pass: don't need interface? don't request it, methods not needed are just not avail.
  - D: pass: users depend in interfaces not concrete impls.
