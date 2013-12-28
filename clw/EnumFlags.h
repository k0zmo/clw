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

#include "TypeTraits.h"

namespace clw
{
    template<typename Enum>
    class EnumFlags
    {
        static_assert(std::is_enum<Enum>::value, "Enum is not a enum-type");
        static_assert(sizeof(Enum) <= 8, "Enum is too big");

    public:
        typedef typename detail::if_<(sizeof(Enum) > 4), 
            typename detail::if_<detail::is_enum_signed<Enum>::value, int64_t, uint64_t>::type,
            typename detail::if_<detail::is_enum_signed<Enum>::value, int32_t, uint64_t>::type
        >::type enum_type;
        //typedef typename std::underlying_type<Enum>::type enum_type;

        EnumFlags() : _v(0) {}
        EnumFlags(Enum v) : _v(enum_type(v)) {}
        explicit EnumFlags(enum_type v) : _v(v) {}

        // Compiler-generated copy/move ctor/assignment operators are fine

        enum_type raw() const { return _v; }
        Enum cast() const { return static_cast<Enum>(_v); }

        EnumFlags operator~() const { return EnumFlags(~_v); }
        EnumFlags operator&(EnumFlags f) const { return EnumFlags(_v & f._v); }
        EnumFlags operator|(EnumFlags f) const { return EnumFlags(_v | f._v); }
        EnumFlags operator^(EnumFlags f) const { return EnumFlags(_v ^ f._v); }

        EnumFlags& operator&=(EnumFlags f) { _v &= f._v; return *this; }
        EnumFlags& operator|=(EnumFlags f) { _v |= f._v; return *this; }
        EnumFlags& operator^=(EnumFlags f) { _v ^= f._v; return *this; }

        bool testFlag(Enum f) const { return (_v & enum_type(f)) == enum_type(f); }       

    private:
        enum_type _v;
    };

    template<typename Enum>
    inline EnumFlags<Enum> operator|(Enum a, const EnumFlags<Enum>& b)
    {
        return b | a;
    }

    template<typename Enum>
    inline EnumFlags<Enum> operator&(Enum a, const EnumFlags<Enum>& b)
    {
        return b & a;
    }

    template<typename Enum>
    inline EnumFlags<Enum> operator^(Enum a, const EnumFlags<Enum>& b)
    {
        return b ^ a;
    }

    template<typename Enum>
    inline EnumFlags<Enum> operator|(Enum a, Enum b)
    {
        return EnumFlags<Enum>(a) | EnumFlags<Enum>(b);
    }

    template<typename Enum>
    inline EnumFlags<Enum> operator&(Enum a, Enum b)
    {
        return EnumFlags<Enum>(a) & EnumFlags<Enum>(b);
    }

    template<typename Enum>
    inline EnumFlags<Enum> operator^(Enum a, Enum b)
    {
        return EnumFlags<Enum>(a) ^ EnumFlags<Enum>(b);
    }
}