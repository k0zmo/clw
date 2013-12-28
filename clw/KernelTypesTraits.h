#pragma once

#include "Prerequisites.h"
#include <type_traits>

namespace clw
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