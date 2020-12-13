
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "Foundation/NSString.hpp"
#include "base.hpp"

NSString NSString::alloc()
{
    return NSObject::alloc("NSString");
}

id NSString::clazz()
{
    return id{objc_getClass("NSString")};
}

id NSString::initWithUTF8String(const char* nullterminatedCString)
{
    return objc_send_msg<id>(data, "initWithUTF8String:", nullterminatedCString);
}

id NSString::initWithBytes(const void* bytes, unsigned long length, NSStringEncoding encoding)
{
    return objc_send_msg<id>(data, "initWithBytes:length:encoding:", bytes, length, encoding);
}

id NSString::initWithBytesNoCopy(void* bytes, unsigned long length, NSStringEncoding encoding, bool freeWhenDone)
{
    return objc_send_msg<id>(data, "initWithBytesNoCopy:length:encoding:freeWhenDone:", bytes, length, encoding,
                             freeWhenDone);
}

id NSString::initWithCharacters(const char16_t* characters, unsigned long length)
{
    return objc_send_msg<id>(data, "initWithCharacters:length:", characters, length);
}

id NSString::initWithCharactersNoCopy(const char16_t* characters, unsigned long length, bool freeWhenDone)
{
    return objc_send_msg<id>(data, "initWithCharactersNoCopy:length:freeWhenDone:", characters, length, freeWhenDone);
}

id NSString::initWithString(const NSString& aString)
{
    return objc_send_msg<id>(data, "initWithString:", aString);
}

id NSString::initWithCString(const char* nullTerminatedCString, NSStringEncoding encoding)
{
    return objc_send_msg<id>(data, "initWithCString:encoding:", nullTerminatedCString, encoding);
}

unsigned long NSString::length()
{
    return objc_send_msg<unsigned long>(data, "length");
}

const char* NSString::UTF8String()
{
    return objc_send_msg<const char*>(data, "UTF8String");
}

unsigned long NSString::lengthOfBytesUsingEncoding(NSStringEncoding enc)
{
    return objc_send_msg<unsigned long>(data, "lengthOfBytesUsingEncoding:", enc);
}

unsigned long NSString::maximumLengthOfBytesUsingEncoding(NSStringEncoding enc)
{
    return objc_send_msg<unsigned long>(data, "maximumLengthOfBytesUsingEncoding:", enc);
}

char16_t NSString::characterAtIndex(unsigned long index)
{
    return objc_send_msg<char16_t>(data, "characterAtIndex:", index);
}

void NSString::getCharacters(char16_t* buffer, NSRange range)
{
    objc_send_msg<void>(data, "characterAtIndex:range:", buffer, range);
}

bool NSString::getBytes(void* buffer, unsigned long maxBufferCount, unsigned long* usedBufferCount,
                        NSStringEncoding encoding, NSStringEncodingConversionOptions options, NSRange range,
                        NSRange* leftover)
{
    return objc_send_msg<bool>(data, "getBytes:maxLength:usedLength:encoding:options:range:remainingRange:", buffer,
                               maxBufferCount, usedBufferCount, encoding, options, range, leftover);
}

bool NSString::getCString(char* buffer, unsigned long maxBufferCount, NSStringEncoding encoding)
{
    return objc_send_msg<bool>(data, "getCString:maxLength:encoding:", buffer, maxBufferCount, encoding);
}

unsigned long NSString::hash()
{
    return objc_send_msg<unsigned long>(data, "hash");
}

NSComparisonResult NSString::caseInsensitiveCompare(NSString string)
{
    return objc_send_msg<NSComparisonResult>(data, "caseInsensitiveCompare:", string);
}

NSComparisonResult NSString::localizedCaseInsensitiveCompare(NSString string)
{
    return objc_send_msg<NSComparisonResult>(data, "localizedCaseInsensitiveCompare:", string);
}

NSComparisonResult NSString::compare(NSString string)
{
    return objc_send_msg<NSComparisonResult>(data, "compare:", string);
}

NSComparisonResult NSString::localizedCompare(NSString string)
{
    return objc_send_msg<NSComparisonResult>(data, "localizedCompare:", string);
}

NSComparisonResult NSString::compare(NSString string, NSStringCompareOptions mask)
{
    return objc_send_msg<NSComparisonResult>(data, "compare:options:", string);
}

NSComparisonResult NSString::compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare)
{
    return objc_send_msg<NSComparisonResult>(data, "compare:options:range:", string);
}

NSComparisonResult NSString::compare(NSString string, NSStringCompareOptions mask, NSRange rangeOfReceiverToCompare,
                                     id locale)
{
    return objc_send_msg<NSComparisonResult>(data, "compare:options:range:locale:", string);
}

NSComparisonResult NSString::localizedStandardCompare(NSString string)
{
    return objc_send_msg<NSComparisonResult>(data, "localizedStandardCompare:", string);
}

bool NSString::hasPrefix(NSString string)
{
    return objc_send_msg<bool>(data, "hasPrefix:", string);
}

bool NSString::hasSuffix(NSString string)
{
    return objc_send_msg<bool>(data, "hasSuffix:", string);
}

bool NSString::isEqualToString(NSString string)
{
    return objc_send_msg<bool>(data, "isEqualToString:", string);
}
