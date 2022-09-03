#include <cobj/t.h>
#include <cobj/baseless/cobj_arrayt.h>
#include <cobj/core/cobj_carray.h>

#include <stdio.h>
#include <assert.h>


int main(int argc, char *argv[]) {
    ((void)(argc));
    ((void)(argv));

    T backing[10];

    // arrays must be initialised
    // so either default all elems on create
    // or copy/move from existing array.
    // if move then what happens to src?
    cobj_ArrayTMut arr;
    cobj_ArrayTMut_default(&arr, backing, COBJ_CARRAY_LEN(backing));

    printf("foo");

    cobj_SliceTIter it;
    cobj_ArrayTMut_iter(&arr, &it);
    for (T const *p;  NULL != (p = cobj_SliceTIter_next(&it));) {
        // do something with p
        // p->...
        printf("ham %d", p->dummy);
    }
    cobj_SliceTIter_destroy(&it);

    printf("foo");

    cobj_ArrayTMut_destroy(&arr);
}
// good, p is contained in the for-loop scope, pity 'it' isn't.
// bad 3 compares, one advance, one deref
// safer, wont advance past end, cannot deref at end.


// now can the above be re-implemented in terms of a common impl for all
// vectors using say void *, and a typed shim function set?
