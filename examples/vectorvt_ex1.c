#include <cobj/t.h>
#include <cobj/based/cobj_vectorvt.h>
#include <cobj/core/cobj_carray.h>

#include <stdio.h>
#include <assert.h>


int main(int argc, char *argv[]) {
    ((void)(argc));
    ((void)(argv));

    T backing[10];
    cobj_VectorVT vec;
    cobj_VectorVT_new(&vec, backing, COBJ_CARRAY_LEN(backing));

    T t1;
    T_default(&t1);

    bool ok;
    ok = cobj_VectorVT_push_back(&vec, &t1);
    if (!ok) {}

    ok = cobj_VectorVT_pop_back(&vec, &t1);
    if (!ok) {}

    ok = cobj_VectorVT_push_back(&vec, &t1);
    if (!ok) {}

    T_default(&t1);
    ok = cobj_VectorVT_push_back(&vec, &t1);
    if (!ok) {}

    printf("foo");

    //etc

    cobj_SliceVTIter it;
    cobj_VectorVT_iter(&vec, &it);
    for (T const *p;  NULL != (p = cobj_SliceVTIter_next(&it));) {
        // do something with p
        // p->...
        printf("foo %d", p->dummy);
    }
    cobj_SliceVTIter_destroy(&it);

    printf("foo");

    cobj_VectorVT_destroy(&vec);
}
// good, p is contained in the for-loop scope, pity 'it' isn't.
// bad 2 compares, one advance, one deref
// safer, wont advance past end, cannot deref at end.


// now can the above be re-implemented in terms of a common impl for all
// vectors using say void *, and a typed shim function set?
