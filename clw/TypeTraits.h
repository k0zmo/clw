/*
    Copyright (c) 2012, 2013 Kajetan Swierk <k0zmo@outlook.com>
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#pragma once

#include "Prerequisites.h"
#include <type_traits>

namespace clw
{
    namespace detail
    {
        template<typename T>
        struct not_
            : public std::integral_constant<bool,
                !T::value
            >
        {
        };

        template<bool B>
        struct not_bool
            : public std::integral_constant<bool,
                !B
            >
        {
        };

        template<typename T>
        struct is_scoped_enum
            : public std::integral_constant<bool,
                std::is_enum<T>::value && !std::is_convertible<T, int>::value
            >
        {
        };

        // Works also with enum types (not like std version)
        template<typename T>
        struct is_unsigned_
            : public std::integral_constant<bool,
                (typename std::remove_cv<T>::type)(0) < (typename std::remove_cv<T>::type)(-1)>
        {
        };

        template<typename T>
        struct is_signed_
            : public std::integral_constant<bool,
                not_<is_unsigned_<T>>::value
            >
        {
        };

        template<typename T>
        struct is_enum_unsigned
            : public std::integral_constant<bool,
                is_unsigned_<T>::value &&
                std::is_enum<T>::value
            >
        {
        };

        template<typename T>
        struct is_enum_signed
            : public std::integral_constant<bool,
                is_signed_<T>::value &&
                std::is_enum<T>::value
            >
        {
        };

        namespace test
        {
            static_assert(std::is_integral<int>::value, "Ooops");
            static_assert(!not_<std::is_integral<int>>::value, "Ooops");
            static_assert(!std::is_integral<nullptr_t>::value, "Ooops");
            static_assert(not_<std::is_integral<nullptr_t>>::value, "Ooops");

            static_assert(!not_bool<true>::value, "Ooops");
            static_assert(not_bool<false>::value, "Ooops");

            enum class EC {};
            enum E {};
            struct S {};

            static_assert(!is_scoped_enum<E>::value, "Ooops");
            static_assert(!is_scoped_enum<int>::value, "Ooops");
            static_assert(!is_scoped_enum<S>::value, "Ooops");
            static_assert(is_scoped_enum<EC>::value, "Ooops");

            enum class E_signed : signed {};
            enum E_unsigned : unsigned {};

            static_assert(!is_unsigned_<int>::value, "Ooops");
            static_assert(is_unsigned_<unsigned>::value, "Ooops");
            static_assert(!is_unsigned_<E_signed>::value, "Ooops");
            static_assert(is_unsigned_<E_unsigned>::value, "Ooops");
            static_assert(is_signed_<E_signed>::value, "Ooops");
            static_assert(!is_signed_<E_unsigned>::value, "Ooops");

            static_assert(!is_enum_unsigned<E_signed>::value, "Ooops");
            static_assert(!is_enum_unsigned<unsigned>::value, "Ooops");
            static_assert(is_enum_unsigned<E_unsigned>::value, "Ooops");
            static_assert(!is_enum_unsigned<int>::value, "Ooops");

            static_assert(is_enum_signed<E_signed>::value, "Ooops");
            static_assert(!is_enum_signed<unsigned>::value, "Ooops");
            static_assert(!is_enum_signed<E_unsigned>::value, "Ooops");
            static_assert(!is_enum_signed<int>::value, "Ooops");
        }
    }
}