#include <cobj/t.h>
#include <cobj/based/cobj_queuevt.h>
#include <cobj/core/cobj_carray.h>

#include <stdio.h>
#include <assert.h>


int main(int argc, char *argv[]) {
    ((void)(argc));
    ((void)(argv));

    T backing[10];
    cobj_QueueVT vec;
    cobj_QueueVT_new(&vec, backing, COBJ_CARRAY_LEN(backing));

    T t1;
    T_default(&t1);

    bool ok;
    ok = cobj_QueueVT_put(&vec, &t1);
    if (!ok) {}

    ok = cobj_QueueVT_get(&vec, &t1);
    if (!ok) {}

    ok = cobj_QueueVT_put(&vec, &t1);
    if (!ok) {}

    T_default(&t1);
    ok = cobj_QueueVT_put(&vec, &t1);
    if (!ok) {}

    printf("foo");

    cobj_QueueVT_destroy(&vec);
}
