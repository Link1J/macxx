
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
    return objc_send_msg<NSProcessInfo>(clazz(), "processInfo");
}

NSProcessInfo::NSProcessInfo(id data)
    : NSObject(data)
{}

NSString NSProcessInfo::processName()
{
    return objc_send_msg<NSString>(data, "processName");
}

int NSProcessInfo::processIdentifier()
{
    return objc_send_msg<int>(data, "processIdentifier");
}

bool NSProcessInfo::iOSAppOnMac()
{
    return objc_send_msg<bool>(data, "isiOSAppOnMac");
}

bool NSProcessInfo::macCatalystApp()
{
    return objc_send_msg<bool>(data, "isMacCatalystApp");
}

NSString NSProcessInfo::globallyUniqueString()
{
    return objc_send_msg<NSString>(data, "globallyUniqueString");
}

NSString NSProcessInfo::userName()
{
    return objc_send_msg<NSString>(data, "userName");
}

NSString NSProcessInfo::fullUserName()
{
    return objc_send_msg<NSString>(data, "fullUserName");
}

bool NSProcessInfo::automaticTerminationSupportEnabled()
{
    return objc_send_msg<bool>(data, "automaticTerminationSupportEnabled");
}

void NSProcessInfo::disableSuddenTermination()
{
    return objc_send_msg<void>(data, "disableSuddenTermination");
}

void NSProcessInfo::enableSuddenTermination()
{
    return objc_send_msg<void>(data, "enableSuddenTermination");
}

void NSProcessInfo::disableAutomaticTermination(NSString reason)
{
    return objc_send_msg<void>(data, "disableAutomaticTermination:", reason._get_id()._get_abi());
}

void NSProcessInfo::enableAutomaticTermination(NSString reason)
{
    return objc_send_msg<void>(data, "enableAutomaticTermination:", reason._get_id()._get_abi());
}

unsigned long NSProcessInfo::operatingSystem()
{
    return objc_send_msg<unsigned long>(data, "operatingSystem");
}

NSString NSProcessInfo::operatingSystemName()
{
    return objc_send_msg<NSString>(data, "operatingSystemName");
}

bool NSProcessInfo::isOperatingSystemAtLeastVersion(NSOperatingSystemVersion version)
{
    return objc_send_msg<bool>(data, "isOperatingSystemAtLeastVersion:", version);
}

NSString NSProcessInfo::hostName()
{
    return objc_send_msg<NSString>(data, "hostName");
}

NSString NSProcessInfo::operatingSystemVersionString()
{
    return objc_send_msg<NSString>(data, "operatingSystemVersionString");
}

NSOperatingSystemVersion NSProcessInfo::operatingSystemVersion()
{
    return objc_send_msg<NSOperatingSystemVersion>(data, "operatingSystemVersion");
}

unsigned long NSProcessInfo::processorCount()
{
    return objc_send_msg<unsigned long>(data, "processorCount");
}

unsigned long NSProcessInfo::activeProcessorCount()
{
    return objc_send_msg<unsigned long>(data, "activeProcessorCount");
}

unsigned long long NSProcessInfo::physicalMemory()
{
    return objc_send_msg<unsigned long long>(data, "physicalMemory");
}

double NSProcessInfo::systemUptime()
{
    return objc_send_msg<double>(data, "systemUptime");
}

id NSProcessInfo::beginActivityWithOptions(NSActivityOptions options, NSString reason)
{
    return objc_send_msg<id>(data, "beginActivityWithOptions:reason:", options, reason._get_id()._get_abi());
}

void NSProcessInfo::endActivity(id activity)
{
    objc_send_msg<void>(data, "endActivity:", activity._get_abi());
}

void NSProcessInfo::performActivityWithOptions(NSActivityOptions options, NSString reason,
                                               std::function<void(void* block)> block)
{
    objc_send_msg<void>(data, "performActivityWithOptions:reason:usingBlock:", options, reason._get_id()._get_abi(),
                        block.target<int (*)(void* block)>());
}

void NSProcessInfo::performExpiringActivityWithReason(NSString                                       reason,
                                                      std::function<void(void* block, bool expired)> block)
{
    objc_send_msg<void>(data, "performExpiringActivityWithReason:usingBlock:", reason._get_id()._get_abi(),
                        block.target<int (*)(void* block, bool expired)>());
}