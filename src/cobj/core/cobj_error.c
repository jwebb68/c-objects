#include <cobj/core/cobj_error.h>
#include <cobj/core/cobj_memory.h>

static void cobj_ErrorCode_wipe(cobj_Error *const self)
{
    STRUCTWIPE(self);
}

void cobj_Error_destroy(cobj_Error *const self)
{
    cobj_ErrorCode_wipe(self);
}

void cobj_Error_new(cobj_Error *const self,
                    cobj_ErrorCode code,
                    size_t line,
                    char const *const filen)
{
    self->code = code;
    self->line = line;
    self->filen = filen;
}

bool WARN_UNUSED_RESULT cobj_Error_raise_(cobj_Error *const self,
                                          cobj_ErrorCode code,
                                          size_t line,
                                          char const *const filen)
{
    cobj_Error_new(self, code, line, filen);
    return false;
}
