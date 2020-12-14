
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

    template<typename T>
    struct NSCopying : public Protocol
    {
        const auto copy() const;
    };

    template<typename T>
    struct NSMutableCopying : public Protocol
    {
        auto mutableCopy();
    };

    template<typename... A>
    struct NSObject : public id<NSObjectProtocol, A...>
    {
        using super = id<NSObjectProtocol, A...>;

        NSObject();
        ~NSObject();

    protected:
        NSObject(Class clazz);
    };
} // namespace macxx
