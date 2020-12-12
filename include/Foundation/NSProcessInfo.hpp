
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "NSObject.hpp"
#include "NSString.hpp"

class NSProcessInfo : public NSObject
{
public:
    NSProcessInfo(id data);

    static NSProcessInfo alloc();
    static id            clazz();
    static NSProcessInfo processInfo();

    NSString processName();
    int processIdentifier();
    bool iOSAppOnMac();
    bool macCatalystApp();
    NSString globallyUniqueString();
};