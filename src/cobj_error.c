#include "cobj_error.h"
#include "cobj_memory.h"


static void Error_wipe(Error *const self) {
    STRUCTWIPE(self);
}


void Error_destroy(Error *const self) {
    Error_wipe(self);
}


void Error_new_(Error *const self, ErrorCode code, size_t line, char const *filen) {
    self->code = code;
    self->line = line;
    self->filen = filen;
}


bool Error_raise(Error *const self, ErrorCode code, size_t line, char const *filen) {
    Error_new(self, code, line, filen);
    return true;
}
