#include "t.h"
#include "cobj_vectorvt.h"
#include "cobj_carray.h"

#include <stdio.h>
#include <assert.h>


int main(int argc, char *argv[]) {
    ((void)(argc));
    ((void)(argv));

    T backing[10];
    VectorVT vec;
    VectorVT_new(&vec, backing, COBJ_CARRAY_LEN(backing));

    T t1;
    T_new_default(&t1);

    bool ok;
    ok = VectorVT_push_back(&vec, &t1);
    if (!ok) {}

    ok = VectorVT_pop_back(&vec, &t1);
    if (!ok) {}

    ok = VectorVT_push_back(&vec, &t1);
    if (!ok) {}

    T_new_default(&t1);
    ok = VectorVT_push_back(&vec, &t1);
    if (!ok) {}

    printf("foo");

    //etc

    VectorVTIter it;
    VectorVTIter_new(&it, &vec);
    // or VectorT_iter(&vec, &it);
    for (T const *p;  NULL != (p = VectorVTIter_next(&it));) {
        // do something with p
        // p->...
        printf("foo %d", p->dummy);
    }
    VectorVTIter_destroy(&it);

    printf("foo");

    VectorVT_destroy(&vec);
}
// good, p is contained in the for-loop scope, pity 'it' isn't.
// bad 3 compares, one advance, one deref
// safer, wont advance past end, cannot deref at end.


// now can the above be re-implemented in terms of a common impl for all
// vectors using say void *, and a typed shim function set?
