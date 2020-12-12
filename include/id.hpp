
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <iosfwd>

class id
{
    void* _id;

public:
    id() = default;
    explicit id(void* id);

    void  _set_abi(void* abi);
    void* _get_abi() const;
};

std::basic_ostream<char, std::char_traits<char>>& operator<<(std::basic_ostream<char, std::char_traits<char>>& os,
                                                             const id&                                         object);