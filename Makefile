# build main code/test/examples for modules
# modules are subfolders from root, containing a main for exes, liba for statics and libso for shared.
# all other .c,.cc files are treated as part of that module.
# <modl>/src/main.{c,cc}
# <modl>/src/liba.{c,cc}
# <modl>/src/libso.{c,cc}
# builds an exe based on module name. <modl> for exes, lib<modl>.a for statics, lib<modl>.so for shared.
# for defined configs (debug,release)
# output is in build/<config>/
# builds tests and runs them for each module.
# tests are .c,.cc files in <modl>/src/ matching pattern test_*.{c,cc}
# tests are built into build/<config>/tests/test_<modl> as an exe
# builds examples as found in the examples/ folder.
# each .c.cc file is built into an exe into the build/<config/examples folder.
#
# you will need pkg-config and catch2 installed (for c++ tests)
# TODO: pick details up from .yaml file (will mean dropping make)
# caveats:
# will only work if only one of main.c main.cc liba.c liba.cc libso.c libso.cc exist for a module
# it will die horribly if theres not only one such.
# TODO: allow specifying .c/.cc files for each module and not auto-detect.
# TODO: rewrite in C++ and not makefile.. (or rust for portability/self-containment)

.SUFFIXES:
.SUFFIXES: c cc a o

# toolchains
# GCC
# CLANG
# ARM
TOOLCHAIN ?= native

ifeq ($(TOOLCHAIN),native)
OBJDUMP=objdump
CC=cc
CXX=c++
AR=gcc-ar
AS=as
LINK=$(CC)

else ifeq ($(TOOLCHAIN),gnu)

OBJDUMP=objdump
CC=gcc
CXX=g++
AR=gcc-ar
AS=as
LINK=$(CC)

else ifeq ($(TOOLCHAIN),clang)

OBJDUMP=objdump
CC=clang
CXX=clang++
AR=ar
AS=as
LINK=$(CC)

else ifeq ($(TOOLCHAIN),arm)

CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
OBJDUMP=arm-none-eabi-objdump
LDFLAGS+=--specs=nosys.specs
AR=arm-none-eabi-gcc-ar
AS=arm-none-eabi-as
LINK=$(CC)

else

$(error TOOLCHAIN='$(TOOLCHAIN)' not supported)

endif


TARGET_TRIP:=$(CC) -dumpmachine

# Redefine link operation as not able to assign CC to $(CXX) in build rules.
# every attempt does not reassign to CC, just uses CC which messes up C++ linking
# but reassigning into another var works..
# ie. CC= $(CXX) doesn't, LINK=$(CXX) does..
LINK.o = $(LINK) $(LDFLAGS) $(TARGET_ARCH)

allowed_configs:=
allowed_configs+=debug
allowed_configs+=release
allowed_configs+=minsize
allowed_configs+=fast
ifeq ($(origin configs), undefined)
configs:=$(allowed_configs)
endif
ifneq ($(filter-out $(allowed_configs),$(configs)),)
$(error allowed configs: $(allowed_configs))
endif

#CFLAGS already defined
CFLAGS += -Werror -Wall -Wextra -Wpedantic

# catch2 is forcing use of exceptions.
# need test lib that doesn't use exceptions.
# only want 1 exception: panic/abort/exit.
#CXXFLAGS += -Werror -Wall -Wextra -Wpedantic -std=c++17 -fno-exceptions
CXXFLAGS += -Werror -Wall -Wextra -Wpedantic -std=c++17

# CFLAGS additionals for debug
debug_CFLAGS := -Og -g
debug_CXXFLAGS := -Og -g
debug_CPPFLAGS := -DDEBUG
#debug_LDFLAGS:=

release_CFLAGS := -O3 -g
release_CXXFLAGS := -O3 -g
release_CPPFLAGS := -DRELEASE -DNDEBUG
#release_LDFLAGS:=
#release_LDFLAGS += --lto
#release_CFLAGS += --lto
#release_CXXFLAGS += --lto

minsize_CFLAGS := -Os -g
minsize_CXXFLAGS := -Os -g
minsize_CPPFLAGS := -DRELEASE -DNDEBUG
#minsize_LDFLAGS:=
#minsize_LDFLAGS+= --lto
#minsize_CFLAGS += --lto
#minsize_CXXFLAGS += --lto

fast_CFLAGS := -Ofast -g
fast_CXXFLAGS := -Ofast -g
fast_CPPFLAGS := -DRELEASE -DNDEBUG
#fast_LDFLAGS:=
fast_LDFLAGS+= --lto
fast_CFLAGS += --lto
#fast_CXXFLAGS += --lto

# # 'global' additionals for compoent

# foo_CXXFLAGS :=

# # additionals for component per config
# foo_debug_CFLAGS :=
# foo_debug_CPPFLAGS :=
# foo_debug_CXXFLAGS :=

# foo_release_CFLAGS :=
# foo_release_CPPFLAGS :=
# foo_release_CXXFLAGS :=

# foo_minsize_CFLAGS :=
# foo_minsize_CPPFLAGS :=
# foo_minsize_CXXFLAGS :=

# foo_fast_CFLAGS :=
# foo_fast_CPPFLAGS :=
# foo_fast_CXXFLAGS :=

# foo_DEPS:=

# modls:=
# modls+=foo
name:=foo

# build makefile to build/clean buildtree from src
# in makefile:
# $<modl>_DEPS:= lists deps of modl, relative to root of project.
# must be in same project..TODO: cross project building so can pull in projs from other repos.
# $modls:= <list of modules to build.

# folder tree laid out thus:
# examples/<ex1>.c
# examples/<ex2>.cc
# /src/
# /src/main.{c,cc} # for exe builds
# /src/liba.{c,cc} # for static lib builds
# /src/libso.{c,cc} # for shared lib builds
# /src/path/to/<other>.{c,cc}

# will compile examples/<file>.c into build/<config>/obj/examples/<file>.o
# will link build/<config>/obj/examples/<file>.o into build/<config>/examples/<file>
# i.e. every single .c and .cc file compiles to a single product.
# dependencies are from main project (is no deps between examples)

# will compile src/<file>.{c,cc} into build/<config>/obj/<file>.o
# will preprocess src/<file>.{c,cc} into build/<config>/dep/<file>.d for deps tracking of included files.

# for c,cc exes:
# will link build/<config>/obj/<file>.o into build/<config>/<name>
# for c,cc static libss:
# will link build/<config>/obj/<file>.o into build/<config>/lib<modl>.a
# for c,cc shared libs
# will link build/<config>/obj/<file>.o into build/<config>/lib<modl>.so

# TODO: error out if main.c/main.cc exist together
# TODO: error out if liba.c/liba.cc exist together
# TODO: error out if libso.c/libso.cc exist together
# TODO: error out if main.{c,cc} or liba.{c,cc} or libso.{c,cc} exist together

all: build

