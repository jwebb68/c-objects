/**
 * an example of classes/struct in C and their usage
 * based on patterns emerging, Rust and C++
 */

// hacky defs so I don't need to include things
// prob not right defn
// to be replaced with correct if ever used.
#define NULL ((void *)0)
typedef unsigned long size_t;
typedef unsigned char bool;
static const bool false = 0;
static const bool true = !false;
void ASSERT(bool val) {}
void FREE(void *p) {}
void *MALLOC(size_t n) {}
void *REALLOC(void *p, size_t n) {}

// our example class/struct, all should follow the patterns here
typedef struct _Foo Foo;
struct _Foo {
   int dummy;
};

// the default initialiser list as a macro
// so it can be used to initialise members in other structs (since
// extern consts cannot in C).
// also, so it can be used where inst is in global/static ram/rom
#define Foo_INIT (Foo){0}
// in .h
//extern Foo const Foo_init;
// in .c
Foo const Foo_init = Foo_INIT;

// the destructor,
// before: *self is initialised
// after: *self is destroyed/uninitialised
// UB is called on uninitialised/destroyed
void Foo_destroy(Foo const self) {
}

// the default ctor
// optional
// 'required' if Foo_free implemented (_free uses it)
// creates the struct using defaults.
// using by-value so I don't have to handle any existing state
// since there isn't any
Foo Foo_default(void) {
    Foo inst = Foo_init;
    return inst;
}

// non-default ctor
// optional
// may be many of them.. differentiator is in the funcname afer new
// e.g. Foo Foo_new_by_eck(...)
// creates the struct using supplied args
Foo Foo_new(...) { ??? }

// perform a deep copy of Foo..
// not using ptrs so can be used in expressions/function call chaining.
// cannot prevent C from shallow/bit copying..
// can only implement clone if all members and members of members etc
// implement clone (and can be cloned without failing).
Foo Foo_clone(Foo const self) {
    // for simple structs, when bitcopying works..
    Foo inst = self;
    // for complex members (i.e. those with own clone function)
    //, call clone on complex members...
    return inst;
}

// try_clone
// optional
// clone from other to this, with possbility that op can fail.
// will need to handle existing state in self..
// and if fail then cleanup/call destroy on members already cloned before exit..
// self is uninitialised/destroyed before
// self is initialised after on success
// self is destroyed on fail
Err Foo_try_clone(Foo const self, Foo *const other) {
    ASSERT(self != NULL);
    // for simple structs, when bitcopying works.. (prob don't need try_clone)
    *other = self;
    // for complex members (i.e. those with own clone/try_clone a function)
    // but must clean up partially cloned struct manually on fail.
    // and not to leave it to caller..
    return ??;
}


// move contents of src into dest
// before: dest is uninitialised
// after: dest holds contents of self
// after: self is now invalid/uninitialised
// UB is self is used after move
// c.f. use-after-free issues.
Foo Foo_move(Foo const self) {
    // no point wiping self as it's a local
    return self;
}

// Foo_malloc
// optional
// moves value into heap, and not a copy of.
// where val came from is now invalid (not enforceable in C)
// val not const so to prevent const values being moved.
Foo *Foo_malloc(Foo val) {
    Foo *const inst = MALLOC(sizeof(val));
    if (inst != NULL) {
        *inst = Foo_move(val);
    }
    return inst;
}
// Foo_free
// required if Foo_malloc implemented
// destroys struct by calling Foo_destroy then releases heap
void Foo_free(Foo *const self) {
    if (self != NULL) {
        Foo_destroy(*self);
    }
    FREE(self);
}


