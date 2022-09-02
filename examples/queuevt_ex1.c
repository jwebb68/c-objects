#include <cobj/t.h>
#include <cobj/cobj_queuevt.h>
#include <cobj/cobj_carray.h>

#include <stdio.h>
#include <assert.h>


int main(int argc, char *argv[]) {
    ((void)(argc));
    ((void)(argv));

    T backing[10];
    QueueVT vec;
    QueueVT_new(&vec, backing, COBJ_CARRAY_LEN(backing));

    T t1;
    T_default(&t1);

    bool ok;
    ok = QueueVT_put(&vec, &t1);
    if (!ok) {}

    ok = QueueVT_get(&vec, &t1);
    if (!ok) {}

    ok = QueueVT_put(&vec, &t1);
    if (!ok) {}

    T_default(&t1);
    ok = QueueVT_put(&vec, &t1);
    if (!ok) {}

    printf("foo");

    QueueVT_destroy(&vec);
}
