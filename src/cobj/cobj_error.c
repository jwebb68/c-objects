#include "cobj_error.h"

#include "cobj_memory.h"

static void Error_wipe(Error *const self)
{
    STRUCTWIPE(self);
}

void Error_destroy(Error *const self)
{
    Error_wipe(self);
}

void Error_new(Error *const self, ErrorCode code, size_t line, char const *const filen)
{
    self->code = code;
    self->line = line;
    self->filen = filen;
}

bool WARN_UNUSED_RESULT Error_raise(Error *const self,
                                    ErrorCode code,
                                    size_t line,
                                    char const *const filen)
{
    Error_new(self, code, line, filen);
    return true;
}
