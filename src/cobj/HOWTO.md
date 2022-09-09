# How to create a struct based on the 'rules' presented

## Create a header/impl pair for your struct.
- name it to be the same name as the struct.
  So when it's used as '#include <struct.h>' then it's self describing what the include is pulling in.
- Add usual include guards to header, add usual fwd-decls and struct def.

e.g. MyStruct

```
mystruct.h:

#if !defined (MYSTRUCT_H)
#define MYSTRUCT_H

typedef struct MyStruct_s MyStruct;

#include <other.h>

struct MyStruct_s {
}

#endif// defined(MYSTRUCT_H)
```
```
mystruct.c:

#include "mystruct.h"

```

-----

## Add in basic methods
The idea is to use functions to operate on the instance, and not do the quick inline things.
i.e. functions signal the intent.

Wiping is a debug/disgnostics thing to capture use of uninitialised structs and can be
conditionally compiled out for release.

The *_member funcs are for operating on structs when they are member of another struct, the wipe is not called with the expectation that the wipe will be done for the containing struct (i.e. do one wipe instead of lot of little ones).

Avoid the temptation to check for null in these funcs - this should not be needed and besides is a time and codespace sink.

- wipe, move
- destroy, destroy_member,

```
mystruct.h:
...
void MyStruct_destroy_member(MyStruct *const self);
void MyStruct_destroy(MyStruct *const self);
void MyStruct_move(MyStruct *const self, MyStruct *const src);
```
```
mystruct.c:
...

static void MyStruct_wipe(MyStruct *const self) {
    memset(self, 0xa5, sizeof(*self));
}

void MyStruct_destroy(MyStruct *const self) {
    // destroy members that need destroying here using their destroy_member fn.
}

void MyStruct_destroy(MyStruct *const self) {
    MyStruct_destroy_member(self);
    MyStruct_wipe(self);
}

void MyStruct_move(MyStruct *const self, MyStruct *const src) {
    *self = *src;
    MyStruct_wipe(src);
}
```

-----

## Add copy methods as and when needed.
A copy/clone can fail (e.g. malloc fail) and don't want to enforce panicing, so push the 'how to handle' to the caller.

- try_copy, try_copy_member

```
mystruct.h:
...
bool WARN_UNUSED_RESULT MyStruct_try_copy_member(MyStruct *const self MyStruct const *const src);
bool WARN_UNUSED_RESULT MyStruct_try_copy(MyStruct *const self MyStruct const *const src);
```
```
mystruct.c:
...
bool WARN_UNUSED_RESULT MyStruct_try_copy_member(MyStruct *const self MyStruct const *const src, Error *const err) {
    // copy members explicitly here using their try_copy_member function.

    return true; // for a struct the does not need member-wise copying.
}
bool WARN_UNUSED_RESULT MyStruct_try_copy(MyStruct *const self MyStruct const *const src) {
    if (!MyStruct_try_copy_member(self, src, err)) {
        MyStruct_wipe(self);
        return false;
    }
    return true;
}
```

-----

## add creational methods
- default (for creating without args)
- with_* (for creating with where it reads better, or uses same type for diff reasons)
- new (if you cannot think of a better name)

```
mystruct.h:
...
void MyStruct_default(MyStruct *const self);
```
```
mystruct.c:
...
void MyStruct_default(MyStruct *const self) {
    // initialise members to default values here, maybe call member *_default() funcs if applicable.
}
```


```
mystruct.h:
...
void MyStruct_with_foo(MyStruct *const self, int foo);
```
```
mystruct.c:
...
void MyStruct_with_foo(MyStruct *const self, int foo) {
    // initialise self with foo here..
    // e.g. Vector_with_capacity(Vector *const self, size_t capacity);
}
```

other examples:
Given vector, that creates or manages it's backing store on the heap:
- `VectorT_new(VectorT *const self);` // capacity some default initial value, poss 0.
- `VectorT_with_capacity(VectorT *const self, size_t capacity);` // create with capacity specified.
- `VectorT_own(Vector *const self, T *const arr, size_t len);` // own the heap based backing store created elsewhere.

Given vector, does not own/manage it's backing store, that must have a backing store provided to it.
- `VectorT_new(VectorT *const self, T *const arr, size_t len);`
- `VectorT_with_store(VectorT *const self, T *const arr, size_t len);`

A slice: reference to a non-owned/shared array of type T
(note: slices don't own the array, but non-ownwership is not a reason to to use new)
- `SliceT_new(SliceT *const self, T *const arr, size_t len);`

-----
## Add conversional methods
For where a value is being type converted from one type to another.

- If from const then ownership of the value not transferred (so implicit copy).
- If from non-const then ownership of the value is transferred.
- Conversion can fail hence try_*

examples:
- `bool CharString_try_from_cstr(CharString *const self, char const *const str, Error *const err);` // create CharString from cstr.. cstr NOT destroyed
- `bool Path_try_from_CharString(Path *const self, CharString const *const str, Error *const err);` // create Path from CharString.. CharString not destroyed.
- `bool Path_try_from_CharString_mut(Path *const self, CharString *const str, Error *const err);` // create Path from CharString.. CharString destroyed.


Note: as per rust, if you have a `*_try_from_*` method then you can get a `*_try_into_` for free on the other type.
e.g. a `bool Path_try_from_CharString_mut(Path *, CharString *, Error *)` is same as `bool CharString_try_into_Path(CharString *, Path *, Error *)`.