/*
class Foo {
    public:
        int dummy;
    public:
        // automatic: dtor
        // Foo_destroy(Foo *const self);
        //void ~Foo(void);

        // automatic: default ctor
        // Foo_default
        //Foo();

        // Foo Foo_new(...)
        Foo(...)

        // automatic: copy/clone
        // Foo_clone(Foo const other);
        //Foo(const Foo &other);

        // extra: move
        // Foo(Foo &&other);
};
// what we get for free in C++
// destroy (~Foo) - dtor
// default (Foo()) - default ctor
// clone (Foo(const Foo &other) - copy ctor
// malloc (Foo::new())
// free (Foo::delete())
// array_malloc (Foo::new[](std::size_t size);
// array_free (Foo::delete[](void *ptr);
// automatic calling of clone.
// automatic calling of array_clone.
// automatic calling of destroy
// automatic calling of array_destroy
*/

// now what does Foo methods look like if we pass ptrs and not values..
// destroy looks same
// optional
// if implemented then must handle case when self is NULL so callers don't have to.
void pFoo_destroy(Foo *const self) {
}
//default (and all ctors) now return ptr to foo to use for call chaining.
// assumes self is uninitialised.
// UB if self is not uninitialised.
// ip=inplace
//Foo *Foo_ipnew_default(Foo *const self) {
Foo *Foo_ipdefault(Foo *const self) {
    ASSERT(self != NULL);
    *self = Foo_init;
    return self;
}
// specific ctors
// optional, may be many
// assumes self is uninitialised.
// UB if self is not uninitialised.
Foo *Foo_ipnew(Foo *const self, ...) {
    return self;
}
// cloning
// self is uninitialised
// src is initialised.
Foo *Foo_ipclone(const Foo *const self, Foo *const dest){
    pFoo_try_clone(self, dest);//??
    return dest;
}
// if clone can fail, implement try_clone_from
// before: self: uninitialised/destroyed
// after[success]: self: initialised
// after[fail]: self: uninitialised/destroyed
Error pFoo_try_ipclone(const Foo *const self, Foo *const dest){
    ASSERT(self != NULL);
    ASSERT(src != NULL);
    *dest = *self;
    // now will need to handle any existing content in self
    // could say existing is assumed to be crap so can be blatted over.
    // so no reusing on Foo instances alreadt initialised.
    return ??;
}

// use after free cannot be checked/guarded for unless self is passed
// by ptr so func can amend to NULL.
void pFoo_free(Foo *const self) {
    if (self != NULL) {
        pFoo_destroy(self);
    }
    FREE(self);
}
// moves src into dest
// before: dest is uninitialised
// after: dest has self contents
// after: self is invalid and must not be used
// UB is self is used
// c.f. use-after-free issues.
Foo *Foo_ipmove(Foo *const self, Foo *const dest) {
    ASSERT(src != NULL);
    ASSERT(dest != NULL);
    *dest = *self;
    // could wipe src here...
    return dest;
}

// val is moved into heap allocation, so simple bitcopy suffices.
// don't want copy as copy/clone is potentially expensive.
// if you want a copy, use (try_)clone_info
// use of val after call is not checked for.
Foo *pFoo_malloc(const Foo *const val) {
    if (val == NULL) return val;
    Foo *const inst = MALLOC(sizeof(*val));
    if (inst != NULL) {
        pFoo_move(val, inst);
    }
    return inst;
}
// usual C way is to malloc memory, then do new/clone
//Foo *Foo_malloc(void) {
//    return MALLOCZ(sizeof(Foo))
//}
// but then Foo * is uninitialised so cannot be freed
// this then leads to manually adding destroy(or not) before every _free


// for when arr+size are sep members of structs
void Foo_array_destroy(Foo *const arr, size_t len) {
    ASSERT(len == 0 || arr != NULL);
    for (size_t idx=0; idx< len; ++idx) {
        Foo_destroy(arr[idx]);
    }
}
void pFoo_array_destroy(Foo *const arr, size_t len) {
    ASSERT(len == 0 || arr != NULL);
    for (size_t idx=0; idx< len; ++idx) {
        pFoo_destroy(&arr[idx]);
    }
}

// Foo *Foo_array_default(?) {
//     ??
// }
Foo *pFoo_array_default(Foo *const arr, size_t len) {
    ASSERT(len == 0 || arr != NULL);
    for (size_t idx=0; idx < len; ++idx) {
        pFoo_default(&arr[idx]);
    }
    return arr;
}

