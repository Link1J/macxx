
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <stddef.h>
#include <stdint.h>

using _SEL   = void*;
using _id    = void*;
using _Class = _id;
typedef void* (*_IMP)(...);

extern "C" void*       objc_msgSend(_id self, _SEL _cmd, ...);
extern "C" _id         objc_alloc(_Class clazz);
extern "C" _Class      objc_getClass(const char* name);
extern "C" _Class      object_getClass(_id obj);
extern "C" _IMP        class_getMethodImplementation(_Class clazz, _SEL name);
extern "C" const char* sel_getName(_SEL sel);
extern "C" bool        sel_isMapped(_SEL sel);
extern "C" _SEL        sel_getUid(const char* str);