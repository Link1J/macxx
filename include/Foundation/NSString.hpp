
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "NSObject.hpp"
#include <ostream>
#include <string>
#include <type_traits>

// For making clang-format play nice
#define DEPRECTED_T(x) [[deprecated(x)]]
#define DEPRECTED [[deprecated]]

enum NSStringEncoding
{
    NSASCIIStringEncoding         = 1,
    NSNEXTSTEPStringEncoding      = 2,
    NSJapaneseEUCStringEncoding   = 3,
    NSUTF8StringEncoding          = 4,
    NSISOLatin1StringEncoding     = 5,
    NSSymbolStringEncoding        = 6,
    NSNonLossyASCIIStringEncoding = 7,
    NSShiftJISStringEncoding      = 8,
    NSISOLatin2StringEncoding     = 9,
    NSUnicodeStringEncoding       = 10,
    NSWindowsCP1251StringEncoding = 11,
    NSWindowsCP1252StringEncoding = 12,
    NSWindowsCP1253StringEncoding = 13,
    NSWindowsCP1254StringEncoding = 14,
    NSWindowsCP1250StringEncoding = 15,
    NSISO2022JPStringEncoding     = 21,
};

struct NSRange
{
    unsigned long location;
    unsigned long length;
};

enum NSStringEncodingConversionOptions
{
    NSAllowLossyEncodingConversion             DEPRECTED = 1,
    NSExternalRepresentationEncodingConversion DEPRECTED = 2,
    NSStringEncodingConversionAllowLossy                 = 1,
    NSStringEncodingConversionExternalRepresentation     = 2,
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

class NSString : public NSObject
{
public:
    NSString(id data);

    static NSString alloc();
    static id       clazz();

    id initWithUTF8String(const char* nullterminatedCString);
    id initWithBytes(const void* bytes, unsigned long length, NSStringEncoding encoding);
    id initWithBytesNoCopy(void* bytes, unsigned long length, NSStringEncoding encoding, bool freeWhenDone);
    id initWithCharacters(const char16_t* characters, unsigned long length);
    id initWithCharactersNoCopy(const char16_t* characters, unsigned long length, bool freeWhenDone);
    id initWithString(const NSString& aString);
    id initWithCString(const char* nullTerminatedCString, NSStringEncoding encoding);

    unsigned long length();
    const char*   UTF8String();
    unsigned long hash();

    const char*   cStringUsingEncoding(NSStringEncoding encoding) const;
    unsigned long lengthOfBytesUsingEncoding(NSStringEncoding enc);
    unsigned long maximumLengthOfBytesUsingEncoding(NSStringEncoding enc);
    char16_t      characterAtIndex(unsigned long index);
    void          getCharacters(char16_t* buffer, NSRange range);
    bool getBytes(void* buffer, unsigned long maxBufferCount, unsigned long* usedBufferCount, NSStringEncoding encoding,
                  NSStringEncodingConversionOptions options, NSRange range, NSRange* leftover);
    bool getCString(char* buffer, unsigned long maxBufferCount, NSStringEncoding encoding);

    NSComparisonResult caseInsensitiveCompare(NSString string);
    NSComparisonResult localizedCaseInsensitiveCompare(NSString string);
    NSComparisonResult compare(NSString string);
    NSComparisonResult localizedCompare(NSString string);
    NSComparisonResult compare(NSString string, NSStringCompareOptions mask);
    NSComparisonResult compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare);
    NSComparisonResult compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare,
                               id locale);
    NSComparisonResult localizedStandardCompare(NSString string);
    bool               hasPrefix(NSString string);
    bool               hasSuffix(NSString string);
    bool               isEqualToString(NSString string);
};

#undef DEPRECTED_T
#undef DEPRECTED