// Foo *Foo_array_new(?) {
//     ??
// }
// Foo *pFoo_array_new(Foo *const arr, size_t len, ?) {
//     for (size_t idx=0; idx < len; ++idx) {
//         pFoo_new(&arr[idx], ??);
//     }
// }

Foo *Foo_array_clone(const Foo *const arr, size_t len, const Foo *const dest, size_t const len_dest) {
    ASSERT(len == len_dest);
    ASSERT(len_dest == 0 || dest != NULL);
    ASSERT(len == 0 || arr != NULL);
    for (size_t idx=0; idx < len; ++idx) {
        arr[idx] = Foo_clone(dest[idx]);
    }
    return arr;
}
Foo *pFoo_array_clone(const Foo *const arr, size_t len, const Foo *const dest, size_t const len_dest) {
    ASSERT(len == len_dest);
    ASSERT(len_dest == 0 || dest != NULL);
    ASSERT(len == 0 || arr != NULL);
    for (size_t idx=0; idx < len; ++idx) {
        pFoo_clone(&arr[idx], &dest[idx]);
    }
    return arr;
}

void Foo_array_free(Foo *const arr, size_t len) {
    if (arr != NULL) {
        Foo_array_destroy(arr, len);
    }
    FREE(arr);
}
void pFoo_array_free(Foo *const arr, size_t len) {
    if (arr != NULL) {
        pFoo_array_destroy(arr, len);
    }
    FREE(arr);
}
// puts arr into heap, not a copy of
Foo *pFoo_array_move(Foo *const src, Foo *const dest, size_t const len) {
    ASSERT(src != NULL);
    ASSERT(dest != NULL);
    //memcpy(dest, src, sizeof(*dest)*len);
    for (size_t idx=0;idx<len; ++idx) {
        pFoo_move(&src[idx], &dest[idx]);
    }
    return dest;
}
Foo *Foo_array_malloc(Foo *const arr, size_t len) {
    if (arr == NULL || len == 0) return NULL;
    Foo *const inst = MALLOCZ(sizeof(*arr)*len);
    if (inst != NULL) {
        pFoo_array_move(arr, inst, len);
    }
    return inst;
}


// as inline/variable sized struct
// fixed size array of foo
// all are allocated/created
// all are destroyed
// but fixed size means that any change in size needs recoding..
typedef struct _FooArray FooArray;
struct _FooArray {
    Foo arr[10];
};
// non-ptr variant
void FooArray_destroy(FooArray const self) {
    for (size_t idx=0; idx< CARRAY_COUNT(self.arr); ++idx) {
        Foo_destroy(self.arr[idx]);
    }
}
FooArray FooArray_default(void) {
    FooArray inst;
    for (size_t idx=0;idx<CARRAY_COUNT(inst.arr);++idx) {
        inst.arr[idx] = Foo_default();
    }
    return inst;
}
FooArray FooArray_clone(FooArray const self) {
    FooArray inst;
    for (size_t idx=0; idx<CARRAY_COUNT(inst.arr);++idx) {
        inst.arr[idx] = Foo_clone(self.arr[idx]);
    }
    return inst;
}
void FooArray_free(FooArray *const self) {
    if (self != NULL) {
        FooArray_destroy(self);
    }
    FREE(self);
}
FooArray FooArray_move(FooArray const self) {
    FooArray other = self;
    // wipe self here?? no point - it's local.
    return other;
}
FooArray *FooArray_malloc(FooArray val) {
    FooArray *const inst = MALLOC(sizeof(val));
    if (inst != NULL) {
        *inst = FooArray_move(val);
    }
    return inst;
}
//ptr variant
void pFooArray_destroy(FooArray *const self) {
    ASSERT(self != NULL);
    for (size_t idx=0; idx< CARRAY_COUNT(self->arr); ++idx) {
        pFoo_destroy(&self->arr[idx]);
    }
}
FooArray *pFooArray_default(FooArray *const self) {
    ASSERT(self != NULL);
    for (size_t idx=0;idx<CARRAY_COUNT(self->arr);++idx) {
        pFoo_ipnew_default(&self->arr[idx]);
    }
    return self;
}
FooArray *pFooArray_clone(FooArray *const self, FooArray *const dest) {
    ASSERT(dest != NULL);
    ASSERT(self != NULL);
    for (size_t idx=0; idx<CARRAY_COUNT(src->arr);++idx) {
        pFoo_clone(&self->arr[idx], &dest->arr[idx]);
    }
    return dest;
}
void pFooArray_free(FooArray *const self) {
    if (self != NULL) {
        pFooArray_destroy(self);
    }
    FREE(self);
}
FooArray *pFooArray_move(FooArray *const self, FooArray *const dest) {
    ASSERT(self != NULL);
    ASSERT(dest != NULL);
    *dest = *self;
    // could wipe self here..
    return dest;
}
FooArray *pFooArray_malloc(FooArray *const val) {
    if (val == NULL) return val;
    FooArray *const inst = MALLOC(sizeof(*val));
    if (inst != NULL) {
        pFooArray_move(val, inst);
    }
    return inst;
}

