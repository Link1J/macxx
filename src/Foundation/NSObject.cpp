
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "Foundation/NSObject.hpp"
#include "Foundation/NSString.hpp"
#include "base.hpp"

id NSObject::alloc(const char* s)
{
    return objc_send_msg<id>(id{objc_getClass(s)}, "alloc");
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
    return objc_send_msg<id>(data, "init");
}

NSString NSObject::description() const
{
    return objc_send_msg<id>(data, "description");
}

std::basic_ostream<char, std::char_traits<char>>& operator<<(std::basic_ostream<char, std::char_traits<char>>& os,
                                                             const NSObject&                                   object)
{
    return os << object.description().cStringUsingEncoding(NSUTF8StringEncoding);
}

id NSObject::copy()
{
    return objc_send_msg<id>(data, "copy");
}

id NSObject::mutableCopy()
{
    return objc_send_msg<id>(data, "mutableCopy");
}

id NSObject::_get_id()
{
    return data;
}

void NSObject::dealloc()
{
    objc_send_msg<void>(data, "dealloc");
}

// These Two Functions are here on purpose, do not move them to NSString

NSString::NSString(id data)
    : NSObject(data)
{}

const char* NSString::cStringUsingEncoding(NSStringEncoding encoding) const
{
    return objc_send_msg<const char*>(data, "cStringUsingEncoding:", encoding);
}