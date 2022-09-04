#if !defined(COBJ_PANIC_H)
#    define COBJ_PANIC_H

#    include <cobj/core/cobj_defs.h>
#    include <stdbool.h>

NO_RETURN void cobj_panic_(char const *const file, int line, char const *const msg);
#    define cobj_panic(msg) cobj_panic_(__FILE__, __LINE__, msg)

NO_RETURN void cobj_panic0_(char const *const file, int line);
#    define cobj_panic0() cobj_panic0_(__FILE__, __LINE__)

// assert by any other name?
//  this cannot be fn as want the cond check to happen without messing with regs/stack and a func
//  will do that. also the file+line needs to be pushed AFTER the cond check, again what a func
//  cannot do.
#    define cobj_panic_if(cond, msg) \
        do { \
            if (!(cond)) { cobj_panic(msg); } \
        } while (0)

#endif //! defined(COBJ_PANIC_H)
