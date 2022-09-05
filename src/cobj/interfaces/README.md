Introduction
=============
an experiment/research into implementing interfaces in C
and possible reasons to not do it like this.

what I want:

```
    interface interface1 {
        get_value();
        set_value(value);
        //
        query_interface(interfaceid) -> interface_ptr;
    }

    interface interface1 {
        get_value();
        set_value(value);
        get_objects();
        set_objects(value);
        //
        query_interface(interfaceid) -> interface_ptr;
    }

    object implements interface1, interface2 {

    }


    {
    object o = object::new();
    interface1 i1 = (interface1)o.query_interface(interface1::id)
    v = i1.get_value()
    }
```

note, in C there's no object inheritance, use composition + interfaces instead(?)

various ways:
1. function pointers in ram struct.
2. function pointers in vtable, vtable ptr in ram image


observations
------------

"one interface, function pointers in ram struct."
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

```
typedef struct MyStruct_s MyStruct;
struct MyStruct_s {
    void (const *foo)(MyStruct const *const self);
    void (*foo_mut)(MyStruct *const self);
};
void MyStruct_foo(MyStruct const *const self)
{
}
void MyStruct_foo_mut(MyStruct *const self)
{
}
void MyStruct_destroy(MyStruct *const self)
{
}
void MyStruct_new(MyStruct *const self)
{
    self->foo = MyStruct_foo;
    self->foo_mut = MyStruct_foo_mut;
    // or, but beware of other member initialisations in init-list
    //MyStruct inst = {.foo=MyStruct_foo};
    //*self = inst;
}
int main()
{
    MyStruct s;
    MyStruct_new(&s);

    MyStruct_foo(&s);
    //or
    s.foo(&s);

    MyStruct_destroy(&s);
}
```

 - larger ram footprint, per struct
 - copy of pointers in each struct i.e. ram bloat
 - fast, one index load + jump per func call.
 - cannot check for corruption of ram struct easily.
 - no interface inheritance
 - does not use offsetof (misra)
 - would look like struct.method notation to call
 - only 1 'interface'

-----------------------

"one interface, function pointers in ram struct, in own struct."
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

```
typedef MyIFace_s MyIface;
struct MyIface_s {
    void (const *foo)(MyStruct const *const self);
    void (*foo_mut)(MyStruct *const self);
};
typedef struct MyStruct_s MyStruct;
struct MyStruct_s {
    MyIface myiface;
};
void MyStruct_foo(MyStruct const *const self)
{
}
void MyStruct_foo_mut(MyStruct *const self)
{
}
void MyStruct_destroy(MyStruct *const self)
{
}

MyIface const myiface_init = {
    MyStruct_foo,
    MyStruct_foo_mut,
};

void MyIface_new(MyIface *self)
{
    self->foo = MyStruct_foo;
    self->foo_mut = MyStruct_foo_mut;
    // or
    *self = myiface_init; // beware, will copy padding.
}

void MyStruct_new(MyStruct *const self)
{
    MyIface_new(&self->myiface);
}
int main()
{
    MyStruct s;
    MyStruct_new(&s);
    MyStruct_foo(&s);
    //or
    s.foo(&s);
    MyStruct_destroy(&s);
}
```

 - copy of pointers in each struct i.e. ram bloat
 - fast, one index load + jump per func call.
 - cannot check for corruption of ram struct easily.
 - no interface inheritance
 - does not use offsetof (misra happy)
 - would look like struct.method notation to call
 - only 1 'interface'

-----------------------

"segregated interfaces, function pointers in vtable per interface in const mem."