#allsrc:=$(foreach f,$(modls),$(wildcard $(f)/src/*.c $(f)/src/*.cc))
#allhdr:=$(foreach f,$(modls),$(wildcard $(f)/src/*.h $(f)/src/*.hh))
#testsrc:= $(foreach f,$(modls),$(wildcard $(f)/src/test_*.c $(f)/src/test_*.cc))
#src:=$(filter-out $(testsrc),$(foreach f,$(modls),$(wildcard $(f)/src/*.c $(f)/src/*.cc)))
allsrc:=$(wildcard src/*.c src/*.cc)
testsrc:=$(wildcard src/test_*.c src/test_*.cc)
src:=$(filter-out $(testsrc),$(allsrc))

obj:=
clean:=
dep:=
rtests:=


COMPILE.cdep=$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MP -MM
COMPILE.ccdep=$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MP -MM

# package:
# name in worksp/<tool>.?: [project]name= ?
# source in src/**
# must have one of a (main|liba|libso).{c,cc,a} in src/
# if main, then builds as build/<config>/<name>
# if liba then builds as build/<config>/lib<name>.a
# if libso then builds as build/<config>/lib<name>.so
# will build obj to build/<config>/obj/<name>/**
# will build deps to build/<config>/deps/<name>/**
# will build to build/<config>/
# subfolders of src build to same stem in build/<config>/obj/<name>/
# -e.g. src/path/to/file.c builds to build/<config>/obj/<name>/path/to/file.o
# TODO: handle depends, i.e. in <tool>.?: [depends]<dep_name>=<path/to_submodule (relative from<tool>.? location)>


## $1 = modl, $2 = config
# $1 = name, $2 = config
define mk_build

clean+=$(patsubst src/%.c,build/$(2)/obj/%.o,$(filter src/%.c,$(src)))
$(if $(filter src/%.c,$(src)), $(patsubst src/%.c,build/$(2)/obj/%.o,$(filter src/%.c,$(src))): | build/$(2)/obj)
$(if $(filter src/%.c,$(src)), $(patsubst src/%.c,build/$(2)/dep/%.d,$(filter src/%.c,$(src))): | build/$(2)/dep)
$(if $(filter src/%.c,$(src)), $(patsubst src/%.c,build/$(2)/dep/%.d,$(filter src/%.c,$(src))): CPPFLAGS += $$($(2)_CPPFLAGS))
$(if $(filter src/%.c,$(src)), $(patsubst src/%.c,build/$(2)/obj/%.o,$(filter src/%.c,$(src))): CPPFLAGS += $$($(2)_CPPFLAGS))
$(if $(filter src/%.c,$(src)), $(patsubst src/%.c,build/$(2)/obj/%.o,$(filter src/%.c,$(src))): CFLAGS += $$($(2)_CFLAGS))
$(if $(filter src/%.c,$(src)), $(patsubst src/%.c,build/$(2)/obj/%.o,$(filter src/%.c,$(src))): build/$(2)/obj/%.o: src/%.c ;$$(COMPILE.c) -o $$@  $$<)
$(if $(filter src/%.c,$(src)), $(patsubst src/%.c,build/$(2)/dep/%.d,$(filter src/%.c,$(src))): build/$(2)/dep/%.d: src/%.c ;$$(COMPILE.cdep) -MT $$(patsubst src/%.c,build/$(2)/obj/%.o,$$<) -o $$@ $$<)
$(if $(filter src/%.c,$(src)), dep += $(patsubst src/%.c,build/$(2)/dep/%.d,$(filter src/%.c,$(src))))
$(if $(filter src/liba.c,$(src)), build/$(2)/lib$(name).a: | build/$(2))
$(if $(filter src/liba.c,$(src)), build/$(2)/lib$(name).a: $(patsubst src/%.c,build/$(2)/obj/%.o,$(filter src/%.c,$(src))); $$(AR) rcs $$@ $$^)
# $(if $(filter src/liba.c,$(src)), build/$(2)/$(name): $($(name)_DEP),$$($(name)_targ))
$(if $(filter src/liba.c,$(src)), build: build/$(2)/lib$(name).a)
$(if $(filter src/liba.c,$(src)), clean += build/$(2)/lib$(name).a)
$(if $(filter src/liba.c,$(src)), $(2)_$(name)_targ:=build/$(2)/lib$(name).a)
$(if $(filter src/main.c,$(src)), build/$(2)/$(name): | build/$(2))
$(if $(filter src/main.c,$(src)), build/$(2)/$(name): LDFLAGS += $($(2)_LDFLAGS))
$(if $(filter src/main.c,$(src)), build/$(2)/$(name): LDLIBS += $($(2)_LDLIBS))
$(if $(filter src/main.c,$(src)), build/$(2)/$(name): $(patsubst src/%.c,build/$(2)/obj/%.o,$(filter src/%.c,$(src))); $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
# $(if $(filter src/main.c,$(src)), build/$(2)/$(1): $(foreach m,$($(1)_DEP),$$($(m)_targ)))
$(if $(filter src/main.c,$(src)), build: build/$(2)/$(name))
$(if $(filter src/main.c,$(src)), clean += build/$(2)/$(name))
$(if $(filter src/main.c,$(src)),$(name)_targ := build/$(2)/$(name))


clean+=$(patsubst src/%.cc,build/$(2)/obj/%.o,$(filter src/%.cc,$(src)))
$(if $(filter src/%.cc,$(src)), $(patsubst src/%.cc,build/$(2)/obj/%.o,$(filter src/%.cc,$(src))): | build/$(2)/obj)
$(if $(filter src/%.cc,$(src)), $(patsubst src/%.cc,build/$(2)/dep/%.d,$(filter src/%.cc,$(src))): | build/$(2)/dep)
$(if $(filter src/%.cc,$(src)), $(patsubst src/%.cc,build/$(2)/dep/%.d,$(filter src/%.cc,$(src))): CPPFLAGS += $$($(2)_CPPFLAGS))
$(if $(filter src/%.cc,$(src)), $(patsubst src/%.cc,build/$(2)/obj/%.o,$(filter src/%.cc,$(src))): CPPFLAGS += $$($(2)_CPPFLAGS))
$(if $(filter src/%.cc,$(src)), $(patsubst src/%.cc,build/$(2)/obj/%.o,$(filter src/%.cc,$(src))): CXXFLAGS += $$($(2)_CXXFLAGS))
$(if $(filter src/%.cc,$(src)), $(patsubst src/%.cc,build/$(2)/obj/%.o,$(filter src/%.cc,$(src))): build/$(2)/obj/%.o: src/%.cc ;$$(COMPILE.cc) -o $$@  $$<)
$(if $(filter src/%.cc,$(src)), $(patsubst src/%.cc,build/$(2)/dep/%.d,$(filter src/%.cc,$(src))): build/$(2)/dep/%.d: src/%.cc ;$$(COMPILE.ccdep) -MT $$(patsubst src/%.cc,build/$(2)/obj/%.o,$$<) -o $$@  $$<)
$(if $(filter src/%.cc,$(src)), dep += $(patsubst src/%.cc,build/$(2)/dep/%.d,$(filter src/%.cc,$(src))))
$(if $(filter src/liba.cc,$(src)),build/$(2)/lib$(name).a: | build/$(2))
$(if $(filter src/liba.cc,$(src)),build/$(2)/lib$(name).a: $(patsubst src/%.cc,build/$(2)/obj/%.o,$(filter src/%.cc,$(src))); $$(AR) rcs $$@ $$^)
# $(if $(filter src/liba.cc,$(src)),build/$(2)/$(1): $(foreach m,$($(1)_DEP),$$($(m)_targ)))
$(if $(filter src/liba.cc,$(src)),build: build/$(2)/lib$(name).a)
$(if $(filter src/liba.cc,$(src)),clean += build/$(2)/lib$(name).a)
# $(if $(filter src/liba.cc,$(src)),$(name)_targ := build/$(2)/lib$(name).a)
$(if $(filter src/main.cc,$(src)),build/$(2)/$(name): | build/$(2))
$(if $(filter src/main.cc,$(src)),build/$(2)/$(name): LDFLAGS += $($(2)_LDFLAGS))
$(if $(filter src/main.cc,$(src)),build/$(2)/$(name): LDLIBS += $($(2)_LDLIBS))
$(if $(filter src/main.cc,$(src)),build/$(2)/$(name): LINK = $(CXX))
$(if $(filter src/main.cc,$(src)),build/$(2)/$(name): $(patsubst src/%.cc,build/$(2)/obj/%.o,$(filter src/%.cc,$(src))); $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
# $(if $(filter src/main.cc,$(src)),build/$(2)/$(1): $(foreach m,$($(1)_DEP),$$($(m)_targ)))
$(if $(filter src/main.cc,$(src)),build: build/$(2)/$(name))
$(if $(filter src/main.cc,$(src)),clean += build/$(2)/$(name))
# $(if $(filter src/main.cc,$(src)),$(name)_targ := build/$(2)/$(name))

clean+=$(patsubst src/%.s,build/$(2)/obj/%.o,$(filter src/%.s,$(src)))
$(if $(filter src/%.s,$(src)), $(patsubst src/%.s,build/$(2)/obj/%.o,$(filter src/%.s,$(src))): | build/$(2)/obj)
$(if $(filter src/%.s,$(src)), $(patsubst src/%.s,build/$(2)/dep/%.d,$(filter src/%.s,$(src))): | build/$(2)/dep)
$(if $(filter src/%.s,$(src)), $(patsubst src/%.s,build/$(2)/obj/%.o,$(filter src/%.s,$(src))): build/$(2)/obj/%.o: src/%.s ;$$(COMPILE.s) -o $$@  $$<)
$(if $(filter src/%.s,$(src)), $(patsubst src/%.s,build/$(2)/dep/%.d,$(filter src/%.s,$(src))): build/$(2)/dep/%.d: src/%.s ;$$(COMPILE.s) -MP -MMD -MT $$(patsubst src/%.s,build/$(2)/obj/%.o,$$<) -o $$@  $$<)
$(if $(filter src/%.s,$(src)), dep +=$(patsubst src/%.s,build/$(2)/dep/%.d,$(filter src/%.s,$(src))))
endef


# clean+=$(patsubst $(1)/src/%.c,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.c,$(src)))
# clean+=$(patsubst $(1)/src/%.cc,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.cc,$(src)))
# clean+=$(patsubst $(1)/src/%.s,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.s,$(src)))

# build/$(2)/obj/$(1): ; mkdir -p $$@
# build/$(2)/dep/$(1): ; mkdir -p $$@

# build/debug/obj/foo/
# build/debug/dep/foo/
# build/debug/foo/


# $(if $(filter $(1)/src/%.c,$(src)),$(patsubst $(1)/src/%.c,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.c,$(src))): | build/$(2)/obj/$(1))
# $(if $(filter $(1)/src/%.cc,$(src)),$(patsubst $(1)/src/%.cc,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.cc,$(src))): | build/$(2)/obj/$(1))
# $(if $(filter $(1)/src/%.s,$(src)),$(patsubst $(1)/src/%.s,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.s,$(src))): | build/$(2)/obj/$(1))

# $(if $(filter $(1)/src/%.c,$(src)),$(patsubst $(1)/src/%.c,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.c,$(src))): | build/$(2)/dep/$(1))
# $(if $(filter $(1)/src/%.cc,$(src)),$(patsubst $(1)/src/%.cc,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.cc,$(src))): | build/$(2)/dep/$(1))
# $(if $(filter $(1)/src/%.s,$(src)),$(patsubst $(1)/src/%.s,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.s,$(src))): | build/$(2)/dep/$(1))

# $(if $(filter $(1)/src/%.c,$(src)),$(patsubst $(1)/src/%.c,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.c,$(src))): CPPFLAGS:=$$(CPPFLAGS) $$($(1)_CPPFLAGS) $$($(2)_CPPFLAGS) $$($(1)_$(2)_CPPFLAGS) $(foreach m,$($(1)_DEPS),-I$(m)/src))
# $(if $(filter $(1)/src/%.cc,$(src)),$(patsubst $(1)/src/%.cc,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.cc,$(src))): CPPFLAGS:=$$(CPPFLAGS) $$($(1)_CPPFLAGS) $$($(2)_CPPFLAGS) $$($(1)_$(2)_CPPFLAGS) $(foreach m,$($(1)_DEPS),-I$(m)/src))

# $(if $(filter $(1)/src/%.c,$(src)),$(patsubst $(1)/src/%.c,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.c,$(src))): CPPFLAGS:=$$(CPPFLAGS) $$($(1)_CPPFLAGS) $$($(2)_CPPFLAGS) $$($(1)_$(2)_CPPFLAGS) $(foreach m,$($(1)_DEPS),-I$(m)/src))
# $(if $(filter $(1)/src/%.c,$(src)),$(patsubst $(1)/src/%.c,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.c,$(src))): CFLAGS:=$$(CFLAGS) $$($(1)_CFLAGS) $$($(2)_CFLAGS) $$($(1)_$(2)_CFLAGS))

# $(if $(filter $(1)/src/%.cc,$(src)),$(patsubst $(1)/src/%.cc,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.cc,$(src))): CPPFLAGS:=$$(CPPFLAGS) $$($(1)_CPPFLAGS) $$($(2)_CPPFLAGS) $$($(1)_$(2)_CPPFLAGS)  $(foreach m,$($(1)_DEPS),-I$(m)/src))
# $(if $(filter $(1)/src/%.cc,$(src)),$(patsubst $(1)/src/%.cc,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.cc,$(src))): CXXFLAGS:=$$(CXXFLAGS) $$($(1)_CXXFLAGS) $$($(2)_CXXFLAGS) $$($(1)_$(2)_CXXFLAGS))

# $(if $(filter $(1)/src/%.c,$(src)),$(patsubst $(1)/src/%.c,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.c,$(src))): build/$(2)/obj/$(1)/%.o: $(1)/src/%.c ;$$(COMPILE.c) -o $$@  $$<)
# $(if $(filter $(1)/src/%.cc,$(src)),$(patsubst $(1)/src/%.cc,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.cc,$(src))): build/$(2)/obj/$(1)/%.o: $(1)/src/%.cc ;$$(COMPILE.cc) -o $$@  $$<)
# $(if $(filter $(1)/src/%.s,$(src)),$(patsubst $(1)/src/%.s,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.s,$(src))): build/$(2)/obj/$(1)/%.o: $(1)/src/%.s ;$$(COMPILE.s) -o $$@  $$<)


# $(if $(filter $(1)/src/%.c,$(src)),$(patsubst $(1)/src/%.c,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.c,$(src))): build/$(2)/dep/$(1)/%.d: $(1)/src/%.c ;$$(COMPILE.c) -MM -MT $$(patsubst $(1)/src/%.c,build/$(2)/obj/$(1)/%.o,$$<) -o $$@ $$<)
# $(if $(filter $(1)/src/%.cc,$(src)),$(patsubst $(1)/src/%.cc,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.cc,$(src))): build/$(2)/dep/$(1)/%.d: $(1)/src/%.cc ;$$(COMPILE.cc) -MM -MT $$(patsubst $(1)/src/%.cc,build/$(2)/obj/$(1)/%.o,$$<) -o $$@  $$<)
# $(if $(filter $(1)/src/%.s,$(src)),$(patsubst $(1)/src/%.s,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.s,$(src))): build/$(2)/dep/$(1)/%.d: $(1)/src/%.s ;$$(COMPILE.s) -MM -MT $$(patsubst $(1)/src/%.s,build/$(2)/obj/$(1)/%.o,$$<) -o $$@  $$<)

# $(if $(filter $(1)/src/%.c,$(src)),dep += $(patsubst $(1)/src/%.c,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.c,$(src))))
# $(if $(filter $(1)/src/%.cc,$(src)),dep += $(patsubst $(1)/src/%.cc,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.cc,$(src))))
# $(if $(filter $(1)/src/%.s,$(src)),dep +=$(patsubst $(1)/src/%.s,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.s,$(src))))

# $(if $(filter $(1)/src/liba.c,$(src)),build/$(2)/lib$(1).a: | build/$(2))
# $(if $(filter $(1)/src/liba.c,$(src)),build/$(2)/lib$(1).a: $(patsubst $(1)/src/%.c,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.c,$(src))); $$(AR) rcs $$@ $$^)
# $(if $(filter $(1)/src/liba.c,$(src)),build/$(2)/$(1): $(foreach m,$($(1)_DEP),$$($(m)_targ)))
# $(if $(filter $(1)/src/liba.c,$(src)),build: build/$(2)/lib$(1).a)
# $(if $(filter $(1)/src/liba.c,$(src)),clean += build/$(2)/lib$(1).a)
# $(if $(filter $(1)/src/liba.c,$(src)),$(1)_targ := build/$(2)/lib$(1).a)

# $(if $(filter $(1)/src/liba.cc,$(src)),build/$(2)/lib$(1).a: | build/$(2))
# $(if $(filter $(1)/src/liba.cc,$(src)),build/$(2)/lib$(1).a: $(patsubst $(1)/src/%.cc,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.cc,$(src))); $$(AR) rcs $$@ $$^)
# $(if $(filter $(1)/src/liba.cc,$(src)),build/$(2)/$(1): $(foreach m,$($(1)_DEP),$$($(m)_targ)))
# $(if $(filter $(1)/src/liba.cc,$(src)),build: build/$(2)/lib$(1).a)
# $(if $(filter $(1)/src/liba.cc,$(src)),clean += build/$(2)/lib$(1).a)
# $(if $(filter $(1)/src/liba.cc,$(src)),$(1)_targ := build/$(2)/lib$(1).a)

# $(if $(filter $(1)/src/main.c,$(src)),build/$(2)/$(1): | build/$(2))
# $(if $(filter $(1)/src/main.c,$(src)),build/$(2)/$(1): LDFLAGS += $($(2)_LDFLAGS))
# $(if $(filter $(1)/src/main.c,$(src)),build/$(2)/$(1): LDLIBS += $($(2)_LDLIBS))
# $(if $(filter $(1)/src/main.c,$(src)),build/$(2)/$(1): $(patsubst $(1)/src/%.c,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.c,$(src))); $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
# $(if $(filter $(1)/src/main.c,$(src)),build/$(2)/$(1): $(foreach m,$($(1)_DEP),$$($(m)_targ)))
# $(if $(filter $(1)/src/main.c,$(src)),build: build/$(2)/$(1))
# $(if $(filter $(1)/src/main.c,$(src)),clean += build/$(2)/$(1))
# $(if $(filter $(1)/src/main.c,$(src)),$(1)_targ := build/$(2)/$(1))

# $(if $(filter $(1)/src/main.cc,$(src)),build/$(2)/$(1): | build/$(2))
# $(if $(filter $(1)/src/main.cc,$(src)),build/$(2)/$(1): LDFLAGS += $($(2)_LDFLAGS))
# $(if $(filter $(1)/src/main.cc,$(src)),build/$(2)/$(1): LDLIBS += $($(2)_LDLIBS))
# $(if $(filter $(1)/src/main.cc,$(src)),build/$(2)/$(1): LINK = $(CXX))
# $(if $(filter $(1)/src/main.cc,$(src)),build/$(2)/$(1): $(patsubst $(1)/src/%.cc,build/$(2)/obj/$(1)/%.o,$(filter $(1)/src/%.cc,$(src))); $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
# $(if $(filter $(1)/src/main.cc,$(src)),build/$(2)/$(1): $(foreach m,$($(1)_DEP),$$($(m)_targ)))
# $(if $(filter $(1)/src/main.cc,$(src)),build: build/$(2)/$(1))
# $(if $(filter $(1)/src/main.cc,$(src)),clean += build/$(2)/$(1))
# $(if $(filter $(1)/src/main.cc,$(src)),$(1)_targ := build/$(2)/$(1))

#endef


$(foreach c,$(configs),build/$(c) build/$(c)/obj build/$(c)/dep):
	mkdir -p $@

# $(foreach m,$(modls),$(foreach c,$(configs),$(eval $(call mk_modl,$(m),$(c)))))
$(foreach c,$(configs),$(eval $(call mk_build,$(feck),$(c))))


examples:=
examples+=$(patsubst examples/%.c,%,$(wildcard examples/*.c))
examples+=$(patsubst examples/%.cc,%,$(wildcard examples/*.cc))
examples_src:=$(wildcard examples/*.c examples/*.cc)

# examples:
# in examples/
# nope:can be simple file: foo.{c,cc,s}, then compiles to build/<config>/examples/foo, c,cc: must have a main
# only: can be folder for more complex, but folder must have a (main|liba|libso).{c,cc,s}?
# will be given project as deps. e.g. -Isrc/
# $1 = example name $2 = config
define mk_example

# clean += $(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).c,$(examples_src)))
#clean += $(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).c,$(examples_src)))

clean += $(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).c,$(examples_src)))
#$(if $(filter examples/$(1).c,$(examples_src)),$(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).c,$(examples_src))): CPPFLAGS += $$($(2)_CPPFLAGS) $(foreach m,$(modls),-I$(m)/src))
$(if $(filter examples/$(1).c,$(examples_src)), $(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).c,$(examples_src))): CPPFLAGS += $$($(2)_CPPFLAGS) -Isrc/)
$(if $(filter examples/$(1).c,$(examples_src)), $(patsubst examples/$(1).c,build/$(2)/dep/examples/$(1).d,$(filter examples/$(1).c,$(examples_src))): CPPFLAGS += $$($(2)_CPPFLAGS) -Isrc/)
$(if $(filter examples/$(1).c,$(examples_src)), $(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).c,$(examples_src))): CFLAGS += $$($(2)_CFLAGS))
$(if $(filter examples/$(1).c,$(examples_src)), $(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).c,$(examples_src))): CXXFLAGS += $$($(2)_CXXFLAGS))
$(if $(filter examples/$(1).c,$(examples_src)), $(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).c,$(examples_src))): build/$(2)/obj/examples/%.o: examples/%.c | build/$(2)/obj/examples; $$(COMPILE.c) -o $$@ $$<)
#$(if $(filter examples/$(1).c,$(examples_src)),$(patsubst examples/$(1).c,build/$(2)/dep/examples/$(1).d,$(filter examples/$(1).c,$(examples_src))): CPPFLAGS +=  $($(2)_CPPFLAGS) $(foreach m,$(modls),-I$(m)/src))
$(if $(filter examples/$(1).c,$(examples_src)), $(patsubst examples/$(1).c,build/$(2)/dep/examples/$(1).d,$(filter examples/$(1).c,$(examples_src))): build/$(2)/dep/examples/%.d: examples/%.c | build/$(2)/dep/examples; $$(COMPILE.cdep) -MT $$(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$$<) -o $$@ $$<)
$(if $(filter examples/$(1).c,$(examples_src)), clean += $(patsubst examples/$(1).c,build/$(2)/dep/examples/$(1).d,$(filter examples/$(1).c,$(examples_src))))
#$(if $(filter $(1)/src/%.c,$(src)),dep += $(patsubst $(1)/src/%.c,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.c,$(src))))
#$(if $(filter src/%.c,$(src)), dep += $(patsubst src/%.c,build/$(2)/dep/$(1)/%.d,$(filter src/%.c,$(src))))
#$(if $(filter examples/$(1).c,$(examples_src)), dep += build/$(2)/dep/examples/$(1).d)
$(if $(filter examples/$(1).c,$(examples_src)), build/$(2)/examples/$(1): LDFLAGS += $($(2)_LDFLAGS))
$(if $(filter examples/$(1).c,$(examples_src)), build/$(2)/examples/$(1): LDLIBS += $($(2)_LDLIBS))
$(if $(filter examples/$(1).c,$(examples_src)), build/$(2)/examples/$(1): $(patsubst examples/$(1).c,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).c,$(examples_src))) | build/$(2)/examples; $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
#$(if $(filter examples/$(1).c,$(examples_src)),build/$(2)/examples/$(1): $(foreach m,$(modls),$(filter %.a %.so,$($(m)_targ))))
$(if $(filter examples/$(1).c,$(examples_src)), build/$(2)/examples/$(1): $(filter %.a %.so,$($(2)_$(name)_targ)))
$(if $(filter examples/$(1).c,$(examples_src)), examples: build/$(2)/examples/$(1))
$(if $(filter examples/$(1).c,$(examples_src)), clean += build/$(2)/examples/$(1))

$(if $(filter examples/$(1).c,$(examples_src)), build/$(2)/examples/$(1).s: build/$(2)/examples/$(1); $(OBJDUMP) -Sr $$< > $$@)
$(if $(filter examples/$(1).c,$(examples_src)), examples: build/$(2)/examples/$(1).s)
$(if $(filter examples/$(1).c,$(examples_src)), clean += build/$(2)/examples/$(1).s)


# clean += $(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).cc,$(examples_src)))
clean += $(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).cc,$(examples_src)))
# $(if $(filter examples/$(1).cc,$(examples_src)),$(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).cc,$(examples_src))): CPPFLAGS +=  $($(2)_CPPFLAGS)  $(foreach m,$(modls),-I$(m)/src))
$(if $(filter examples/$(1).cc,$(examples_src)), $(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).cc,$(examples_src))): CPPFLAGS += $($(2)_CPPFLAGS) -Isrc)
$(if $(filter examples/$(1).cc,$(examples_src)), $(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).cc,$(examples_src))): CFLAGS +=  $($(2)_CFLAGS))
$(if $(filter examples/$(1).cc,$(examples_src)), $(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).cc,$(examples_src))): CXXFLAGS +=  $($(2)_CXXFLAGS))
$(if $(filter examples/$(1).cc,$(examples_src)), $(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).cc,$(examples_src))): build/$(2)/obj/examples/%.o: examples/%.cc | build/$(2)/obj/examples; $$(COMPILE.cc) -o $$@ $$<)
$(if $(filter examples/$(1).cc,$(examples_src)), clean += $(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).cc,$(examples_src))))
#$(if $(filter examples/$(1).cc,$(examples_src)),$(patsubst examples/$(1).cc,build/$(2)/dep/examples/$(1).d,$(filter examples/$(1).cc,$(examples_src))): CPPFLAGS +=  $($(2)_CPPFLAGS) $(foreach m,$(modls),-I$(m)/src))
$(if $(filter examples/$(1).cc,$(examples_src)), $(patsubst examples/$(1).cc,build/$(2)/dep/examples/$(1).d,$(filter examples/$(1).cc,$(examples_src))): CPPFLAGS +=  $($(2)_CPPFLAGS) -Isrc)
$(if $(filter examples/$(1).cc,$(examples_src)), $(patsubst examples/$(1).cc,build/$(2)/dep/examples/$(1).d,$(filter examples/$(1).cc,$(examples_src))): build/$(2)/dep/examples/%.d: examples/%.cc | build/$(2)/dep/examples; $$(COMPILE.ccdep) -MT $$(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$$<) -o $$@ $$<)
$(if $(filter examples/$(1).cc,$(examples_src)), clean += $(patsubst examples/$(1).cc,build/$(2)/dep/examples/$(1).d,$(filter examples/$(1).cc,$(examples_src))))
#$(if $(filter $(1)/src/%.cc,$(src)),dep += $(patsubst $(1)/src/%.cc,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.cc,$(src))))
#$(if $(filter src/%.cc,$(src)),dep += $(patsubst src/%.cc,build/$(2)/dep/$(1)/%.d,$(filter src/%.cc,$(src))))
#$(if $(filter $(1)/src/%.s,$(src)),dep +=$(patsubst $(1)/src/%.s,build/$(2)/dep/$(1)/%.d,$(filter $(1)/src/%.s,$(src))))
#$(if $(filter src/%.s,$(src)),dep +=$(patsubst src/%.s,build/$(2)/dep/$(1)/%.d,$(filter src/%.s,$(src))))
#$(if $(filter examples/$(1).cc,$(examples_src)),dep += build/$(2)/dep/examples/$(1).d)
$(if $(filter examples/$(1).cc,$(examples_src)), build/$(2)/examples/$(1): LDFLAGS += $($(2)_LDFLAGS))
$(if $(filter examples/$(1).cc,$(examples_src)), build/$(2)/examples/$(1): LDLIBS += $($(2)_LDLIBS))
$(if $(filter examples/$(1).cc,$(examples_src)), build/$(2)/examples/$(1): LINK = $(CXX))
$(if $(filter examples/$(1).cc,$(examples_src)), build/$(2)/examples/$(1): $(patsubst examples/$(1).cc,build/$(2)/obj/examples/$(1).o,$(filter examples/$(1).cc,$(examples_src))) | build/$(2)/examples; $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
#$(if $(filter examples/$(1).cc,$(examples_src)), build/$(2)/examples/$(1):  $(foreach m,$(modls),$(filter %.a %.so,$($(m)_targ))))
$(if $(filter examples/$(1).cc,$(examples_src)), build/$(2)/examples/$(1):  $(filter %.a %.so,$($(name)_targ)))
$(if $(filter examples/$(1).cc,$(examples_src)), examples: build/$(2)/examples/$(1))
$(if $(filter examples/$(1).cc,$(examples_src)), clean += build/$(2)/examples/$(1))

$(if $(filter examples/$(1).cc,$(examples_src)),build/$(2)/examples/$(1).s: build/$(2)/examples/$(1); $(OBJDUMP) -Sr $$< > $$@)
$(if $(filter examples/$(1).cc,$(examples_src)),examples: build/$(2)/examples/$(1).s)
$(if $(filter examples/$(1).cc,$(examples_src)),clean += build/$(2)/examples/$(1).s)

endef


$(foreach c,$(configs),build/$(c)/obj/examples build/$(c)/dep/examples build/$(c)/examples):
	mkdir -p $@

$(foreach e,$(examples),$(foreach c,$(configs),$(eval $(call mk_example,$(e),$(c)))))

# tests
# code tests in src/path/to/test_%.{c,cc.s}
# build into test app in build/<config>/tests/path/to/test_%
# use testing framework: check
# main func in each file (bleugh, copying)

# $2= config
# $1 = modle name
define mk_modl_tests

# clean+=$(patsubst $(1)/src/test_%.c,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.c,$(testsrc)))
# clean+=$(patsubst $(1)/src/test_%.cc,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.cc,$(testsrc)))
# clean+=$(patsubst $(1)/src/test_%.s,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.s,$(testsrc)))

clean += $(patsubst src/test_%.c,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.c,$(testsrc)))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.c,$(testsrc))): | build/$(2)/obj/tests)
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.c,$(testsrc))): | build/$(2)/dep/tests)
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.c,$(testsrc))): CPPFLAGS+=$$($(2)_CPPFLAGS))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.c,$(testsrc))): CPPFLAGS+=$$($(2)_CPPFLAGS))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.c,$(testsrc))): CFLAGS+=$$($(2)_CFLAGS) $(shell pkg-config --cflags check))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.c,$(testsrc))): CFLAGS+=$$($(2)_CFLAGS) $(shell pkg-config --cflags check))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.c,$(testsrc))): build/$(2)/obj/tests/test_%.o: src/test_%.c ;$$(COMPILE.c) -o $$@  $$<)
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.c,$(testsrc))): build/$(2)/dep/tests/test_%.d: src/test_%.c ;$$(COMPILE.cdep) -MT $$(patsubst src/test_%.c,build/$(2)/obj/tests/test_%.o,$$<) -o $$@ $$<)
$(if $(filter src/test_%.c,$(testsrc)), dep += $(patsubst src/test_%.c,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.c,$(testsrc))))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/tests/test_%,$(filter src/test_%.c,$(testsrc))): $(filter %.a %.so,$($(2)_$(name)_targ)))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/tests/test_%,$(filter src/test_%.c,$(testsrc))): | build/$(2)/tests)
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/tests/test_%,$(filter src/test_%.c,$(testsrc))): LDFLAGS+= $($(2)_LDFLAGS) $(shell pkg-config --static check))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/tests/test_%,$(filter src/test_%.c,$(testsrc))): LDLIBS+= $($(2)_LDLIBS) $(shell pkg-config --libs check))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/tests/test_%,$(filter src/test_%.c,$(testsrc))): LDLIBS+= $($(2)_LDLIBS) $(shell pkg-config --libs check))
$(if $(filter src/test_%.c,$(testsrc)), $(patsubst src/test_%.c,build/$(2)/tests/test_%,$(filter src/test_%.c,$(testsrc))): $(patsubst src/test_%.c,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.c,$(testsrc))); $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
$(if $(filter src/test_%.c,$(testsrc)), rtests+= $(patsubst src/test_%.c,build/$(2)/tests/test_%,$(filter src/test_%.c,$(testsrc))))
$(if $(filter src/test_%.c,$(testsrc)), clean += $(patsubst src/test_%.c,build/$(2)/tests/test_%,$(filter src/test_%.c,$(testsrc))))

clean += $(patsubst src/test_%.cc,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.cc,$(testsrc)))
$(if $(filter src/test_%.cc,$(testsrc)), $(patsubst src/test_%.cc,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.cc,$(testsrc))): | build/$(2)/obj/tests)
$(if $(filter src/test_%.cc,$(testsrc)), $(patsubst src/test_%.cc,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.cc,$(testsrc))): | build/$(2)/dep/tests)
$(if $(filter src/test_%.cc,$(testsrc)), $(patsubst src/test_%.cc,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.cc,$(testsrc))): CPPFLAGS+=$$($(2)_CPPFLAGS))
$(if $(filter src/test_%.cc,$(testsrc)), $(patsubst src/test_%.cc,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.cc,$(testsrc))): CPPFLAGS+=$$($(2)_CPPFLAGS))
$(if $(filter src/test_%.cc,$(testsrc)), $(patsubst src/test_%.cc,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.cc,$(testsrc))): CXXFLAGS+=$$($(2)_CXXFLAGS) $(shell pkg-config --cflags catch2))
$(if $(filter src/test_%.cc,$(testsrc)), $(patsubst src/test_%.cc,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.cc,$(testsrc))): CXXFLAGS+=$$($(2)_CXXFLAGS) $(shell pkg-config --cflags catch2))
$(if $(filter src/test_%.cc,$(testsrc)), $(patsubst src/test_%.cc,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.cc,$(testsrc))): build/$(2)/obj/tests/test_%.o: src/test_%.cc ;$$(COMPILE.cc) -o $$@  $$<)
$(if $(filter src/test_%.cc,$(testsrc)), $(patsubst src/test_%.cc,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.cc,$(testsrc))): build/$(2)/dep/tests/test_%.d: src/test_%.cc ;$$(COMPILE.ccdep) -MT $$(patsubst src/test_%.cc,build/$(2)/obj/tests/test_%.o,$$<) -o $$@  $$<)
$(if $(filter src/test_%.cc,$(testsrc)), dep += $(patsubst src/test_%.cc,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.cc,$(testsrc))))
$(if $(filter src/test_%.cc,$(testsrc)), build/$(2)/tests/test_$(name): $(filter %.a %.so,$($(2)_$(name)_targ)))
$(if $(filter src/test_main.cc,$(testsrc)), build/$(2)/tests/test_$(name): | build/$(2)/tests)
$(if $(filter src/test_main.cc,$(testsrc)), build/$(2)/tests/test_$(name): LDFLAGS+=$$($(2)_LDFLAGS))
$(if $(filter src/test_main.cc,$(testsrc)), build/$(2)/tests/test_$(name): LDLIBS+=$$($(2)_LDLIBS) $(shell pkg-config --libs catch2))
$(if $(filter src/test_main.cc,$(testsrc)), build/$(2)/tests/test_$(name): LINK = $$(CXX))
$(if $(filter src/test_main.cc,$(testsrc)), build/$(2)/tests/test_$(name): $(patsubst src/test_%.cc,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.cc,$(testsrc))); $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
$(if $(filter src/test_main.cc,$(testsrc)), rtests+= build/$(2)/tests/test_$(name))
$(if $(filter src/test_main.cc,$(testsrc)), clean += build/$(2)/tests/test_$(name))

clean += $(patsubst src/test_%.s,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.s,$(testsrc)))
$(if $(filter src/test_%.s,$(testsrc)), $(patsubst src/test_%.s,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.s,$(testsrc))): | build/$(2)/obj/tests)
$(if $(filter src/test_%.s,$(testsrc)), $(patsubst src/test_%.s,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.s,$(testsrc))): | build/$(2)/dep/tests)
$(if $(filter src/test_%.s,$(testsrc)), $(patsubst src/test_%.s,build/$(2)/obj/tests/test_%.o,$(filter src/test_%.s,$(testsrc))): build/$(2)/obj/tests/test_%.o: src/test_%.s ;$$(COMPILE.s) -o $$@  $$<)
$(if $(filter src/test_%.s,$(testsrc)), $(patsubst src/test_%.s,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.s,$(testsrc))): build/$(2)/dep/test/%.d: src/test_%.s ;$$(COMPILE.s) -MM -MT $$(patsubst src/test_%.s,build/$(2)/obj/tests/test_%.o,$$<) -o $$@  $$<)
$(if $(filter src/test_%.s,$(testsrc)), dep +=$(patsubst src/test_%.s,build/$(2)/dep/tests/test_%.d,$(filter src/test_%.s,$(testsrc))))
$(if $(filter src/test_%.s,$(testsrc)), build/$(2)/tests/test_$(name): $(filter %.a %.so,$($(name)_targ)))
endef


# $(if $(filter $(1)/src/test_%.c,$(testsrc)),$(patsubst $(1)/src/test_%.c,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.c,$(testsrc))): | build/$(2)/obj/tests/$(1))
# $(if $(filter $(1)/src/test_%.cc,$(testsrc)),$(patsubst $(1)/src/test_%.cc,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.cc,$(testsrc))): | build/$(2)/obj/tests/$(1))
# $(if $(filter $(1)/src/test_%.s,$(testsrc)),$(patsubst $(1)/src/test_%.s,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.s,$(testsrc))): | build/$(2)/obj/tests/$(1))


# $(if $(filter $(1)/src/test_%.c,$(testsrc)),$(patsubst $(1)/src/test_%.c,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.c,$(testsrc))): | build/$(2)/dep/tests/$(1))
# $(if $(filter $(1)/src/test_%.cc,$(testsrc)),$(patsubst $(1)/src/test_%.cc,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.cc,$(testsrc))): | build/$(2)/dep/tests/$(1))
# $(if $(filter $(1)/src/test_%.s,$(testsrc)),$(patsubst $(1)/src/test_%.s,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.s,$(testsrc))): | build/$(2)/dep/tests/$(1))

# $(if $(filter $(1)/src/test_%.c,$(testsrc)),$(patsubst $(1)/src/test_%.c,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.c,$(testsrc))): CPPFLAGS:=$$(CPPFLAGS) $$($(1)_CPPFLAGS) $$($(2)_CPPFLAGS) $$($(1)_$(2)_CPPFLAGS) $(foreach m,$($(1)_DEPS),-I$(m)/src))
# $(if $(filter $(1)/src/test_%.cc,$(testsrc)),$(patsubst $(1)/src/test_%.cc,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.cc,$(testsrc))): CPPFLAGS:=$$(CPPFLAGS) $$($(1)_CPPFLAGS) $$($(2)_CPPFLAGS) $$($(1)_$(2)_CPPFLAGS) $(shell pkg-config --cflags catch2) $(foreach m,$($(1)_DEPS),-I$(m)/src))

# $(if $(filter $(1)/src/test_%.c,$(testsrc)),$(patsubst $(1)/src/test_%.c,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.c,$(testsrc))): CPPFLAGS:=$$(CPPFLAGS) $$($(1)_CPPFLAGS) $$($(2)_CPPFLAGS) $$($(1)_$(2)_CPPFLAGS) $(foreach m,$($(1)_DEPS),-I$(m)/src))
# $(if $(filter $(1)/src/test_%.c,$(testsrc)),$(patsubst $(1)/src/test_%.c,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.c,$(testsrc))): CFLAGS:=$$(CFLAGS) $$($(1)_CFLAGS) $$($(2)_CFLAGS) $$($(1)_$(2)_CFLAGS))

# $(if $(filter $(1)/src/test_%.cc,$(testsrc)),$(patsubst $(1)/src/test_%.cc,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.cc,$(testsrc))): CPPFLAGS:=$$(CPPFLAGS) $$($(1)_CPPFLAGS) $$($(2)_CPPFLAGS) $$($(1)_$(2)_CPPFLAGS) $(shell pkg-config --cflags catch2) $(foreach m,$($(1)_DEPS),-I$(m)/src))
# $(if $(filter $(1)/src/test_%.cc,$(testsrc)),$(patsubst $(1)/src/test_%.cc,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.cc,$(testsrc))): CXXFLAGS:=$$(CXXFLAGS) $$($(1)_CXXFLAGS) $$($(2)_CXXFLAGS) $$($(1)_$(2)_CXXFLAGS))

# $(if $(filter $(1)/src/test_%.c,$(testsrc)),$(patsubst $(1)/src/test_%.c,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.c,$(testsrc))): build/$(2)/obj/tests/$(1)/test_%.o: $(1)/src/test_%.c ;$$(COMPILE.c) -o $$@  $$<)
# $(if $(filter $(1)/src/test_%.cc,$(testsrc)),$(patsubst $(1)/src/test_%.cc,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.cc,$(testsrc))): build/$(2)/obj/tests/$(1)/test_%.o: $(1)/src/test_%.cc ;$$(COMPILE.cc) -o $$@  $$<)
# $(if $(filter $(1)/src/test_%.s,$(testsrc)),$(patsubst $(1)/src/test_%.s,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.s,$(testsrc))): build/$(2)/obj/tests/$(1)/test_%.o: $(1)/src/test_%.s ;$$(COMPILE.s) -o $$@  $$<)

# $(if $(filter $(1)/src/test_%.c,$(testsrc)),$(patsubst $(1)/src/test_%.c,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.c,$(testsrc))): build/$(2)/dep/tests/$(1)/test_%.d: $(1)/src/test_%.c ;$$(COMPILE.c) -MM -MT $$(patsubst $(1)/src/test_%.c,build/$(2)/obj/tests/$(1)/test_%.o,$$<) -o $$@ $$<)
# $(if $(filter $(1)/src/test_%.cc,$(testsrc)),$(patsubst $(1)/src/test_%.cc,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.cc,$(testsrc))): build/$(2)/dep/tests/$(1)/test_%.d: $(1)/src/test_%.cc ;$$(COMPILE.cc) -MM -MT $$(patsubst $(1)/src/test_%.cc,build/$(2)/obj/tests/$(1)/test_%.o,$$<) -o $$@  $$<)
# $(if $(filter $(1)/src/test_%.s,$(testsrc)),$(patsubst $(1)/src/test_%.s,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.s,$(testsrc))): build/$(2)/dep/$(1)/test/%.d: $(1)/src/test_%.s ;$$(COMPILE.s) -MM -MT $$(patsubst $(1)/src/test_%.s,build/$(2)/obj/tests/$(1)/test_%.o,$$<) -o $$@  $$<)

# $(if $(filter $(1)/src/test_%.c,$(testsrc)),dep += $(patsubst $(1)/src/test_%.c,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.c,$(testsrc))))
# $(if $(filter $(1)/src/test_%.cc,$(testsrc)),dep += $(patsubst $(1)/src/test_%.cc,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.cc,$(testsrc))))
# $(if $(filter $(1)/src/test_%.s,$(testsrc)),dep +=$(patsubst $(1)/src/test_%.s,build/$(2)/dep/tests/$(1)/test_%.d,$(filter $(1)/src/test_%.s,$(testsrc))))

# $(if $(filter $(1)/src/test_%.s,$(testsrc)),build/$(2)/tests/test_$(1): $(foreach m,$(modls),$(filter %.a %.so,$($(m)_targ))))
# $(if $(filter $(1)/src/test_%.c,$(testsrc)),build/$(2)/tests/test_$(1): $(foreach m,$(modls),$(filter %.a %.so,$($(m)_targ))))
# $(if $(filter $(1)/src/test_%.cc,$(testsrc)),build/$(2)/tests/test_$(1): $(foreach m,$(modls),$(filter %.a %.so,$($(m)_targ))))

# $(if $(filter $(1)/src/test_main.c,$(testsrc)),build/$(2)/tests/test_$(1): | build/$(2)/tests)
# $(if $(filter $(1)/src/test_main.c,$(testsrc)),build/$(2)/tests/test_$(1): LDFLAGS += $($(2)_LDFLAGS))
# $(if $(filter $(1)/src/test_main.c,$(testsrc)),build/$(2)/tests/test_$(1): LDLIBS += $($(2)_LDLIBS))
# $(if $(filter $(1)/src/test_main.c,$(testsrc)),build/$(2)/tests/test_$(1): $(patsubst $(1)/src/test_%.c,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.c,$(testsrc))); $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
# $(if $(filter $(1)/src/test_main.c,$(testsrc)),rtests+= build/$(2)/tests/test_$(1))
# $(if $(filter $(1)/src/test_main.c,$(testsrc)),clean += build/$(2)/tests/test_$(1))

# $(if $(filter $(1)/src/test_main.cc,$(testsrc)),build/$(2)/tests/test_$(1): | build/$(2)/tests)
# $(if $(filter $(1)/src/test_main.cc,$(testsrc)),build/$(2)/tests/test_$(1): LDFLAGS += $($(2)_LDFLAGS))
# $(if $(filter $(1)/src/test_main.cc,$(testsrc)),build/$(2)/tests/test_$(1): LDLIBS += $($(2)_LDLIBS))
# $(if $(filter $(1)/src/test_main.cc,$(testsrc)),build/$(2)/tests/test_$(1): LINK = $(CXX))
# $(if $(filter $(1)/src/test_main.cc,$(testsrc)),build/$(2)/tests/test_$(1): $(patsubst $(1)/src/test_%.cc,build/$(2)/obj/tests/$(1)/test_%.o,$(filter $(1)/src/test_%.cc,$(testsrc))); $$(LINK.o) $$^ $$(LOADLIBES) $$(LDLIBS) -o $$@)
# $(if $(filter $(1)/src/test_main.cc,$(testsrc)),rtests+= build/$(2)/tests/test_$(1) )
# $(if $(filter $(1)/src/test_main.cc,$(testsrc)),clean += build/$(2)/tests/test_$(1))

# endef


$(foreach c,$(configs),build/$(c)/obj/tests build/$(c)/dep/tests build/$(c)/tests):
	mkdir -p $@

$(foreach c,$(configs),$(eval $(call mk_modl_tests,fack,$(c))))



.PHONY: build
build:

.PHONY: examples
examples:

.PHONY: clean
clean:
	$(RM) $(clean)
	find . -name '*.orig' -delete


.PNONY: $(addprefix run_,$(rtests))
$(foreach d,$(rtests),$(eval run_$(d): $(d)))

$(addprefix run_,$(rtests)):
	$(patsubst run_%,%,$@) -b



.PHONY: test tests
tests: $(addprefix run_,$(rtests))
test: tests

distclean:
	$(RM) -r build

# clang-format does not format class/struct defs how I like them
# with access spec 1 indent, and members 1 more indent after that
# currently puts access spec on same level as members..
# but then I want a full reformatter (space between functions, members etc.. managed as well).
.PHONY: format
format:
	astyle --project -I -n  $(allsrc) $(allhdr)
	#env -u NIX_CFLAGS_COMPILE clang-format -style=file -i $(allsrc) $(allhdr)

# make deps if clean or distclean not given
# and should be the only target given
ifeq ($(MAKECMDGOALS),clean)
else ifeq ($(MAKECMDGOALS),distclean)
else
-include $(dep)
endif
