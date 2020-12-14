
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
} // namespace macxx