```
typedef struct InterfaceID_s InterfaceID;
// interface.h ?
// md5/uuid etc.
struct InterfaceID_s {
    uint8_t value[8];
};

// myiface1.h
typedef struct MyIface1_s MyIface1;

// myiface1.h
typedef struct MyIface1Vtbl_s MyIface1Vtbl;
struct MyIface1Vtbl_s {
    struct MyIface1Vtbl_s const *const token;
    ptrdiff_t adj;
    void (*destroy)(MyIface1 *const self);
    int (*query_interface)(MyIface1 const *const self, InterfaceID const *const iid, void const **out);
    int (*query_interface_mut)(MyIface1 *const self, InterfaceID const *const iid, void **out);
    int (*foo)(MyIface1 const *const self);
    int (*foo_mut)(MyIface1 *const self);
};

//myiface1.h
struct MyIface1_s {
    // vtbl must remain const even if struct is not.
    MyIface1Vtbl const *const _vtbl;
};

//myiface1.h
extern InterfaceID const MyIface1_IID;

#define MyIface1_init(vtbl) { ._vtbl=vtbl }

//myiface1.c
static void MyIface1_check(MyIface1 const *const self)
{
    // Not good, want to return error, not assert, so call-site decides what to do.
    assert(self->_vtbl.token == self->_vtbl);
}

//myiface1.c
InterfaceID const MyIface1_IID = {
    //TDB
};

//myiface1.h+myiface1.c
void MyIface1_destroy(MyIface1 const *const self)
{
    MyIface1_check(self);
    return self->_vtbl.destroy(self);
}

int MyIface1_query_interface(MyIface1 const *const self, InterfaceID const *const iid, void const **out)
{
    MyIface1_check(self);
    return self->_vtbl.query_interface(self, iid, out);
}

//myiface1.h+myiface1.c
int MyIface1_query_interface_mut(MyIface1 *const self, InterfaceID const *const iid, void **out)
{
    MyIface1_check(self);
    return self->_vtbl.query_interface_mut(self, iid, out);
}

//myiface1.h+myiface1.c
int MyIface1_foo(MyIface1 const *const self)
{
    // need to check _vtbl is correct here
    MyIface1_check(self);
    return self->_vtbl.foo(self);
}

//myiface1.h+myiface1.c
int MyIface1_foo_mut(MyIface1 *const self)
{
    // need to check _vtbl is correct here
    MyIface1_check(self);
    return self->_vtbl.foo_mut(self);
}

//myiface1.h+myiface1.c
// new? poss not.
void MyIface1_new(MyIface1 *const self, MyIface1Vtbl const *const vtbl)
{
    self->_vtbl = vtbl;
}
//========================================================================

typedef struct MyStruct_s MyStruct;
struct MyStruct_s {
    MyIface1 myiface1;
};

int MyStruct_query_interface(MyStruct const *const self, InterfaceID const *const iid, void const **out)
{
    int ret;
    if (InterfaceID_is_eq(iid, &myiface1)) {
        out = &myiface1;
        ret = 0;
    } else {
        ret = -1;
    }
    return ret;
}
int MyStruct_query_interface_mut(MyStruct *const self, InterfaceID const *const iid, void **out)
{
    int ret;
    if (InterfaceID_is_eq(iid, &myiface1)) {
        out = &myiface1;
        ret = 0;
    } else {
        ret = -1;
    }
    return ret;
}

void MyStruct_foo(MyStruct const *const self)
{
}

void MyStruct_foo_mut(MyStruct *const self)
{
}
//===============
static MyStruct const *MyStruct_MyIface_get_self(MyIface1 const *const self)
{
    ptrdiff_t adj = self->_vtbl.adj;
    MyStruct const *const obj = (MyStruct const *)((uint8_t const *const)self - adj);
    return obj;
}
static MyStruct *MyStruct_MyIface1_get_self_mut(MyIface1 *const self)
{
    ptrdiff_t adj = self->_vtbl.adj;
    MyStruct *const obj = (MyStruct *)((uint8_t *const)self - adj);
    return obj;
}

void MyStruct_MyIface_destroy(MyIface1 const *const self)
{
    MyStruct const *inst = MyStruct_MyIface1_get_self(self)
    MyStruct_destroy(inst);
}
int MyStruct_MyIface_query_interface(MyIface1 const *const self, InterfaceID const *const iid, void const **out)
{
    MyStruct const *inst = MyStruct_MyIface1_get_self(self)
    return MyStruct_query_interface(inst, iid, out);
}
int MyStruct_MyIface_query_interface_mut(MyIface1 *const self, InterfaceID const *const iid, void **out)
{
    MyStruct *inst = MyStruct_MyIface1_get_self_mut(self)
    return MyStruct_query_interface_mut(inst, iid, out);
}

void MyStruct_MyIface_foo(MyIface1 const *const self)
{
    MyStruct const *inst = MyStruct_MyIface1_get_self(self)
    MyStruct_foo(inst);
}

void MyStruct_MyIface_foo_mut(MyIface1 *const self)
{
    MyStruct *inst = MyStruct_MyIface_get_self_mut(self)
    MyStruct_foo_mut(inst);
}

static MyIfaceVtbl const MyStruct_MyIface_vtbl = {
    .token = &MyStruct_MyIface1_vtbl,
    .adj = offsetof(MyStruct, myiface1),
    .destroy = MyStruct_MyIface1_destroy,
    .query_interface = MyStruct_MyIface1_query_interface,
    .query_interface_mut = MyStruct_MyIface1_query_interface_mut,
    // == interface specific functions here.
    .foo = MyStruct_MyIface1_foo,
    .foo_mut = MyStruct_MyIface1_foo_mut,
};

void MyStruct_destroy(MyStruct *const self)
{
    // interfaces do not need destroying as they have no state.
}
void MyStruct_new(MyStruct *const self)
{
    MyIface1_new(&self->myiface1, &MyStruct_MyIface1_vtbl);
}
#define MyStruct_init \
{ \
    .myiface1=MyIface1_init(&MyStruct_MyIface1_vtbl) \
}

int main()
{
    MyStruct s;
    MyStruct_new(&s);

    int rc;
    MyIface const *si;
    rc = MyStruct_query_interface(&s, &MyIface_IID, &si);
    assert(rc == 0);
    // si has lifetime same or less than object it was asked of.

    MyIface_foo(si);

    MyStruct_destroy(&s);
    // both s and si now invalid.
}
```

 - one ptr (vtbl ptr) per interface per struct.
 - no dupe of func pointers, they are in const ram.
 - not so fast, 2 index loads + jump + 2 other index loads per func call.
 - can check for corruption (e.g. vtable must begin with magic number)
 - no interface inheritance
 - would use offsetof (misra sad)
 - best called via functions, i.e not using struct.method syntax
 - can have multiple 'interfaces' but each would have own vtbl ptr in struct.
 - one fuck of a lot of effort/typing..
 -

