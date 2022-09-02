#include "cobj_panic.h"

#include <assert.h>

void panic(void)
{
    assert(false);
}

void panic_if(bool cond)
{
    assert(cond);
    (void)cond;
}
