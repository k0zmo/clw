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

#include "clw/Prerequisites.h"
#include "clw/MemoryObject.h"

namespace clw
{
    // !TODO: Add new classes to reflect changes in OpenCL 1.2

    enum class EChannelOrder
    {
        R           = 0x10B0,
        A           = 0x10B1,
        RG          = 0x10B2,
        RA          = 0x10B3,
        RGB         = 0x10B4,
        RGBA        = 0x10B5,
        BGRA        = 0x10B6,
        ARGB        = 0x10B7,
        Intensity   = 0x10B8,
        Luminance   = 0x10B9,
        Rx          = 0x10BA,
        RGx         = 0x10BB,
        RGBx        = 0x10BC
    };

    enum class EChannelType
    {
        Normalized_Int8           = 0x10D0,
        Normalized_Int16          = 0x10D1,
        Normalized_UInt8          = 0x10D2,
        Normalized_UInt16         = 0x10D3,
        Normalized_565            = 0x10D4,
        Normalized_555            = 0x10D5,
        Normalized_101010         = 0x10D6,
        Unnormalized_Int8         = 0x10D7,
        Unnormalized_Int16        = 0x10D8,
        Unnormalized_Int32        = 0x10D9,
        Unnormalized_UInt8        = 0x10DA,
        Unnormalized_UInt16       = 0x10DB,
        Unnormalized_UInt32       = 0x10DC,
        HalfFloat                 = 0x10DD,
        Float                     = 0x10DE
    };

    struct CLW_EXPORT ImageFormat
    {
        EChannelOrder order;
        EChannelType type;

        ImageFormat() : order(EChannelOrder(0)), type(EChannelType(0)) {}
        ImageFormat(EChannelOrder order, EChannelType type)
            : order(order), type(type) {}

        bool isNull() const
        {
            return order == EChannelOrder(0) ||
                type == EChannelType(0);
        }

        bool operator==(const ImageFormat& other) const 
        {
            return order == other.order &&
                type == other.type;
        }

        bool operator!=(const ImageFormat& other) const
        {
            return !operator==(other);
        }
    };

    class CLW_EXPORT Image2D : public MemoryObject
    {
    public:
        Image2D() {}
        Image2D(Context* ctx, cl_mem id)
            : MemoryObject(ctx, id) {}

        Image2D(const Image2D& other);
        Image2D& operator=(const Image2D& other);

        Image2D(Image2D&& other);
        Image2D& operator=(Image2D&& other);

        ImageFormat format() const;

        int width() const;
        int height() const;

        int bytesPerElement() const;
        int bytesPerLine() const;

    private:
        mutable ImageFormat _fmt;
    };

    class CLW_EXPORT Image3D : public MemoryObject
    {
    public:
        Image3D() {}
        Image3D(Context* ctx, cl_mem id)
            : MemoryObject(ctx, id) {}

        Image3D(const Image3D& other);
        Image3D& operator=(const Image3D& other);

        Image3D(Image3D&& other);
        Image3D& operator=(Image3D&& other);

        ImageFormat format() const;

        int width() const;
        int height() const;
        int depth() const;

        int bytesPerElement() const;
        int bytesPerLine() const;
        int bytesPerSlice() const;

    private:
        mutable ImageFormat _fmt;
    };

#define CASE(X) case X: return string(#X);
    inline string channelTypeName(EChannelType type)
    {
        switch(type)
        {
        CASE(EChannelType::Normalized_Int8);
        CASE(EChannelType::Normalized_Int16);
        CASE(EChannelType::Normalized_UInt8);
        CASE(EChannelType::Normalized_UInt16);
        CASE(EChannelType::Normalized_565);
        CASE(EChannelType::Normalized_555);
        CASE(EChannelType::Normalized_101010);
        CASE(EChannelType::Unnormalized_Int8);
        CASE(EChannelType::Unnormalized_Int16);
        CASE(EChannelType::Unnormalized_Int32);
        CASE(EChannelType::Unnormalized_UInt8);
        CASE(EChannelType::Unnormalized_UInt16);
        CASE(EChannelType::Unnormalized_UInt32);
        CASE(EChannelType::HalfFloat);
        CASE(EChannelType::Float);
        default: return "Undefined";
        }
    }

    inline string channelOrderName(EChannelOrder order)
    {
        switch(order)
        {
        CASE(EChannelOrder::R);
        CASE(EChannelOrder::A);
        CASE(EChannelOrder::RG);
        CASE(EChannelOrder::RA);
        CASE(EChannelOrder::RGB);
        CASE(EChannelOrder::RGBA);
        CASE(EChannelOrder::BGRA);
        CASE(EChannelOrder::ARGB);
        CASE(EChannelOrder::Intensity);
        CASE(EChannelOrder::Luminance);
        CASE(EChannelOrder::Rx);
        CASE(EChannelOrder::RGx);
        CASE(EChannelOrder::RGBx);
        default: return "Undefined";
        }
    }
#undef CASE
}
