
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "id.hpp"

class NSString;

class NSObject
{
protected:
    id data;

    static id alloc(const char* s);

public:
    id _get_id();

    NSObject(id data);

    static NSObject alloc();
    static id       clazz();

    id   init();
    id   clasz();
    id   copy();
    id   mutableCopy();
    void dealloc();

    NSString description() const;
};

std::basic_ostream<char, std::char_traits<char>>& operator<<(std::basic_ostream<char, std::char_traits<char>>& os,
                                                             const NSObject&                                   object);