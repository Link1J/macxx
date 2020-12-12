
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "NSObject.hpp"
#include <ostream>
#include <string>
#include <type_traits>

enum NSStringEncoding
{
    NSASCIIStringEncoding         = 1,
    NSNEXTSTEPStringEncoding      = 2,
    NSJapaneseEUCStringEncoding   = 3,
    NSUTF8StringEncoding          = 4,
    NSISOLatin1StringEncoding     = 5,
    NSSymbolStringEncoding        = 6,
    NSNonLossyASCIIStringEncoding = 7,
    NSShiftJISStringEncoding      = 8,
    NSISOLatin2StringEncoding     = 9,
    NSUnicodeStringEncoding       = 10,
    NSWindowsCP1251StringEncoding = 11,
    NSWindowsCP1252StringEncoding = 12,
    NSWindowsCP1253StringEncoding = 13,
    NSWindowsCP1254StringEncoding = 14,
    NSWindowsCP1250StringEncoding = 15,
    NSISO2022JPStringEncoding     = 21,
};

class NSString : public NSObject
{
public:
    NSString(id data);

    static NSString alloc();
    static id       clazz();

    id initWithUTF8String(const char* nullterminatedCString);
    id initWithBytes(const void* bytes, unsigned long length, NSStringEncoding encoding);

    const char* cStringUsingEncoding(NSStringEncoding encoding) const;
};