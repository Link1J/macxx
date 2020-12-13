
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "base.hpp"
#include <stddef.h>
#include <stdint.h>
#include <type_traits>

extern "C" void*       objc_msgSend(void* self, const char* _cmd, ...);
extern "C" void*       objc_alloc(void* clazz);
extern "C" void*       objc_getClass(const char* name);
extern "C" void*       object_getClass(void* obj);
extern "C" void*       class_getMethodImplementation(void* clazz, const char* name);
extern "C" const char* sel_getName(const char* sel);
extern "C" bool        sel_isMapped(const char* sel);
extern "C" const char* sel_getUid(const char* str);
extern "C" void        objc_msgSend_stret(void* stretAddr, void* self, const char* _cmd, ...);
extern "C" double      objc_msgSend_fpret(void* self, const char* _cmd, ...);

template<typename R, typename... A>
R objc_send_msg(id self, std::string cmd, A... args)
{
    auto _cmd = sel_getUid(cmd.c_str());
    if constexpr (std::is_pod_v<R>)
    {
        R return_value;
        objc_msgSend_stret(&return_value, self._get_abi(), _cmd, args...);
        return return_value;
    }
    else if constexpr (std::is_floating_point_v<R>)
    {
        return ((R(*)(void*, const char*, A...))(objc_msgSend_fpret))(self._get_abi(), _cmd, args...);
    }
    else if constexpr (std::is_integral_v<R>)
    {
        return ((R(*)(void*, const char*, A...))(objc_msgSend))(self._get_abi(), _cmd, args...);
    }
    else
    {
        return id{objc_msgSend(self._get_abi(), _cmd, args...)};
    }
}