
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "NSObject.hpp"
#include "NSString.hpp"
#include <functional>

namespace macxx
{
    struct NSOperatingSystemVersion
    {
        unsigned long majorVersion;
        unsigned long minorVersion;
        unsigned long patchVersion;
    };

    enum NSProcessInfoThermalState
    {
        NSProcessInfoThermalStateNominal,
        NSProcessInfoThermalStateFair,
        NSProcessInfoThermalStateSerious,
        NSProcessInfoThermalStateCritical,
    };

    enum
    {
        NSWindowsNTOperatingSystem MACXX_DEPRECTED = 1,
        NSWindows95OperatingSystem MACXX_DEPRECTED,
        NSSolarisOperatingSystem   MACXX_DEPRECTED,
        NSHPUXOperatingSystem      MACXX_DEPRECTED,
        NSMACHOperatingSystem      MACXX_DEPRECTED,
        NSSunOSOperatingSystem     MACXX_DEPRECTED,
        NSOSF1OperatingSystem      MACXX_DEPRECTED
    };

    enum NSActivityOptions : uint64_t
    {
        NSActivityIdleDisplaySleepDisabled             = (1ULL << 40),
        NSActivityIdleSystemSleepDisabled              = (1ULL << 20),
        NSActivitySuddenTerminationDisabled            = (1ULL << 14),
        NSActivityAutomaticTerminationDisabled         = (1ULL << 15),
        NSActivityUserInitiated                        = (0x00FFFFFFULL | NSActivityIdleSystemSleepDisabled),
        NSActivityUserInitiatedAllowingIdleSystemSleep = (NSActivityUserInitiated & ~NSActivityIdleSystemSleepDisabled),
        NSActivityBackground                           = 0x000000FFULL,
        NSActivityLatencyCritical                      = 0xFF00000000ULL,
    };

    using NSTimeInterval = double;

    struct NSProcessInfo : public NSObject<>
    {
        NSProcessInfo() = delete;

        static auto processInfo();

        // auto arguments();
        // auto environment();
        auto processName();
        auto processIdentifier();
        auto iOSAppOnMac();
        auto macCatalystApp();
        auto globallyUniqueString();
        auto userName();
        auto fullUserName();
        auto automaticTerminationSupportEnabled();
        auto hostName();
        auto operatingSystemVersionString();
        auto operatingSystemVersion();
        auto processorCount();
        auto activeProcessorCount();
        auto physicalMemory();
        auto systemUptime();
        auto thermalState();
        auto lowPowerModeEnabled();

        auto disableSuddenTermination();
        auto enableSuddenTermination();
        auto disableAutomaticTermination(NSString reason);
        auto enableAutomaticTermination(NSString reason);
        MACXX_DEPRECTED
        auto operatingSystem();
        MACXX_DEPRECTED
        auto operatingSystemName();
        auto isOperatingSystemAtLeastVersion(NSOperatingSystemVersion version);
        auto beginActivityWithOptions(NSActivityOptions options, NSString reason);
        auto endActivity(id<> activity);
        auto performActivityWithOptions(NSActivityOptions options, NSString reason,
                                        std::function<void(void* block)> block);
        auto performExpiringActivityWithReason(NSString reason, std::function<void(void* block, BOOL expired)> block);
    };

    static_assert(sizeof(NSProcessInfo) == sizeof(id<>));

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

    auto NSProcessInfo::operatingSystem()
    {
        return objc_send_msg<unsigned long>(*this, "operatingSystem");
    }

    auto NSProcessInfo::operatingSystemName()
    {
        return objc_send_msg<NSString>(*this, "operatingSystemName");
    }

    auto NSProcessInfo::isOperatingSystemAtLeastVersion(NSOperatingSystemVersion version)
    {
        return objc_send_msg<BOOL>(*this, "isOperatingSystemAtLeastVersion:", version);
    }

    auto NSProcessInfo::hostName()
    {
        return objc_send_msg<NSString>(*this, "hostName");
    }

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