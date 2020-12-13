
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "id.hpp"
#include "Foundation/NSObject.hpp"

id::id(void* id)
    : _id(id)
{}

void id::_set_abi(void* abi)
{
    _id = abi;
}

void* id::_get_abi() const
{
    return _id;
}

std::basic_ostream<char, std::char_traits<char>>& operator<<(std::basic_ostream<char, std::char_traits<char>>& os,
                                                             const id&                                         object)
{
    return os << NSObject{object};
}