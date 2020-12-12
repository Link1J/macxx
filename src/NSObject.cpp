
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "NSObject.hpp"
#include "Foundation/NSString.hpp"
#include "base.hpp"

id NSObject::alloc(const char* s)
{
    return id{objc_msgSend(objc_getClass(s), sel_getUid("alloc"))};
}

NSObject NSObject::alloc()
{
    return alloc("NSObject");
}

id NSObject::clazz()
{
    return id{objc_getClass("NSObject")};
}

id NSObject::clasz()
{
    return id{object_getClass(data._get_abi())};
}

NSObject::NSObject(id data)
    : data(data)
{}

id NSObject::init()
{
    return id{objc_msgSend(data._get_abi(), sel_getUid("init"))};
}

NSString NSObject::description() const
{
    return id{objc_msgSend(data._get_abi(), sel_getUid("description"))};
}

std::basic_ostream<char, std::char_traits<char>>& operator<<(std::basic_ostream<char, std::char_traits<char>>& os,
                                                             const NSObject&                                   object)
{
    return os << object.description().cStringUsingEncoding(NSUTF8StringEncoding);
}

id NSObject::copy()
{
    return id{objc_msgSend(data._get_abi(), sel_getUid("copy"))};
}

id NSObject::mutableCopy()
{
    return id{objc_msgSend(data._get_abi(), sel_getUid("mutableCopy"))};
}

// These Two Functions are here on purpose, do not move them to NSString

NSString::NSString(id data)
    : NSObject(data)
{}

const char* NSString::cStringUsingEncoding(NSStringEncoding encoding) const
{
    return (const char*)objc_msgSend(data._get_abi(), sel_getUid("cStringUsingEncoding:"), encoding);
}