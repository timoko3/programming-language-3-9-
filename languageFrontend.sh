#!/bin/bash

CXX="g++"
TARGET="front_end.out"
STD="-std=c++17"

SOURCES=(
    main.cpp
    front_end/*.cpp
    general/*.cpp
    general/tree/*.cpp
    general/tokens/*.cpp
    general/devTools/*.cpp
    general/stack/*.cpp
    core/*.cpp
)

INCLUDES="-I. -I./general -I./derivative -I./devTools"

CXXFLAGS=(
    -ggdb3 $STD -O0
    -Wall -Wextra -Waggressive-loop-optimizations -Wc++14-compat 
    -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts 
    -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body 
    -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness 
    -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd 
    -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls 
    -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel 
    -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods 
    -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wsync-nand 
    -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros 
    -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing 
    -Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation 
    -fstack-protector -fstrict-overflow -flto-odr-type-merging 
    -fno-omit-frame-pointer -pie -fPIE -Werror=vla
)

SANITIZERS="-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr"

DEBUG_FLAGS=""
if [ "$1" == "debug" ]; then
    DEBUG_FLAGS="-DDEBUG"
    echo "Building in DEBUG mode..."
fi

$CXX $DEBUG_FLAGS "${CXXFLAGS[@]}" $SANITIZERS "${SOURCES[@]}" $INCLUDES -o $TARGET

if [ $? -eq 0 ]; then
    echo "Successfully built: $TARGET"
else
    echo "Build failed!"
    exit 1
fi
