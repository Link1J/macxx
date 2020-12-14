
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>
#include <cstdint>

namespace macxx::abi
{
    namespace objc
    {
        struct object;
        struct selector;
        struct clazz;
        struct object;
        struct method;
        struct ivar;
        struct category;
        struct property;
    }; // namespace objc

    using id              = objc::object*;
    using SEL             = objc::selector*;
    using Class           = objc::clazz*;
    using Protocol        = objc::object;
    using Method          = objc::method*;
    using Ivar            = objc::ivar*;
    using Category        = objc::category*;
    using objc_property_t = objc::property*;
    using IMP             = void (*)();

    using _c_string = const char*;

    extern "C"
    {
        void*  objc_msgSend(id self, SEL _cmd, ...);
        void   objc_msgSend_stret(void* stretAddr, id self, SEL _cmd, ...);
        double objc_msgSend_fpret(id self, SEL _cmd, ...);
        id     objc_alloc(Class clazz);
        id     objc_getClass(_c_string name);

        Class object_getClass(id obj);

        _c_string sel_getName(SEL sel);
        bool      sel_isMapped(_c_string sel);
        SEL       sel_getUid(_c_string str);

        _c_string class_getName(Class cls);
        Class     class_getSuperclass(Class cls);
    }

#if defined(_MACXX_ABI_DEBUG_INFO_)
    // The ABI types info may not be correct.
    namespace objc
    {
        struct ivar_list;
        struct method_list;
        struct cache;
        struct protocol_list;

        struct object
        {
            Class isa;
        };

        struct selector
        {
            union
            {
                _c_string name;
                uintptr_t index;
            };
            _c_string types;
        };

        struct clazz
        {
            Class          isa;
            Class          super_class;
            _c_string      name;
            long           version;
            long           info;
            long           instance_size;
            ivar_list*     ivars;
            method_list**  methodLists;
            cache*         cache;
            protocol_list* protocols;
        };

        struct method
        {
            IMP       imp;
            SEL       selector;
            _c_string types;
        };

        struct ivar
        {
            _c_string name;
            _c_string type;
            int*      offset;
            uint32_t  size;
            uint32_t  flags;
        };

        struct ivar_list
        {
            int    count;
            size_t size;
            ivar   ivar_list[];
        };

        struct property
        {
            _c_string name;
            _c_string attributes;
            _c_string type;
            SEL       getter;
            SEL       setter;
        };

        struct property_list
        {
            int            count;
            size_t         size;
            property_list* next;
            property       properties[];
        };
    } // namespace objc
#endif
} // namespace macxx::abi
