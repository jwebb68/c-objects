#include "t.h"
#include "cobj_vectort.h"

void main() {
    T backing[10];
    VectorT vec;
    VectorT_new(&vec, backing, 10);

    T t1;
    VectorT_push_back(&vec, &t1);

    VectorT_pop_back(&vec, &t1);

    //etc

    VectorTIter it;
    VectorTIter_new(&it, &vec);
    // or VectorT_iter(&vec, &it);
    for (T const *p;  NULL != (p = VectorTIter_next(&it));) {
        // do something with p
        // p->...
    }
    VectorTIter_destroy(&it);

    VectorT_destroy(&vec);
}
// good, p is contained in the for-loop scope, pity 'it' isn't.
// bad 3 compares, one advance, one deref
// safer, wont advance past end, cannot deref at end.


// now can the above be re-implemented in terms of a common impl for all
// vectors using say void *, and a typed shim function set?
