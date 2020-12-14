
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

    struct NSString : public NSObject<>
    {
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

    inline auto NSObjectProtocol::description() const
    {
        return objc_send_msg<NSString>(*this, "description");
    }

    inline NSString::NSString()
        : NSObject<>{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "init"));
    }

    inline auto NSString::UTF8String() const
    {
        return objc_send_msg<const char*>(*this, "UTF8String");
    }

    inline NSString::NSString(const char* nullterminatedCString)
        : NSObject<>{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithUTF8String:", nullterminatedCString));
    }

    inline NSString::NSString(const void* bytes, unsigned long length, NSStringEncoding encoding)
        : NSObject<>{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithBytes:length:encoding:", bytes, length, encoding));
    }

    inline NSString::NSString(void* bytes, unsigned long length, NSStringEncoding encoding, bool freeWhenDone)
        : NSObject<>{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithBytesNoCopy:length:encoding:freeWhenDone:", bytes, length,
                                       encoding, freeWhenDone));
    }

    inline NSString::NSString(const char16_t* characters, unsigned long length)
        : NSObject<>{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithCharacters:length:", characters, length));
    }

    inline NSString::NSString(const char16_t* characters, unsigned long length, bool freeWhenDone)
        : NSObject<>{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithCharactersNoCopy:length:freeWhenDone:", characters, length,
                                       freeWhenDone));
    }

    inline NSString::NSString(const NSString& aString)
        : NSObject<>{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithString:", aString));
    }

    inline NSString::NSString(const char* nullTerminatedCString, NSStringEncoding encoding)
        : NSObject<>{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithCString:encoding:", nullTerminatedCString, encoding));
    }

    inline auto NSString::length() const
    {
        return objc_send_msg<unsigned long>(*this, "length");
    }

    inline auto NSString::lengthOfBytesUsingEncoding(NSStringEncoding enc)
    {
        return objc_send_msg<unsigned long>(*this, "lengthOfBytesUsingEncoding:", enc);
    }

    inline auto NSString::maximumLengthOfBytesUsingEncoding(NSStringEncoding enc)
    {
        return objc_send_msg<unsigned long>(*this, "maximumLengthOfBytesUsingEncoding:", enc);
    }

    inline auto NSString::characterAtIndex(unsigned long index)
    {
        return objc_send_msg<char16_t>(*this, "characterAtIndex:", index);
    }

    inline auto NSString::getCharacters(char16_t* buffer, NSRange range)
    {
        objc_send_msg<void>(*this, "characterAtIndex:range:", buffer, range);
    }

    inline auto NSString::getBytes(void* buffer, unsigned long maxBufferCount, unsigned long* usedBufferCount,
                                   NSStringEncoding encoding, NSStringEncodingConversionOptions options, NSRange range,
                                   NSRange* leftover)
    {
        return objc_send_msg<bool>(*this,
                                   "getBytes:maxLength:usedLength:encoding:options:range:remainingRange:", buffer,
                                   maxBufferCount, usedBufferCount, encoding, options, range, leftover);
    }

    inline auto NSString::getCString(char* buffer, unsigned long maxBufferCount, NSStringEncoding encoding)
    {
        return objc_send_msg<bool>(*this, "getCString:maxLength:encoding:", buffer, maxBufferCount, encoding);
    }

    // inline  auto NSString::hash()
    //{
    //    return objc_send_msg<unsigned long>(*this, "hash");
    //}

    inline auto NSString::caseInsensitiveCompare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "caseInsensitiveCompare:", string);
    }

    inline auto NSString::localizedCaseInsensitiveCompare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "localizedCaseInsensitiveCompare:", string);
    }

    inline auto NSString::compare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "compare:", string);
    }

    inline auto NSString::localizedCompare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "localizedCompare:", string);
    }

    inline auto NSString::compare(NSString string, NSStringCompareOptions mask)
    {
        return objc_send_msg<NSComparisonResult>(*this, "compare:options:", string);
    }

    inline auto NSString::compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare)
    {
        return objc_send_msg<NSComparisonResult>(*this, "compare:options:range:", string);
    }

    inline auto NSString::compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare,
                                  id locale)
    {
        return objc_send_msg<NSComparisonResult>(*this, "compare:options:range:locale:", string);
    }

    inline auto NSString::localizedStandardCompare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "localizedStandardCompare:", string);
    }

    inline auto NSString::hasPrefix(NSString string)
    {
        return objc_send_msg<bool>(*this, "hasPrefix:", string);
    }

    inline auto NSString::hasSuffix(NSString string)
    {
        return objc_send_msg<bool>(*this, "hasSuffix:", string);
    }

    inline auto NSString::isEqualToString(NSString string)
    {
        return objc_send_msg<bool>(*this, "isEqualToString:", string);
    }
} // namespace macxx

inline std::ostream& operator<<(std::ostream& os, const macxx::NSString& object)
{
    return (os << object.UTF8String());
}