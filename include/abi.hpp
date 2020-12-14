
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>

namespace macxx::abi
{
    using id              = struct objc_object*;
    using SEL             = struct objc_selector*;
    using Class           = struct objc_class*;
    using Protocol        = struct objc_object;
    using Method          = struct objc_method*;
    using Ivar            = struct objc_ivar*;
    using Category        = struct objc_category*;
    using objc_property_t = struct objc_property*;

    extern "C"
    {
        void*  objc_msgSend(id self, SEL _cmd, ...);
        void   objc_msgSend_stret(void* stretAddr, id self, SEL _cmd, ...);
        double objc_msgSend_fpret(id self, SEL _cmd, ...);
        id     objc_alloc(Class clazz);
        id     objc_getClass(const char* name);

        Class object_getClass(id obj);

        const char* sel_getName(SEL sel);
        bool        sel_isMapped(const char* sel);
        SEL         sel_getUid(const char* str);

        const char* class_getName(Class cls);
        Class       class_getSuperclass(Class cls);
    }
} // namespace macxx::abi
