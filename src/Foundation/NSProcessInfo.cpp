
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "Foundation/NSProcessInfo.hpp"
#include "base.hpp"

NSProcessInfo NSProcessInfo::alloc()
{
    return NSObject::alloc("NSProcessInfo");
}

id NSProcessInfo::clazz()
{
    return id{objc_getClass("NSProcessInfo")};
}

NSProcessInfo NSProcessInfo::processInfo()
{
    return id{objc_msgSend(clazz()._get_abi(), sel_getUid("processInfo"))};
}

NSProcessInfo::NSProcessInfo(id data)
    : NSObject(data)
{}

NSString NSProcessInfo::processName()
{
    return id{objc_msgSend(data._get_abi(), sel_getUid("processName"))};
}

int NSProcessInfo::processIdentifier()
{
    auto a = objc_msgSend(data._get_abi(), sel_getUid("processIdentifier"));
    return *(int*)&a;
}

bool NSProcessInfo::iOSAppOnMac()
{
    auto a = objc_msgSend(data._get_abi(), sel_getUid("isiOSAppOnMac"));
    return *(bool*)&a;
}

bool NSProcessInfo::macCatalystApp()
{
    auto a = objc_msgSend(data._get_abi(), sel_getUid("isMacCatalystApp"));
    return *(bool*)&a;
}

NSString NSProcessInfo::globallyUniqueString()
{
    return id{objc_msgSend(data._get_abi(), sel_getUid("globallyUniqueString"))};
}