// Foo array, where array is allocated outside of struct - ?
// FooArrayRef?
typedef struct _FooArrayRef FooArrayRef;
struct _FooArrayRef {
    Foo *arr;
    size_t len;
};
void FooArrayRef_destroy(FooArrayRef const self) {
    ASSERT(self.len ==0 || self.arr != NULL);
    for (size_t idx=0; idx < self.len; ++idx) {
        Foo_destroy(self.arr[idx]);
    }
}
FooArrayRef FooArrayRef_new(Foo *const arr, size_t len) {
    FooArrayRef inst = {.arr=arr, .len=len};
    return inst;
}
FooArrayRef FooArrayRef_clone_into(FooArrayRef const self, FooArrayRef *const dest) {
    ASSERT(dest != NULL);
    ASSERT(self.len == dest->len);
    ASSERT(self.len == 0 || self.arr != NULL);
    ASSERT(dest->len == 0 || dest->arr != NULL);
    for (size_t idx=0; idx < src.len; ++idx) {
        dest->arr[idx] = Foo_clone(self.arr[idx]);
    }
}
void FooArrayRef_free(FooArrayRef *const self) {
    if (self != NULL) {
        FooArrayRef_destroy(*self);
    }
    FREE(self);
}
FooArrayRef FooArrayRef_move(FooArrayRef const self) {
    FooArrayRef dest = self;
    // would wipe self but it's local only
    return dest;
}
FooArrayRef *FooArrayRef_malloc(FooArrayRef val) {
    FooArrayRef *const inst = MALLOC(sizeof(val));
    if (inst != NULL) {
        *inst = FooArrayRef_move(val);
    }
    return inst;
}


