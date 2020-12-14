
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <Foundation/Foundation.hpp>
#include <catch2/catch.hpp>

TEST_CASE("NSProcessInfo", "[Foundation][NSProcessInfo]")
{
    auto info = macxx::NSProcessInfo::processInfo();
    REQUIRE(info);

    SECTION("processName")
    {
        REQUIRE(info.processName());
    }
    SECTION("processIdentifier")
    {
        REQUIRE(info.processIdentifier());
    }
    SECTION("globallyUniqueString")
    {
        REQUIRE(info.globallyUniqueString());
    }
    SECTION("userName")
    {
        REQUIRE(info.userName());
    }
    SECTION("fullUserName")
    {
        REQUIRE(info.fullUserName());
    }
    SECTION("hostName")
    {
        REQUIRE(info.hostName());
    }
    SECTION("operatingSystemVersionString")
    {
        REQUIRE(info.operatingSystemVersionString());
    }
    SECTION("operatingSystemVersion")
    {
        auto version = info.operatingSystemVersion();
        REQUIRE(version.majorVersion > 9);
    }
}