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
        template <typename T>
        struct is_kernel_value_type
            : public std::integral_constant<bool,
                std::is_arithmetic<T>::value
            >
        {
        };

        /* Same effect
        template <typename T>
        struct is_kernel_value_type
        {
            static const bool value = 
                std::is_arithmetic<T>::value;
        };
        */

        template <typename T>
        struct is_kernel_memory_object
            : public std::integral_constant<bool,
                std::is_base_of<MemoryObject, T>::value
            >
        {
        };

        template <typename T>
        struct is_kernel_custom_arg
            : public std::integral_constant<bool,
                !is_kernel_value_type<T>::value && 
                !is_kernel_memory_object<T>::value
            >
        {
        };
    }
}