----------------

"multiple interfaces, vtbl in const space, but not in ram image".

- how to get vtbl for obj? (compiler knows somehow?)
- fat-ptr idea ('ptr' is ptr to ram state + ptr to vtbl), where are these stored?


* is there a way of segregating the interfaces into one vtbl, a vbtl of vtbls?
* - slower, 3 index loads + jump per call.
* fat ptr, i.e ptr to iface+vtbl

```
struct Iface1Ref {
    struct Iface1 *inst;
    struct Iface1Vtbl *vtbl;
};
```
?
// note: pass by value, not by ref.
// uses 2 regs though:
// uses 2 mem loads + 1 mem loads = 3 memloads [safer]
// or: 1 memload + 1 memload = 2 memloads [unsafe]

```
bool Iface1_foo(Iface1Ref self, ...) {
    // one index/mem load
    if (self.vtbl != self.vtbl->token) { return false; }
    // another index/mem load
    return self.vtbl->foo(self,...)
}
// with self.vtbl->foo pointing to:
bool Object1_Iface1_foo(IfaceRef self, ...) {
    // do I need to do it twice ?
    // if (self.vtbl != self.vtbl->token) { return false; }
    // another index/mem load
    Object1 *inst = (Object1 *)(((uint8_t *)self.inst) + self.vtbl->adj);
    return Object1_foo(inst, ...);
}

//
// vs.
// normal ptr+vtbl in struct.
// uses 1 reg
// uses 3 memload + 2 memload = 5 memloads [safer]
// or: uses 2 memloads + 2 memload = 4 memloads [unsafe]
bool Iface1_foo(Iface1 *self, ...) {
    // one index/mem load
    Iface1Vtbl *vtbl = self->vtbl;
    // one index/mem load
    if (vtbl != vtbl->token) { return false; }
    // another index/mem load
    return vtbl->foo(self, ...)
}
// with self.vtbl->foo pointing to:
bool Object1_Iface1_foo(Iface *self, ...) {
    // one index/mem load
    Iface1Vtbl *vtbl = self->vtbl;
    // do I need to do it twice ?
    // if (vtbl != vtbl->token) { return false; }
    // another index/mem load
    Object1 *inst = (Object1 *)(((uint8_t *)self) + vtbl->adj);
    return Object1_foo(inst, ...);
}
```
---------------------

"combined interfaces, function pointers in vtable per interface in const mem."
 - one ptr (vtbl ptr) for all interfaces per struct.
 - combine all vtbls into one big one..
 - no dupe of func pointers, they are in const vtbl, same for all struct instances.
 - (this is how c++ does it).
 - 2 index lookups and jump

// ah, rats, lose type safety at interface level calls.
iface ptr from query_interface returns what ptr?
ptr in vtable? then it loses method to get back to object.
object? then it plays fast/loose to assume object layouts are transposable.

------------------------

Obviously in C I'm not going to get the obj.method syntax unless I put func
pointers into a struct that exists in ram (i.e solution1) , which I don't want.

So, method calls would devolve into simple thunkers/wrappers to encapsulate how
it's implemented.
Also, want to return a result/error code to check on, which forces use of
out-values, unless a Result struct is used..
(poss nicer, but more typing/maintenance of a separate struct and not idiomatic C)

```
 {
    // object o = object::new();
    Object o = Object_new();

    // interface1 i1 = (interface1)o.query_interface(interface1::id)
    Interface1 *iface1;
    Result res;
    res = Object_query_interface(&o, &Interface1_IID, (void **)&iface1);
    if (Result_is_error(res)) {
     ... do error handling here..
    }

    // v = i1.get_value()
    Type v;
    res = Interace1_get_value(iface1, &v);
    if (Result_is_error(res)) {
     ... do error handling here..
    }
 }
```

Issues
------
The issue is the redirection through the vtbl for each interface
The object inst lives in ram.
The vtbl pointer therefore is in ram since it is in the object struct and so can be corrupted.
which would cause bad stuff to happen.
This is mitigated by using a guard in the vtbl, if the guard does not match expected - fail.
maybe this should be panic..
But even then, malicious actors could match the vbtl structure and fool this.

adding an interface is a very labourious affair,
needing a fair amount of copy/paste (mainly due to type preservation of interface)
however, some of this can be mitigated with helper funcs.

adding a method to an interface is also painful.
for each method added, it requires:
- 3+1 additions in the interface defn
- 3 additions to each class implementing
- most are cut/paste due to type preservation.
- some possible use of helper funcs.

The interface ptrs from query_interface are returned via void **, which is nasty.
as such, the interface ptrs lose const-ness.
typeness is still needed to call functions, so you're forced to preserve that aspect of it.
Even using a Result struct would still leave you with a void *, requiring casting.
(Oh for templates/generics and link-time optimisation (code deduping)).
