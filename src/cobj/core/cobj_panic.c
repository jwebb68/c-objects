#include <assert.h>
#include <cobj/core/cobj_defs.h>
#include <cobj/core/cobj_panic.h>

NO_RETURN void cobj_panic_(char const *const file, int line, char const *const msg)
{
    COBJ_UNUSED_ARG(file);
    COBJ_UNUSED_ARG(line);
    COBJ_UNUSED_ARG(msg);
    assert(false);
    __builtin_unreachable();
}

NO_RETURN void cobj_panic0_(char const *const file, int line)
{
    COBJ_UNUSED_ARG(file);
    COBJ_UNUSED_ARG(line);
    assert(false);
    __builtin_unreachable();
}
