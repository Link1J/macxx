
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "base.hpp"

namespace macxx
{
    struct NSString;

    struct NSObjectProtocol : public Protocol
    {
        auto description() const;
        auto hash() const;
        auto superclass() const;

        auto clasz() const;
    };

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

    template<typename... A>
    struct NSObject : public id<NSObjectProtocol, A...>
    {
        using super = id<NSObjectProtocol, A...>;

        NSObject()
            : NSObject(get_class<NSObject>())
        {
            this->set_abi(objc_send_msg<abi::id>(*this, "init"));
        }

        ~NSObject()
        {
            //objc_send_msg<void>(*this, "release");
            //this->set_abi(nullptr);
        }

    protected:
        NSObject(Class clazz)
        {
            this->set_abi(alloc<abi::id>(clazz));
        }
    };
} // namespace macxx

bool operator==(const macxx::NSObjectProtocol& lhs, const macxx::NSObjectProtocol& rhs)
{
    return macxx::objc_send_msg<bool>(lhs, "isEqual:", rhs);
}