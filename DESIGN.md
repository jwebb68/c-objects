# Rationale

Using structs to contain and manipulate state makes it easy to code and test.
And I find it easier to think/design in those terms (I think as an OO developer).


# How
In My opinion C is, and always will be, a glorified assembler.
It's close enough to assembler without being processor specific,
so more of a processor agnostic assembler.

But C does not have the OO capabilities of C++, nor the capabilities of Rust.

So, treat structs like C++ objects (or rust structs), and note that there is a
set of basic operations per struct (the compiler provided default set in C++), so
can these be emulated in C.

Obviously this is all going to be manual, the compiler isn't going to help
for the most part, but you will get typesafety and const/non-const checking.
There should be no need to use 'void *'.

For C there are 2 aspects to a struct:

  1. Allocated+Uninitialised: the struct is allocated somewhere, be that on the stack,
     on the heap, or in global memory (be that at translation unit level or
     function level.
     structs are allocated uninitialised.
  2. Initialised: once allocated a struct is uninitialised and needs initialisation
     before it can be 'used'.
     This is done by assignment, by initialiser (assignment), or by a function.

There is no (automatic) destructor of a struct in C.

Also, it is about the value represented, so the value moves from one struct to another
(moves memory location), it my even move to a different type.


# basic design constraints

C cannot return structs efficiently.
It also has no clear RVO process, so assuming the compiler will optimise out
temporaries will lead to lots of copying.
The workaround is to allocate the storage then pass a pointer as an argument to
a function, the function is then expected to populate the struct (i.e. initialise it).

note: If you want a language that does have a defined RVO, use C++ or Rust (there are more..)


# basic design patterns
Use names of functions that signify intent.
Beware use of simple C expressions that are intent implementations.


## no exceptions in C, return Error type for errors, don't re-invent exceptions using setjmp/longjmp
Don't use exceptions for flow control i.e. the try..catch blocks.
We want to know where the error occurred, ideally being able to record what the value was that caused the issue.
To record where it occurred, use a macro capturing file+line and capture these into a struct.

```
// note: no 'success' error, no need for one.
typedef enum ErrorCode_e {
    ErrorCode_Fail,
} ErrorCode;
typedef struct Error_s Error;
//typedef size_t LineNo;
typedef uint16_t LineNo;
struct Error_s {
    // beware packing rules, enums are usually 16bit, size_t can be 16/32/64, but only want 16bit.
    ErrorCode code;
    LineNo lineno;
    char const *file;
};
bool Error_raise_impl(Error *self, ErrorCode code, LineNo lineno; char const *file) {
    self->code = code;
    self->lineno = lineno;
    self->file = file;
    return false;
}
#define Error_raise(self, code) Error_raise_impl(self, code, __LINE__, __FILE__)
```

Now calling Error_raise to initialise an error will capture file and lineno.
If a panic (kernel abort) is needed then can report where that happened, and the
error being handled at the time and where that error originated from.
Which is superior to a processor trap handler.


## Naming convention of functions
C does not have namespaces nor methods, so want mechanism to allow this to prevent clashes.
A workable one is to join the namespace and struct name and 'method' name into one separated by underscores.
To have namespace in lowercase,
to have struct names in Caps Case/Camel Case (but not with lower case on first word, but upper case on first letter of each word),
to have method names in 'snake' case.
e.g. `<namespace>_<StructName>_method_name`.
Here the struct is obvious - it's the one that starts with a capital letter after a _.

ones that don't work:
- camel casing the whole thing - you loose what name is the type.
e.g. nameSpaceStructNameMethodName.
- putting method name first and then camelCasing it.


## Get the compiler to spot your bugs
1. Make your pointer args value-const, so if you 'accidentally' forget to deref in a ++,
then compiler will fail the compile rather than not and cause a hard to find bug.
i.e. `++a` vs `++*a`.


## Initialise/Deinitialise/wipe
Given a struct, we should be able to move it from uninitialised to initialised
and back to uninitialised.
i.e.

```
/// initialise self for use.
/// UB if self: already initialised, is NULL, not writable, not readable.
void StructT_new(StructT *const self);

/// deinitialise self for use, free up any held internal resources etc.
/// UB if self: not initialised, is NULL, not writable, not readable.
void StructT_destroy(StructT *const self);
```
note: Avoid temptation to do a null check in the new/destroy - this only slows things down.
      and it may already have been checked beforehand.

TODO: a destroy that calls member destroys will call wipe on each member, when at the end of the struct a wipe is going to be done anyway.
pointless calling wipe for each member in that case.
Should destroy have a flag saying if to wipe or not, or should that be a sep function..
Sep function for speed..

```
void StructT_destroy(StructT *const self); // calls wipe
void StructT_destroy_member(StructT *const self); // does not call wipe
```

## wipe
Also, we want to wipe the struct without calling any member destructors (e.g. moving, after destroying etc).
This should be a private func - no need for anything outside of type to call it.
This is to catch use of 'uninitialised values e.g. debug, so fill with debug pattern'.
You may want to wrap in a conditional if DEBUG and use empty impl if in release.
i.e.:

```
/// UB if self: not initialised, is NULL, not writable, not readable.
static void StructT_wipe(Struct *const self) {
  memset(self, 0xa5, sizeof(*self));
}
```

Avoid temptation to macro this for each struct, i.e.

```
define StructT_wipe(self) do{memset(self, 0xa5, sizeof(*self))} while(0)
```
however the above as  a generic helper macro is ok.

```
#if defined(DEBUG)
#define STRUCT_WIPE(p) do{memset(p, 0xa5, sizeof(*p);}while(0)
#else
#define STRUCT_WIPE(p)
#endif//defined(DEBUG)

```
note: Avoid temptation to do a null check in the wipe - this only slows things down.
      and it may already have been checked beforehand.


## move.
Sometimes we want to put an existing value into a different allocation.
Rather than copy, use a 'move' for this (it's quicker, and cannot fail).
Unlike the default C assignment which is a bit-copy.
Unlike C++ where the default is to call the copy-ctor/copy-assign op.
(and yes, it's implemented using the C bit-copy/assign, but it's not the same intent as).
i.e.:

```
/// UB if self: already initialised, is NULL, not writable, not-readable.
/// UB if src: not initialised, is NULL, not writable, not-readable.
void StructT_move(StructT *const self, StructT *const src) {
  *self=*src;
  StructT_wipe(src);
}
// call when moving a member of a struct, save calling wipe multiple times.
void StructT_move_member(StructT *const self, StructT *const src) {
  *self=*src;
}
```


## copy.
Sometimes we want to do an actual copy.
This may fail due to resource allocation issues (e.g. member on the heap,
needs a new heap allocation and a bit-copy).
So, this then needs to return an error on fail and value on success.
And it's also explicit, i.e. no operation must make a copy unless it's changing type.
And as a resource allocation can fail then couple with the 'functions that can fail' pattern.

```
/// UB if self: already initialised, is NULL, not writable, not-readable.
/// UB if src: not initialised, is NULL, not writable, not-readable.
/// on fail, self is left uninitialised.

// if no resources managed, degrades into a bit-copy
bool Struct_try_copy(StructT *const self, StructT const *const src, Error *const err) {
  *self = *src;
  return true;
}

// or:
// if external resources managed, e.g. malloc block.
bool Struct_try_copy(StructT *const self, StructT const *const src, Error *const err) {
  *self = *src;
  {
    OtherStruct1 *const p = malloc(sizeof(self->member1));
    if (p == NULL) {
        Error_new(err, EMALLOC);
        goto fail;
    }
    OtherStruct1_copy(p, self->member1);
    self->member1 = p; // not assigned until initialised.
  }
  {
    OtherStruct2 *const p = malloc(sizeof(self->member2));
    if (p == NULL) {
        Error_new(err, EMALLOC);
        goto fail_member1;
    }
    OtherStruct2_copy(p, self->member2);
    self->member2 = p; // not assigned until initialised.
  }
  // other copying here.
  ...
  return true;

  // cleanup handlers, in reverse order of copy/create
fail_member2:
  OtherStruct1_destroy(self->member1);
  // fail through to next cleanup
fail:
  StructT_wipe(self);
  return false;
}

// or: if using BoxedStruct as inlined member:
bool Struct_try_copy(StructT *const self, StructT const *const src, Error *const err) {
  *self = *src;
  if (!BoxedOtherStruct1_try_copy(&self->member1, &self->member1, err)) { goto fail; }
  if (!BoxedOtherStruct2_try_copy(&self->member2, &self->member2, err)) { goto fail_member1; }
  if (!BoxedOtherStruct3_try_copy(&self->member2, &self->member3, err)) { goto fail_member2; }
  // other reallocations here.
  return true;

  // cleanup handlers, in reverse order of copy/create
  // so can fall through in reverse order of create.
  // not using StructT destroy as self is only partially initialised not fully.
fail_member2:
  BoxedOtherStruct2_destroy_member(self->member2);
fail_member1:
  BoxedOtherStruct1_destroy_member(self->member1);
fail:
  StructT_wipe(self);
  return false;
}
```

## ops that fail.
Some operations can fail.
So for those operations they must return a success/fail ind (and marking the
function as WARN_UNUSED_RESULT to force the caller to handle the response,
rather than just silently ignore it).
Optionally, they can 'return' an Error struct as well (a kind of richer
Error reporting mechanism).

As preserving the original value requires extra storage, which may not be
available, then in-place changing is preferred.
If the caller wants to keep the old value around, the caller creates the copy,
i.e. caller should be in explicit control when a copy is made.


## passing values to functions.
Pass struct arguments by pointer, may be type-const or type-non-const,
but mark the argument as value-const - the compiler will detect if you mutate the
pointer value itself and fail to compile, which avoids/prevents hard-to-spot bugs.


## passing value to be operated on by the function.
As most functions will deal with a struct, call this parameter 'self'.
The other option was 'this' but that seems too C++-like.
Place this argument as the first arg.
The C calling convention does not have a this/self pointer reg specially allocated.
but it fits the pattern of how a C++ compiler would pass it.

Some functions have an Error return, pass this as either second param or last, possibly as pointer to an error struct.
(undecided, most likely last).


## function call chaining/tail calling.
Some functions call into others and add/remove args.
Better to keep the param order and location the same (i.e. harmonise the param lists).
This reduces the remapping code needed to map the args from the called function to the downstream func.


## 'Returning' values from functions.
Returns of struct types must be via outptr if that struct is larger than 1 reg.


## 'Returning' errors from functions.
It is simpler to return a bool as an indicator of success/fail and have a separate struct filled in as needed.
E.g.

```
bool func(..., Error *const err, ...);
return true, Error *err was not initialised.
return false, Error *err was initialised and needs destroying if handled (it should be initialised in-place for callers).
```

## malloc/free
Simpler solution is to use a 'Boxed' (heap allocated) struct  (easier with templates, which C does not have...)
Either write one when needed - maybe put into same header as struct.


## mutable/const
For a type wrapping a value that could be used in a const/mutable way (e.g. a BoxedStruct),
then provide functions that act on the mutable version  as *_mut.
e.g. For BoxedStructT

```
typedef struct BoxedStructT_s BoxedStructT;
struct BoxedStructT_s {
    StructT *v;
};
...
Struct const *BoxedStruct_deref(BoxedStructT const *const self) {
    return self->v;
}
Struct *BoxedStruct_deref_mut(BoxedStructT *const self) {
    return self->v;
}
```
so now you can access the boxed value and get compiler to fail if you get the const/non-const designation wrong.


## arrays
Potentially treat as separate type, e.g. ArrayStruct, with own new/destroy/boxed/ etc functions.
or, if already using C arrays (as ptr+len), then extract into CArray funcs taking ptr+len and impl ArrayStruct with them.
e.g.

```
CArrayStructT_destroy(StructT *const arr, size_t len) {
    for (size_t i=0; i < len; ++i) {
        StructT_destroy(&arr[i]);
    }
}

typedef struct ArrayStructT_s ArrayStructT;
struct ArrayStructT_s {
    StructT *arr;
    size_l len;
}
ArrayStructT_destroy(ArrayStructT *const self) {
    CArrayStructT_destroy(self->arr, self->len);
    ArrayStructT_wipe(self);
}
```

or:

```
CArrayStructT_destroy(StructT *const arr, size_t len) {
    for (StructT *it=arr, *const e=arr+len; e != it; ++it) {
        StructT_destroy(it);
    }
}
```


## slices
For when iteration over a sequence of struct is required but in a non-owning manner, so when
a slice is destroyed it does not destroy the items it references.
Again, separate file or in same file (separate is neater).
