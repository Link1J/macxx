
# Copyright Jared Irwin 2020
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.17.0)
project(macxx VERSION 0.0.0)

add_library(Foundation SHARED
    src/id.cpp
    src/Foundation/NSObject.cpp
    src/Foundation/NSString.cpp
    src/Foundation/NSProcessInfo.cpp
)
set_target_properties(Foundation
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_include_directories(Foundation
    PUBLIC
        "include"
    PRIVATE
        "src"
)
target_link_libraries(Foundation
    PRIVATE
        objc
        "-framework Foundation"
)