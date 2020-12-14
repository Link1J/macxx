
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

// clang-format should remain off for this file
// It does not improve readablity

#pragma once

#include <AvailabilityMacros.h>
#include <Availability.h>
#include <TargetConditionals.h>

// clang-format off

#define MACXX_TARGETING_MACOSX   0
#define MACXX_TARGETING_IOS      0
#define MACXX_TARGETING_MACOS    0
#define MACXX_TARGETING_TV_OS    0
#define MACXX_TARGETING_WATCH_OS 0
#define MACXX_TARGETING_HOMEKIT  0

#if TARGET_OS_MAC
#   if TARGET_OS_OSX
#       undef MACXX_TARGETING_MACOSX
#       define MACXX_TARGETING_MACOSX 1
#   elif TARGET_OS_IPHONE
#       if TARGET_OS_IOS
#           undef MACXX_TARGETING_IOS
#           define MACXX_TARGETING_IOS 1
#       elif TARGET_OS_TV
#           undef MACXX_TARGETING_TV_OS
#           define MACXX_TARGETING_TV_OS 1
#       elif TARGET_OS_WATCH || TARGET_OS_NANO
#           undef MACXX_TARGETING_WATCH_OS
#           define MACXX_TARGETING_WATCH_OS 1
#       elif TARGET_OS_BRIDGE
#           undef MACXX_TARGETING_HOMEKIT
#           define MACXX_TARGETING_HOMEKIT 1
#       elif TARGET_OS_MACCATALYST
#           undef MACXX_TARGETING_MACOS
#           define MACXX_TARGETING_MACOS 1
#       else
#           undef MACXX_TARGETING_IOS
#           define MACXX_TARGETING_IOS 1
#       endif
#   else
#       undef MACXX_TARGETING_MACOSX
#       define MACXX_TARGETING_MACOSX 1
#   endif
#endif

#ifndef MACXX_TARGETING_VERSION
#define MACXX_TARGETING_VERSION _ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED_
#endif

#if defined(MACXX_REMOVE_DEPRECTED)
#undef MACXX_REMOVE_DEPRECTED
#define MACXX_REMOVE_DEPRECTED 1
#endif

#define MACXX_API_AVAILABLE_FROM(OSX, iOS, macOS, Apple_TV_OS, Apple_Watch_OS) \
    (\
        (MACXX_TARGETING_MACOSX   && (OSX            <= MACXX_TARGETING_VERSION) && (OSX            != 0)) || \
        (MACXX_TARGETING_IOS      && (iOS            <= MACXX_TARGETING_VERSION) && (iOS            != 0)) || \
        (MACXX_TARGETING_MACOS    && (macOS          <= MACXX_TARGETING_VERSION) && (macOS          != 0)) || \
        (MACXX_TARGETING_TV_OS    && (Apple_TV_OS    <= MACXX_TARGETING_VERSION) && (Apple_TV_OS    != 0)) || \
        (MACXX_TARGETING_WATCH_OS && (Apple_Watch_OS <= MACXX_TARGETING_VERSION) && (Apple_Watch_OS != 0)) \
    )

#define MACXX_API_DEPRECTED_FROM(OSX, iOS, macOS, Apple_TV_OS, Apple_Watch_OS) \
    !(MACXX_REMOVE_DEPRECTED || (\
        (MACXX_TARGETING_MACOSX   && (OSX            <= MACXX_TARGETING_VERSION) && (OSX            != 0)) || \
        (MACXX_TARGETING_IOS      && (iOS            <= MACXX_TARGETING_VERSION) && (iOS            != 0)) || \
        (MACXX_TARGETING_MACOS    && (macOS          <= MACXX_TARGETING_VERSION) && (macOS          != 0)) || \
        (MACXX_TARGETING_TV_OS    && (Apple_TV_OS    <= MACXX_TARGETING_VERSION) && (Apple_TV_OS    != 0)) || \
        (MACXX_TARGETING_WATCH_OS && (Apple_Watch_OS <= MACXX_TARGETING_VERSION) && (Apple_Watch_OS != 0)) \
    ))

// clang-format on