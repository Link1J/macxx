
// Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "NSProcessInfo.hpp"

namespace macxx
{
    auto NSProcessInfo::processInfo()
    {
        auto clazz = get_class<NSProcessInfo>();
        return objc_send_msg<NSProcessInfo>(clazz, "processInfo");
    }

    auto NSProcessInfo::processName()
    {
        return objc_send_msg<NSString>(*this, "processName");
    }

    auto NSProcessInfo::processIdentifier()
    {
        return objc_send_msg<NSInteger>(*this, "processIdentifier");
    }

    auto NSProcessInfo::iOSAppOnMac()
    {
        return objc_send_msg<BOOL>(*this, "isiOSAppOnMac");
    }

    auto NSProcessInfo::macCatalystApp()
    {
        return objc_send_msg<BOOL>(*this, "isMacCatalystApp");
    }

    auto NSProcessInfo::globallyUniqueString()
    {
        return objc_send_msg<NSString>(*this, "globallyUniqueString");
    }

    auto NSProcessInfo::userName()
    {
        return objc_send_msg<NSString>(*this, "userName");
    }

    auto NSProcessInfo::fullUserName()
    {
        return objc_send_msg<NSString>(*this, "fullUserName");
    }

    auto NSProcessInfo::automaticTerminationSupportEnabled()
    {
        return objc_send_msg<BOOL>(*this, "automaticTerminationSupportEnabled");
    }

    auto NSProcessInfo::disableSuddenTermination()
    {
        return objc_send_msg<void>(*this, "disableSuddenTermination");
    }

    auto NSProcessInfo::enableSuddenTermination()
    {
        return objc_send_msg<void>(*this, "enableSuddenTermination");
    }

    auto NSProcessInfo::disableAutomaticTermination(NSString reason)
    {
        objc_send_msg<void>(*this, "disableAutomaticTermination:", reason);
    }

    auto NSProcessInfo::enableAutomaticTermination(NSString reason)
    {
        objc_send_msg<void>(*this, "enableAutomaticTermination:", reason);
    }

    auto NSProcessInfo::isOperatingSystemAtLeastVersion(NSOperatingSystemVersion version)
    {
        return objc_send_msg<BOOL>(*this, "isOperatingSystemAtLeastVersion:", version);
    }

    auto NSProcessInfo::hostName()
    {
        return objc_send_msg<NSString>(*this, "hostName");
    }

#if MACXX_API_DEPRECTED_FROM(__MAC_10_10, __IPHONE_8_0, __MAC_13_0, __TVOS_9_0, __WATCHOS_2_0)
    auto NSProcessInfo::operatingSystem()
    {
        return objc_send_msg<unsigned long>(*this, "operatingSystem");
    }

    auto NSProcessInfo::operatingSystemName()
    {
        return objc_send_msg<NSString>(*this, "operatingSystemName");
    }
#endif

    auto NSProcessInfo::operatingSystemVersionString()
    {
        return objc_send_msg<NSString>(*this, "operatingSystemVersionString");
    }

    auto NSProcessInfo::operatingSystemVersion()
    {
        return objc_send_msg<NSOperatingSystemVersion>(*this, "operatingSystemVersion");
    }

    auto NSProcessInfo::processorCount()
    {
        return objc_send_msg<NSUInteger>(*this, "processorCount");
    }

    auto NSProcessInfo::activeProcessorCount()
    {
        return objc_send_msg<NSUInteger>(*this, "activeProcessorCount");
    }

    auto NSProcessInfo::physicalMemory()
    {
        return objc_send_msg<unsigned long long>(*this, "physicalMemory");
    }

    auto NSProcessInfo::systemUptime()
    {
        return objc_send_msg<NSTimeInterval>(*this, "systemUptime");
    }

    auto NSProcessInfo::beginActivityWithOptions(NSActivityOptions options, NSString reason)
    {
        return objc_send_msg<id<>>(*this, "beginActivityWithOptions:reason:", options, reason);
    }

    auto NSProcessInfo::endActivity(id<> activity)
    {
        objc_send_msg<void>(*this, "endActivity:", activity);
    }

    auto NSProcessInfo::performActivityWithOptions(NSActivityOptions options, NSString reason,
                                                   std::function<void(void* block)> block)
    {
        objc_send_msg<void>(*this, "performActivityWithOptions:reason:usingBlock:", options, reason,
                            block.target<int (*)(void* block)>());
    }

    auto NSProcessInfo::performExpiringActivityWithReason(NSString                                       reason,
                                                          std::function<void(void* block, BOOL expired)> block)
    {
        objc_send_msg<void>(*this, "performExpiringActivityWithReason:usingBlock:", reason,
                            block.target<int (*)(void* block, BOOL expired)>());
    }
} // namespace macxx