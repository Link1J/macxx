
# Copyright Jared Irwin 2020-2021
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.16.3)
project(macxx VERSION 0.0.0)

add_library(macxx INTERFACE)
add_library(macxx::macxx ALIAS macxx)
target_include_directories(macxx
    INTERFACE
        "include"
)
target_link_libraries(macxx
    INTERFACE
        objc
)

add_library(Foundation INTERFACE)
add_library(macxx::Foundation ALIAS Foundation)
target_link_libraries(Foundation
    INTERFACE
        macxx
        "-framework Foundation"
)

if (MACXX_BUILD_TESTS)
    Include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.3
    )

    FetchContent_MakeAvailable(Catch2)

    add_executable(test_app
        tests/main.cpp
        tests/Foundation/NSProcessInfo.cpp
    )
    set_target_properties(test_app
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )
    target_link_libraries(test_app
        PRIVATE
            Catch2::Catch2
            macxx::Foundation
    )
    target_compile_definitions(test_app
        PRIVATE
            CATCH_CONFIG_NO_CPP17_UNCAUGHT_EXCEPTIONS
    )
endif()
