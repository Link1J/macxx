
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "NSProcessInfo.hpp"

namespace macxx
{
    NSString::NSString()
        : super{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "init"));
    }

    auto NSString::UTF8String() const
    {
        return objc_send_msg<const char*>(*this, "UTF8String");
    }

    NSString::NSString(const char* nullterminatedCString)
        : super{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithUTF8String:", nullterminatedCString));
    }

    NSString::NSString(const void* bytes, unsigned long length, NSStringEncoding encoding)
        : super{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithBytes:length:encoding:", bytes, length, encoding));
    }

    NSString::NSString(void* bytes, unsigned long length, NSStringEncoding encoding, bool freeWhenDone)
        : super{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithBytesNoCopy:length:encoding:freeWhenDone:", bytes, length,
                                       encoding, freeWhenDone));
    }

    NSString::NSString(const char16_t* characters, unsigned long length)
        : super{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithCharacters:length:", characters, length));
    }

    NSString::NSString(const char16_t* characters, unsigned long length, bool freeWhenDone)
        : super{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithCharactersNoCopy:length:freeWhenDone:", characters, length,
                                       freeWhenDone));
    }

    NSString::NSString(const NSString& aString)
        : super{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithString:", aString));
    }

    NSString::NSString(const char* nullTerminatedCString, NSStringEncoding encoding)
        : super{get_class<NSString>()}
    {
        set_abi(objc_send_msg<abi::id>(*this, "initWithCString:encoding:", nullTerminatedCString, encoding));
    }

    auto NSString::length() const
    {
        return objc_send_msg<unsigned long>(*this, "length");
    }

    auto NSString::lengthOfBytesUsingEncoding(NSStringEncoding enc)
    {
        return objc_send_msg<unsigned long>(*this, "lengthOfBytesUsingEncoding:", enc);
    }

    auto NSString::maximumLengthOfBytesUsingEncoding(NSStringEncoding enc)
    {
        return objc_send_msg<unsigned long>(*this, "maximumLengthOfBytesUsingEncoding:", enc);
    }

    auto NSString::characterAtIndex(unsigned long index)
    {
        return objc_send_msg<char16_t>(*this, "characterAtIndex:", index);
    }

    auto NSString::getCharacters(char16_t* buffer, NSRange range)
    {
        objc_send_msg<void>(*this, "characterAtIndex:range:", buffer, range);
    }

    auto NSString::getBytes(void* buffer, unsigned long maxBufferCount, unsigned long* usedBufferCount,
                            NSStringEncoding encoding, NSStringEncodingConversionOptions options, NSRange range,
                            NSRange* leftover)
    {
        return objc_send_msg<bool>(*this,
                                   "getBytes:maxLength:usedLength:encoding:options:range:remainingRange:", buffer,
                                   maxBufferCount, usedBufferCount, encoding, options, range, leftover);
    }

    auto NSString::getCString(char* buffer, unsigned long maxBufferCount, NSStringEncoding encoding)
    {
        return objc_send_msg<bool>(*this, "getCString:maxLength:encoding:", buffer, maxBufferCount, encoding);
    }

    auto NSString::caseInsensitiveCompare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "caseInsensitiveCompare:", string);
    }

    auto NSString::localizedCaseInsensitiveCompare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "localizedCaseInsensitiveCompare:", string);
    }

    auto NSString::compare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "compare:", string);
    }

    auto NSString::localizedCompare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "localizedCompare:", string);
    }

    auto NSString::compare(NSString string, NSStringCompareOptions mask)
    {
        return objc_send_msg<NSComparisonResult>(*this, "compare:options:", string);
    }

    auto NSString::compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare)
    {
        return objc_send_msg<NSComparisonResult>(*this, "compare:options:range:", string);
    }

    auto NSString::compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare, id locale)
    {
        return objc_send_msg<NSComparisonResult>(*this, "compare:options:range:locale:", string);
    }

    auto NSString::localizedStandardCompare(NSString string)
    {
        return objc_send_msg<NSComparisonResult>(*this, "localizedStandardCompare:", string);
    }

    auto NSString::hasPrefix(NSString string)
    {
        return objc_send_msg<bool>(*this, "hasPrefix:", string);
    }

    auto NSString::hasSuffix(NSString string)
    {
        return objc_send_msg<bool>(*this, "hasSuffix:", string);
    }

    auto NSString::isEqualToString(NSString string)
    {
        return objc_send_msg<bool>(*this, "isEqualToString:", string);
    }
} // namespace macxx

std::ostream& operator<<(std::ostream& os, const macxx::NSString& object)
{
    return (os << object.UTF8String());
}