#if !defined(COBJ_PANIC_H)
#    define COBJ_PANIC_H

#    include <stdbool.h>

void panic(void);
void panic_if(bool cond);

#    define PANIC_IF(cond) panic_if(cond);

#endif //! defined(COBJ_PANIC_H)
