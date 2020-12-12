
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "Foundation/NSString.hpp"
#include "base.hpp"

NSString NSString::alloc()
{
    return NSObject::alloc("NSString");
}

id NSString::clazz()
{
    return id{objc_getClass("NSString")};
}

id NSString::initWithUTF8String(const char* nullterminatedCString)
{
    return id{objc_msgSend(data._get_abi(), sel_getUid("initWithUTF8String:"), nullterminatedCString)};
}

id NSString::initWithBytes(const void* bytes, unsigned long length, NSStringEncoding encoding)
{
    return id{objc_msgSend(data._get_abi(), sel_getUid("initWithBytes:length:encoding:"), bytes, length, encoding)};
}
