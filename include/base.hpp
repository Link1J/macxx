
// Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "abi.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cxxabi.h>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <vector>

// For making clang-format play nice
#define MACXX_DEPRECTED_T(x) [[deprecated(x)]]
#define MACXX_DEPRECTED [[deprecated]]

namespace macxx
{
    using NSUInteger = unsigned long;
    using NSInteger  = long;
    using BOOL       = bool;

    constexpr BOOL YES = ((BOOL)1);
    constexpr BOOL NO  = ((BOOL)0);
} // namespace macxx

namespace macxx
{
    struct Protocol
    {};

    template<class T>
    concept _has_abi = requires
    {
        std::declval<T>().__get_abi();
    };

    template<class T>
    concept _id_abi = sizeof(T) == sizeof(void*) && _has_abi<T>;

    template<typename T>
    concept _protocol = std::is_base_of_v<Protocol, T>;

    template<typename T>
    concept _self_type = _id_abi<T> || _protocol<T>;

    template<typename T>
    concept _msg_respose = std::is_floating_point_v<T> || std::is_integral_v<T> || std::is_pointer_v<T> ||
                           std::is_void_v<T> || _self_type<T> || std::is_standard_layout_v<T>;

    template<typename T>
    concept _abi_type = _msg_respose<T> || _has_abi<T>;
} // namespace macxx

namespace macxx
{
    template<typename T>
    struct _id_impl
    {
        operator T()
        {
            return *(T*)this;
        }
    };

    template<typename... types>
    struct _id_recurr_base
    {};

    template<typename L, typename... types>
    struct _id_recurr_base<L, types...> : public _id_impl<L>, public _id_recurr_base<types...>
    {};

    template<typename... types>
    struct id : public _id_recurr_base<types...>
    {
        constexpr id() = default;

        template<_id_abi T>
        constexpr id(const T& other)
            : _id{other.__get_abi()}
        {}

        constexpr auto __get_abi() const
        {
            return _id;
        }

    protected:
        constexpr void set_abi(abi::id other)
        {
            _id = other;
        }

    private:
        abi::id _id = nullptr;
    };

    static_assert(sizeof(id<>) == sizeof(void*), "id is not a pointer");
} // namespace macxx

namespace macxx
{
    struct SEL
    {
        SEL(std::string_view s)
            : name{s}
            , _id{abi::sel_getUid(s.data())} {};

        SEL(const char* s)
            : name{s}
            , _id{abi::sel_getUid(s)} {};

        SEL(abi::SEL s)
            : name{abi::sel_getName(s)}
            , _id{s} {};

        constexpr auto __get_abi() const
        {
            return _id;
        }

    private:
        std::string_view name;
        abi::SEL         _id;
    };

    // static_assert(sizeof(SEL) == sizeof(void*), "SEL is not a pointer");
} // namespace macxx

namespace macxx
{
    struct Class
    {
        constexpr auto __get_abi() const
        {
            return _id;
        }

    private:
        abi::Class _id;
    };
} // namespace macxx

namespace macxx
{
    template<typename T>
    constexpr auto type_name()
    {
#ifdef __clang__
        std::string_view name   = __PRETTY_FUNCTION__;
        std::string_view prefix = "auto macxx::type_name() [T = ";
        std::string_view suffix = "]";
#elif defined(__GNUC__)
        std::string_view name   = __PRETTY_FUNCTION__;
        std::string_view prefix = "constexpr auto macxx::type_name() [with T = ";
        std::string_view suffix = "]";
#elif defined(_MSC_VER)
        std::string_view name   = __FUNCSIG__;
        std::string_view prefix = "auto __cdecl macxx::type_name<";
        std::string_view suffix = ">(void) noexcept";
#else
        std::string_view name   = "Error: unsupported compiler";
        std::string_view prefix = "";
        std::string_view suffix = "";
#endif
        name.remove_prefix(prefix.size());
        name.remove_suffix(suffix.size());
        return name;
    }

    template<_abi_type T>
    constexpr auto get_abi(const T& input)
    {
        if constexpr (_has_abi<T>)
        {
            return input.__get_abi();
        }
        else if constexpr (_protocol<T>)
        {
            return ((id<>*)&input)->__get_abi();
        }
        else
        {
            return input;
        }
    }

    template<typename T>
    using abi_type = decltype(get_abi(std::declval<T>()));

    template<_msg_respose R, typename... A>
    auto objc_send_msg(_self_type auto& self, SEL cmd, A&&... args)
    {
        using self_t = abi_type<decltype(self)>;

        auto _self = get_abi(self);
        auto _cmd  = get_abi(cmd);

        if constexpr (_self_type<R>)
        {
            const auto msg_i = (void* (*)(self_t, abi::SEL, abi_type<A>...))(abi::objc_msgSend);
            auto       value = msg_i(_self, _cmd, get_abi(args)...);
            return *(R*)&value;
        }
        else if constexpr (std::is_integral_v<R> || std::is_pointer_v<R> || std::is_void_v<R>)
        {
            const auto msg_i = (R(*)(self_t, abi::SEL, abi_type<A>...))(abi::objc_msgSend);
            return msg_i(_self, _cmd, get_abi(args)...);
        }
        else if constexpr (std::is_floating_point_v<R>)
        {
            const auto msg_f = (R(*)(self_t, abi::SEL, abi_type<A>...))(abi::objc_msgSend_fpret);
            return msg_f(_self, _cmd, get_abi(args)...);
        }
        else if constexpr (std::is_standard_layout_v<R>)
        {
            const auto msg_s = (void (*)(R*, self_t, abi::SEL, abi_type<A>...))(abi::objc_msgSend_stret);
            R          return_value;
            msg_s(&return_value, _self, _cmd, get_abi(args)...);
            return return_value;
        }
        else
        {
            static_assert(!sizeof(std::decay_t<R>*), "Invalid return type");
        }
    }

    template<_id_abi T>
    auto get_class()
    {
        auto        name_view = type_name<T>();
        std::string name      = {name_view.data(), name_view.size()};
        if (auto index = name.find('<'); index != std::string::npos)
            name[index] = 0;
        return __builtin_bit_cast(Class, abi::objc_getClass(name.c_str() + 7));
    }

    template<_id_abi T>
    auto get_class(const T& self)
    {
        return abi::object_getClass(get_abi(self));
    }

    template<_msg_respose T>
    auto alloc(Class clazz)
    {
        return objc_send_msg<T>(clazz, "alloc");
    }

    template<_id_abi T>
    auto alloc()
    {
        return alloc<T>(get_class<T>());
    }
} // namespace macxx