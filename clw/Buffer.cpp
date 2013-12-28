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

#include "Buffer.h"
#include "details.h"

namespace clw
{
    namespace detail
    {
        template<typename Value>
        Value bufferInfo(cl_mem id, cl_mem_info info)
        {
            Value value;
            cl_int error;
            if((error = clGetMemObjectInfo(id, info, sizeof(Value), 
                &value, nullptr)) != CL_SUCCESS)
            {
                reportError("memObjectInfo(): ", error);
                return Value(0);
            }
            return value;
        }
    }

    Buffer::Buffer(const Buffer& other)
        : MemoryObject()
    {
        setMemoryId(other._ctx, other._id);
    }

    Buffer& Buffer::operator=(const Buffer& other)
    {
        setMemoryId(other._ctx, other._id);
        return *this;
    }

    Buffer Buffer::createSubBuffer(size_t offset, size_t size, EAccess access)
    {
#if defined(HAVE_OPENCL_1_1)
        cl_int error;
        cl_buffer_region region = {
            /* .origin = */ offset,
            /* .size = */ size
        };
        cl_mem mem = clCreateSubBuffer(memoryId(), cl_mem_flags(access),
            CL_BUFFER_CREATE_TYPE_REGION, &region, &error);
        detail::reportError("Buffer::createSubBuffer(): ", error);
        return Buffer(context(), mem);
#else
        (void) offset;
        (void) size;
        (void) access;
        return Buffer();
#endif
    }

    Buffer Buffer::parentBuffer() const
    {
#if defined(HAVE_OPENCL_1_1)
        cl_mem parent = detail::bufferInfo<cl_mem>
            (memoryId(), CL_MEM_ASSOCIATED_MEMOBJECT);
        if(parent)
            clRetainMemObject(parent);
        return Buffer(_ctx, parent);
#else
        return Buffer();
#endif
    }

    size_t Buffer::offset() const
    {
#if defined(HAVE_OPENCL_1_1)
        return detail::bufferInfo<size_t>(memoryId(), CL_MEM_OFFSET);
#else
        return size_t(0);
#endif
    }
}
