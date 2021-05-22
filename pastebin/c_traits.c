#include <inttypes.h> // uint8_t
#include <string.h> // size_t

// in f.h
struct _F;
typedef struct _F F;
struct _F {
    int a;
    float b;
};

// in .h
struct _Trait1_for_F;
typedef struct _Trait1_for_F Trait1_for_F;
struct _Trait1_for_F {
    void (*foo)(const F *self);
    int (*write_cbuf)(F *self, const uint8_t *buf, size_t buflen);
    int (*write_ccbuf)(F *self, const uint8_t *b, const uint8_t *e);
}
#define DECLARE_TRAIT(tn, sn) \
    struct _ ## tn ## _for_ ## sn ## _type; \
    typedef struct _ ## tn ## _for_ ## sn ## _type tn ## _for_ ## sn ## _type; \
    extern const tn ## _for_ ## sn ## _type  tn ## _for_ ## sn;

#define DEFINE_TRAIT(tn, sn) \
   struct _ ## tn ##_for_## sn ## _type

DECLARE_TRAIT(Trait1, F);

#define DEFINE_TRAIT_METHOD(tn, sn, rt, name, ...) \
    rt (* tn ## _for_ ## sn ## _ ## name)(sn *self, __VA_ARGS__)

DEFINE_TRAIT(Trait1, F) {
    DEFINE_TRAIT_METHOD(Trait1, F, void, foo, const char *boo);
    DEFINE_TRAIT_METHOD(Trait1, F, int, bar);
    DEFINE_TRAIT_METHOD(Trait1, F, int);
    DEFINE_TRAIT_METHOD(Trait1, F, void, nim, uint8_t *b, uint8_t *e);
};

#define IMPLEMENT_TRAIT(tn, sn) \
    const tn ## _for_ ## sn ## _type  tn ## _for_ ## sn = {

IMPLEMENT_TRAIT(Trait1, F) {
}

//in .c
int Trait1_for_F_foo(const F *self) {
    return -1;
}

//in .c
static int Trait1_for_Fwrite_cbuf(F *self, const uint8_t *buf, size_t buflen) {
    return -1
}

//in .c
static int Trait1_for_Fwrite_ccbuf(F *self, const uint8_t *b, const uint8_t e) {
    return -1
}

// in .c
const Trait1_for_F trait1_for_f = {
    Trait_for_F_foo,
    Trait_for_F_write_cbuf,
    Trait_for_F_write_ccbuf,
}

// in .h
extern const Trait1_for_F trait1_for_f

// in .h
struct Trait2_for_F {
    const char *(*str)(const F *self);
}

static const char *Trait2_for_F_str(const F *self) {
    return "F";
}

const Trait2_for_F trait2_for_f = {
    Trait2_for_F_str
};

extern const Trait2_for_F trait2_for_f;

// in main.c
int main() {
    F f;
    trait_for_f->foo(&f);

    int res;
    uint8_t buf[23];
    res = trait_for_f->write_cbuf(&f, buf, sizeof(buf));
    res = trait_for_f->write_cbuf(&f, carray_begin(buf), carray_end(buf));

    printf("%s", trait2_for_f_str(&f));

    return 0;
}
