
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "NSObject.hpp"
#include "NSString.hpp"
#include <functional>

// For making clang-format play nice
#define DEPRECTED_T(x) [[deprecated(x)]]
#define DEPRECTED [[deprecated]]

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
    NSWindowsNTOperatingSystem DEPRECTED = 1,
    NSWindows95OperatingSystem DEPRECTED,
    NSSolarisOperatingSystem   DEPRECTED,
    NSHPUXOperatingSystem      DEPRECTED,
    NSMACHOperatingSystem      DEPRECTED,
    NSSunOSOperatingSystem     DEPRECTED,
    NSOSF1OperatingSystem      DEPRECTED
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

class NSProcessInfo : public NSObject
{
public:
    NSProcessInfo(id data);

    static NSProcessInfo alloc();
    static id            clazz();
    static NSProcessInfo processInfo();

    // NSArray arguments();
    // NSDictionary environment();
    NSString                  processName();
    int                       processIdentifier();
    bool                      iOSAppOnMac();
    bool                      macCatalystApp();
    NSString                  globallyUniqueString();
    NSString                  userName();
    NSString                  fullUserName();
    bool                      automaticTerminationSupportEnabled();
    NSString                  hostName();
    NSString                  operatingSystemVersionString();
    NSOperatingSystemVersion  operatingSystemVersion();
    unsigned long             processorCount();
    unsigned long             activeProcessorCount();
    unsigned long long        physicalMemory();
    double                    systemUptime();
    NSProcessInfoThermalState thermalState();
    bool                      lowPowerModeEnabled();

    void disableSuddenTermination();
    void enableSuddenTermination();
    void disableAutomaticTermination(NSString reason);
    void enableAutomaticTermination(NSString reason);
    DEPRECTED_T("Use operatingSystemVersion or isOperatingSystemAtLeastVersion: instead")
    unsigned long operatingSystem();
    DEPRECTED_T("Use operatingSystemVersionString instead.")
    NSString operatingSystemName();
    bool     isOperatingSystemAtLeastVersion(NSOperatingSystemVersion version);
    id       beginActivityWithOptions(NSActivityOptions options, NSString reason);
    void     endActivity(id activity);
    void performActivityWithOptions(NSActivityOptions options, NSString reason, std::function<void(void* block)> block);
    void performExpiringActivityWithReason(NSString reason, std::function<void(void* block, bool expired)> block);
};

#undef DEPRECTED