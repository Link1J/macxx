
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "NSObject.hpp"
#include "NSString.hpp"

namespace macxx
{
    auto NSObjectProtocol::hash() const
    {
        return objc_send_msg<NSUInteger>(*this, "hash");
    }

    auto NSObjectProtocol::superclass() const
    {
        return objc_send_msg<Class>(*this, "superclass");
    }

    auto NSObjectProtocol::clasz() const
    {
        return objc_send_msg<Class>(*this, "clasz");
    }

    auto NSObjectProtocol::description() const
    {
        return objc_send_msg<NSString>(*this, "description");
    }

    template<typename T>
    const auto NSCopying<T>::copy() const
    {
        return objc_send_msg<const T>(*this, "copy");
    }

    template<typename T>
    auto NSMutableCopying<T>::mutableCopy()
    {
        return objc_send_msg<T>(*this, "mutableCopy");
    }

    template<typename... A>
    NSObject<A...>::NSObject()
        : NSObject(get_class<NSObject>())
    {
        this->set_abi(objc_send_msg<abi::id>(*this, "init"));
    }

    template<typename... A>
    NSObject<A...>::~NSObject()
    {
        objc_send_msg<void>(*this, "release");
        this->set_abi(nullptr);
    }

    template<typename... A>
    NSObject<A...>::NSObject(Class clazz)
    {
        this->set_abi(alloc<abi::id>(clazz));
    }
} // namespace macxx

bool operator==(const macxx::NSObjectProtocol& lhs, const macxx::NSObjectProtocol& rhs)
{
    return macxx::objc_send_msg<bool>(lhs, "isEqual:", rhs);
}