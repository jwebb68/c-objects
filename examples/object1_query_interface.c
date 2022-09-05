#include <cobj/interfaces/cobj_object1.h>
#include <cobj/interfaces/cobj_interface1.h>

#include <assert.h>


int main(int argc, char *argv[]) {
    ((void)argc);
    ((void)argv);

    cobj_Error err;

    cobj_Object1 o1;
    cobj_Object1_new(&o1);

    cobj_Interface1 *o1i1;
    bool ok;
    ok = cobj_Object1_query_interface_mut(&o1, &cobj_Interface1_ID, (void **)&o1i1, &err);
    assert(ok);

    cobj_Object1_destroy(&o1); // invalidates all ptrs and interfaces to o1

    return 0;
}