// Foo array, where array is allocated on heap and managed - Vector
// cap is current capacity of arr..
// uses heap: REALLOC to grow/shrink
typedef struct _FooVec FooVec;
struct _FooVec {
    Foo *arr;
    size_t cap;
    size_t len;
    //size_t quanta;?
};
// value variant
void FooVec_destroy(FooVec const self) {
    ASSERT(self.len == 0 || self.arr != NULL);
    for (size_t idx=0; idx< self.len; ++idx) {
        Foo_destroy(self.arr[idx]);
    }
}
FooVec FooVec_default(void) {
    FooVec inst = {.arr=NULL, .cap=0, .len=0};
    //FooVec inst = {.arr=NULL, .cap=0, .len=0, .quanta=8};
    return inst;
}
FooVec FooVec_new(size_t cap) {
    FooVec inst = {.arr = MALLOC(sizeof(*inst.arr)*cap), .cap = cap, .len = 0};
    //FooVec inst = {.arr = MALLOC(sizeof(*inst.arr)*cap), .cap = cap, .len = 0, .quanta = 8};
    return inst;
}
// FooVec FooVec_new(size_t cap, size_t quanta) {
//     FooVec inst = {.arr = MALLOC(sizeof(*inst.arr)*cap), .cap = cap, .len = 0, .quanta = quanta};
//     return inst;
// }
FooVec FooVec_clone(FooVec const self) {
    FooVec inst = self;
    ASSERT((inst.cap == 0 && inst.len == 0) || inst.arr != NULL);
    inst.arr = MALLOC(sizeof(*inst.arr)*inst.cap);
    for (size_t idx=0;idx<inst.len;++idx) {
        inst.arr[idx] = Foo_clone(self.arr[idx]);
    }
    return inst;
}
void FooVec_free(FooVec *const self) {
    if (self != NULL) {
        FooVec_destroy(*self);
    }
    FREE(self);
}
FooVec FooVec_move(FooVec const self) {
    FooVec dest = self;
    // wipe self here? no point, it's local.
    return dest;
}
FooVec *FooVec_malloc(FooVec val) {
    FooVec *const inst = MALLOC(sizeof(val));
    if (inst != NULL) {
        *inst = FooVec_move(val);
    }
    return inst;
}
void FooVec_push_back(FooVec *const self, Foo val) {
    ASSERT(self != NULL);
    ASSERT((self->len == 0 && self->cap == 0) || self->arr != NULL);
    if (self->len >= self->cap) {
        static const size_t quanta = 8;
        size_t const newcap = self->cap + quanta;
        //size_t const newcap = self->cap + self->quanta;
        self->arr = REALLOC(self->arr, sizeof(*self->arr)*newcap);
        self->cap = newcap;
    }
    ASSERT(self->arr != NULL);
    ASSERT(self->len < self->cap);
    self->arr[self->len] = Foo_move(val);
    self->len += 1;
}
bool FooVec_is_empty(FooVec const self) {
    ASSERT(self != NULL);
    return self->len == 0;
}
// UB is len == 0;
Foo FooVec_pop_back(FooVec *const self) {
    ASSERT(self != NULL);
    ASSERT(!FooVec_is_empty(self));
    ASSERT(self->arr != NULL);
    self->len -= 1;
    Foo val = Foo_move(self->arr[self->len]);
    return val;
}
// bool FooVec_pop_back(FooVec *const self, Foo *const val) {
//     ASSERT(self != NULL);
//     bool const ret = (self->len > 0);
//     if (ret) {
//         self->len -= 1;
//         pFoo_move(&val, &self->arr[self->len]);
//     }
//     return ret;
// }


//ptr variant
void pFooVec_destroy(FooVec *const self) {
    ASSERT(self != NULL);
    ASSERT(self->len == 0 || self->arr != NULL);
    for (size_t idx=0; idx< self->len; ++idx) {
        pFoo_destroy(&self->arr[idx]);
    }
}
/// self uninitialised before call
FooVec *pFooVec_ipdefault(FooVec *const self) {
    ASSERT(self != NULL);
    self->arr=NULL;
    self->cap=0;
    self->len=0;
    //self->quanta = 8;
    return self;
}
FooVec *pFooVec_ipnew(FooVec *const self, size_t cap) {
    ASSERT(self != NULL);
    self->arr=NULL;
    self->cap=cap;
    self->len=0;
    //self->quanta = 8;
    return self;
}
// FooVec *pFooVec_new(FooVec *const self, size_t cap, size_t quanta) {
//     self->arr=NULL;
//     self->cap=cap;
//     self->len=0;
//     self->quanta = quanta;
//     return self;
// }
FooVec *pFooVec_ipclone(const FooVec *const self, FooVec *const dest) {
    ASSERT(self != NULL);
    ASSERT(dest != NULL);
    *dest = *self;
    dest->arr = MALLOC(sizeof(*self->arr)*self->cap);
    for (size_t idx=0;idx<dest->len;++idx) {
        pFoo_clone(&self->arr[idx], &dest->arr[idx]);
    }
    return dest;
}
void pFooVec_free(FooVec *const self) {
    if (self != NULL) {
        pFooVec_destroy(self);
    }
    FREE(self);
}
FooVec *pFooVec_move(FooVec *const self, FooVec *const dest) {
    ASSERT(self != NULL);
    ASSERT(dest != NULL);
    *dest = *self;
    // wipe src here??
    return dest;
}
FooVec *pFooVec_malloc(FooVec *const val) {
    FooVec *const inst = MALLOC(sizeof(*val));
    if (inst != NULL) {
        pFooVec_move(val, inst);
    }
    return inst;
}
void pFooVec_clear(FooVec *const self);
//resize
//truncate
//extend
//swap?

