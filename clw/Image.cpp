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

#include "Image.h"
#include "details.h"

namespace clw
{
    namespace detail
    {
        template<typename Value>
        Value imageInfo(cl_mem _id, cl_image_info info)
        {
            Value value;
            cl_int error;
            if((error = clGetImageInfo(_id, info, sizeof(Value),
                    &value, nullptr)) != CL_SUCCESS)
            {
                reportError("imageInfo(): ", error);
                Value value = {0};
                return value;
            }
            return value;
        }
    }

    Image2D::Image2D(const Image2D& other)
        : MemoryObject(), fmt(other.fmt)
    {
        setMemoryId(other._ctx, other._id);
    }

    Image2D& Image2D::operator=(const Image2D& other)
    {
        fmt = other.fmt;
        setMemoryId(other._ctx, other._id);
        return *this;
    }

    ImageFormat Image2D::format() const
    {
        if(fmt.isNull())
        {
            cl_image_format iformat = detail::imageInfo<cl_image_format>
                (memoryId(), CL_IMAGE_FORMAT);
            fmt.order = EChannelOrder(iformat.image_channel_order);
            fmt.type = EChannelType(iformat.image_channel_data_type);
        }
        return fmt;
    }

    int Image2D::width() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_WIDTH));
    }

    int Image2D::height() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_HEIGHT));
    }

    int Image2D::bytesPerElement() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_ELEMENT_SIZE));
    }

    int Image2D::bytesPerLine() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_ROW_PITCH));
    }

    Image3D::Image3D(const Image3D& other)
        : MemoryObject(), fmt(other.fmt)
    {
        setMemoryId(other._ctx, other._id);
    }

    Image3D& Image3D::operator=(const Image3D& other)
    {
        fmt = other.fmt;
        setMemoryId(other._ctx, other._id);
        return *this;
    }

    ImageFormat Image3D::format() const
    {
        if(fmt.isNull())
        {
            cl_image_format iformat = detail::imageInfo<cl_image_format>
                (memoryId(), CL_IMAGE_FORMAT);
            fmt.order = EChannelOrder(iformat.image_channel_order);
            fmt.type = EChannelType(iformat.image_channel_data_type);
        }
        return fmt;
    }

    int Image3D::width() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_WIDTH));
    }

    int Image3D::height() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_HEIGHT));
    }

    int Image3D::depth() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_DEPTH));
    }

    int Image3D::bytesPerElement() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_ELEMENT_SIZE));
    }

    int Image3D::bytesPerLine() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_ROW_PITCH));
    }

    int Image3D::bytesPerSlice() const
    {
        return int(detail::imageInfo<size_t>(memoryId(), CL_IMAGE_SLICE_PITCH));
    }
}
