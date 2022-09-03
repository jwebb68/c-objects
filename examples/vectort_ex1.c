#include <cobj/t.h>
#include <cobj/baseless/cobj_vectort.h>
#include <cobj/core/cobj_carray.h>

#include <stdio.h>
#include <assert.h>


int main(int argc, char *argv[]) {
    ((void)(argc));
    ((void)(argv));

    T backing[10];
    cobj_VectorT vec;
    cobj_VectorT_new(&vec, backing, COBJ_CARRAY_LEN(backing));

    T t1;
    T_default(&t1);

    bool ok;
    ok = cobj_VectorT_push_back(&vec, &t1);
    if (!ok) {}

    ok = cobj_VectorT_pop_back(&vec, &t1);
    if (!ok) {}

    ok = cobj_VectorT_push_back(&vec, &t1);
    if (!ok) {}

    T_default(&t1);
    ok = cobj_VectorT_push_back(&vec, &t1);
    if (!ok) {}

    printf("foo");

    //etc

    cobj_SliceTIter it;
    cobj_VectorT_iter(&vec, &it);
    for (T const *p;  NULL != (p = cobj_SliceTIter_next(&it));) {
        // do something with p
        // p->...
        printf("foo %d", p->dummy);
    }
    cobj_SliceTIter_destroy(&it);

    printf("foo");

    cobj_VectorT_destroy(&vec);
}
// good, p is contained in the for-loop scope, pity 'it' isn't.
// bad 3 compares, one advance, one deref
// safer, wont advance past end, cannot deref at end.


// now can the above be re-implemented in terms of a common impl for all
// vectors using say void *, and a typed shim function set?
