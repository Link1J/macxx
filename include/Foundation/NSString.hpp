
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "NSObject.hpp"
#include <ostream>
#include <string>
#include <type_traits>

namespace macxx
{
    enum NSStringEncoding
    {
        NSASCIIStringEncoding             = 1,
        NSNEXTSTEPStringEncoding          = 2,
        NSJapaneseEUCStringEncoding       = 3,
        NSUTF8StringEncoding              = 4,
        NSISOLatin1StringEncoding         = 5,
        NSSymbolStringEncoding            = 6,
        NSNonLossyASCIIStringEncoding     = 7,
        NSShiftJISStringEncoding          = 8,
        NSISOLatin2StringEncoding         = 9,
        NSUnicodeStringEncoding           = 10,
        NSWindowsCP1251StringEncoding     = 11,
        NSWindowsCP1252StringEncoding     = 12,
        NSWindowsCP1253StringEncoding     = 13,
        NSWindowsCP1254StringEncoding     = 14,
        NSWindowsCP1250StringEncoding     = 15,
        NSISO2022JPStringEncoding         = 21,
        NSMacOSRomanStringEncoding        = 30,
        NSUTF16StringEncoding             = NSUnicodeStringEncoding,
        NSUTF16BigEndianStringEncoding    = 0x90000100,
        NSUTF16LittleEndianStringEncoding = 0x94000100,
        NSUTF32StringEncoding             = 0x8c000100,
        NSUTF32BigEndianStringEncoding    = 0x98000100,
        NSUTF32LittleEndianStringEncoding = 0x9c000100
    };

    struct NSRange
    {
        unsigned long location;
        unsigned long length;
    };

    enum NSStringEncodingConversionOptions
    {
        NSAllowLossyEncodingConversion             MACXX_DEPRECTED = 1,
        NSExternalRepresentationEncodingConversion MACXX_DEPRECTED = 2,
        NSStringEncodingConversionAllowLossy                       = 1,
        NSStringEncodingConversionExternalRepresentation           = 2,
    };

    enum NSComparisonResult
    {
        NSOrderedSame,
        NSOrderedDescending,
        NSOrderedAscending = -1L,
    };

    enum NSStringCompareOptions
    {
        NSCaseInsensitiveSearch      = 1,
        NSLiteralSearch              = 2,
        NSBackwardsSearch            = 4,
        NSAnchoredSearch             = 8,
        NSNumericSearch              = 64,
        NSDiacriticInsensitiveSearch = 128,
        NSWidthInsensitiveSearch     = 256,
        NSForcedOrderingSearch       = 512,
        NSRegularExpressionSearch    = 1024,
    };

    struct NSString : public NSObject<NSCopying<NSString>, NSMutableCopying<NSString>>
    {
        using super = NSObject<NSCopying<NSString>, NSMutableCopying<NSString>>;

        NSString();
        NSString(const char* nullterminatedCString);
        NSString(const void* bytes, unsigned long length, NSStringEncoding encoding);
        NSString(void* bytes, unsigned long length, NSStringEncoding encoding, bool freeWhenDone);
        NSString(const char16_t* characters, unsigned long length);
        NSString(const char16_t* characters, unsigned long length, bool freeWhenDone);
        NSString(const NSString& aString);
        NSString(const char* nullTerminatedCString, NSStringEncoding encoding);

        auto length() const;
        auto UTF8String() const;

        auto cStringUsingEncoding(NSStringEncoding encoding);
        auto lengthOfBytesUsingEncoding(NSStringEncoding enc);
        auto maximumLengthOfBytesUsingEncoding(NSStringEncoding enc);
        auto characterAtIndex(unsigned long index);
        auto getCharacters(char16_t* buffer, NSRange range);
        auto getBytes(void* buffer, unsigned long maxBufferCount, unsigned long* usedBufferCount,
                      NSStringEncoding encoding, NSStringEncodingConversionOptions options, NSRange range,
                      NSRange* leftover);
        auto getCString(char* buffer, unsigned long maxBufferCount, NSStringEncoding encoding);

        auto caseInsensitiveCompare(NSString string);
        auto localizedCaseInsensitiveCompare(NSString string);
        auto compare(NSString string);
        auto localizedCompare(NSString string);
        auto compare(NSString string, NSStringCompareOptions mask);
        auto compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare);
        auto compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare, id locale);
        auto localizedStandardCompare(NSString string);
        auto hasPrefix(NSString string);
        auto hasSuffix(NSString string);
        auto isEqualToString(NSString string);
    };

    static_assert(sizeof(NSString) == sizeof(id<>));
} // namespace macxx