// design constraint:
// push_back/pop_back take a ptr to T (symmetric?)
// push moves T into vec, pop moves T out of vec.
void pFooVec_push_back(FooVec *const self, Foo *const val) {
    ASSERT(val != NULL);
    ASSERT(self != NULL);
    if (self->len >= self->cap) {
        static const size_t quanta = 8;
        size_t const newcap = self->cap + quanta;
        self->arr = REALLOC(self->arr, sizeof(*self->arr)*newcap);
        self->cap = newcap;
    }
    ASSERT(self->len < self->cap);
    pFoo_move(val , &self->arr[self->len]);
    self->len += 1;
}
bool FooVec_is_empty(FooVec *const self) {
    ASSERT(self != NULL);
    return self->len == 0;
}
bool pFooVec_pop_back(FooVec *const self, Foo *const val) {
    bool const ok = !FooVec_is_empty(self);
    if (ok) {
        ASSERT(self != NULL);
        ASSERT((self->len == 0 && self->cap == 0) || self->arr != NULL);
        self->len -= 1;
        pFoo_move(&self->arr[self->len], val);
    }
    return ok;
}


// Foo array, where array is preallocated to fixed size - FixedVector
// cap is fixed size.
// len is current alloc
//
typedef struct _FooVecF FooVecF;
struct _FooVecF {
    Foo *arr;
    size_t cap;
    size_t len;
};
void FooVecF_erase(FooVecF const self) {
    for (size_t idx=0; idx < self.len; ++idx) {
        Foo_destroy(self.arr[idx]);
    }
}
void FooVecF_destroy(FooVecF const self) {
    FooVecF_erase(self);
}
// cannot have a default
FooVecF FooVecF_new(Foo *const arr, size_t cap) {
    FooVecF inst = {.arr = arr, .cap = cap, .len=0};
    return inst;
}
// cannot have clone, where to get backing store for clone?
// self is uninitialised? where to get backingstore from?

void FooVecF_free(FooVecF *const self) {
    if (self != NULL) {
        FooVecF_destroy(*self);
    }
    FREE(self);
}
FooVecF FooVecF_move(FooVecF const self) {
    FooVecF dest = self;
    // wipe self? no point, it's local
    return dest;
}
FooVecF *FooVecF_malloc(FooVecF const val) {
    FooVecF *const inst = MALLOC(sizeof(val));
    if (inst != NULL) {
        *inst = FooVecF_move(val);
    }
    return inst;
}
bool FooVecF_is_full(FooVecF const self) {
    return self.len >= self.cap;
}
void FooVecF_push_back(FooVecF *const self, Foo const val) {
    ASSERT(self != NULL);
    ASSERT(!FooVecF_is_full(self));
    ASSERT(self->arr != NULL);
    ASSERT(self->len > 0);
    self->arr[self->len] = Foo_move(val);
    self->len += 1;
}
bool FooVecF_is_empty(const FooVecF const self) {
    return self.len == 0;
}
Foo FooVecF_pop_back(FooVecF *const self) {
    ASSERT(!FooVecF_is_empty(self));
    ASSERT(self != NULL);
    ASSERT(self->arr != NULL);
    self->len -= 1;
    Foo val = Foo_move(self->arr[self->len]);
    return val;
}
