# Rationale

Using structs to contain and manipulate state makes it easy to code and test.
And I find it easier to think/design in those terms.

# How
But C does not have the OO capabilities of C++, nor the capabilities of Rust.

So, treat structs like C++ objects (or rust structs), and note that there is a
set of basic operations per struct (the compiler provided default set in C++), so
can these be emulated in C.

Obviously this is all going to be manual, the compiler isn't going to help
for the most part, but you will get const/non-const checking.

In My opinion C is, and always will be, a glorified assembler.
It's close enough to assembler without being processor specific,
so more of a processor agnostic assembler.

For C there are 2 aspects to a struct:
1. allocated+uninitialised: the struct is allocated somewhere, be that on the stack,
   on the heap, or in global memory (be that at translation unit level or
   function level.
   structs are allocated uninitialised.
2. initialised: once allocated a struct is uninitialised and needs initialisation
   before it can be 'used'.
   This is done by assignment, by initialiser (assignment), or by a function.

There is no (automatic) destructor of a struct in C.


# basic design constraints

C cannot return structs efficiently.
The workaround is to allocate the storage then pass a pointer as an argument to
a function, the function is then expected to populate the struct (i.e. initialise it).

# basic design patterns

Given a struct, we should be able to move it from uninitialised to initialised
and back to uninitialised.

Sometimes we want to put an existing value into a different allocation.
Rather than copy, use a 'move' for this (it's quicker, and cannot fail).
Unlike the default C assignment which is a bit-copy.
Unlink C++ where the default is to copy.

Sometimes we want to do an actual copy.
This may fail due to resource allocation issues (e.g. member on the heap,
needs a new heap allocation and a bit-copy).

Some operations can fail.
So for those operations they must return a success/fail ind (and marking the
function as WARN_UNUSED_RESULT to force the caller to handle the response,
rather than just silently ignore it).
Optionally, they can 'return' an Error struct as well (a kind of richer
Error reporting mechanism).

As preserving the original value requires extra storage, which may not be
available, then in-place changing is preferred.
If the caller wants to keep the old value around, the caller creates the copy.

Pass struct arguments by pointer, may be type-const or type-non-const,
but mark the argument as value-const - the compiler will detect if you mutate the
pointer value itself and fail to compile, which avoids/prevents hard-to-spot bugs.

As most functions will deal with a struct, call this parameter 'self'.
The other option was 'this' but that seems too c++-like.
Place this argument as the first arg.
(C calling convention does not have a this/self pointer reg specially allocated.
but it fits the pattern of how a C++ compiler would pass it.

Some functions have an Error return, pass this as either second param or last.
(undecided, most likely last).

Some functions call into others and add/remove args.
better to keep the param order and location the same (i.e. harmonise the param lists).
This reduces the code needed to map the args from the called function to the downstream func.
