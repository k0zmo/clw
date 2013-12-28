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

#include "MemoryObject.h"
#include "details.h"

#if !defined(CL_MEM_USE_PERSISTENT_MEM_AMD)
#  define CL_MEM_USE_PERSISTENT_MEM_AMD 0x40
#endif

namespace clw
{
    namespace detail
    {
        template<typename Value>
        Value memObjectInfo(cl_mem id, cl_mem_info info)
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

    MemoryObject::~MemoryObject()
    {
        if(_id)
            clReleaseMemObject(_id);
    }

    EAccess MemoryObject::access() const
    {
        return EAccess(flags() & 
            (CL_MEM_READ_WRITE |
             CL_MEM_WRITE_ONLY |
             CL_MEM_READ_ONLY)
        );
    }

    EMemoryLocation MemoryObject::memoryLocation() const
    {
        return EMemoryLocation(flags() & 
            (CL_MEM_USE_HOST_PTR |
             CL_MEM_ALLOC_HOST_PTR |
             CL_MEM_USE_PERSISTENT_MEM_AMD)
        );
    }

    EObjectType MemoryObject::type() const
    {
        return EObjectType(detail::memObjectInfo<cl_mem_object_type>(_id, CL_MEM_TYPE));
    }

    cl_mem_flags MemoryObject::flags() const
    {
        return detail::memObjectInfo<cl_mem_flags>(_id, CL_MEM_FLAGS);
    }

    void* MemoryObject::hostPointer() const
    {
        return detail::memObjectInfo<void*>(_id, CL_MEM_HOST_PTR);
    }

    size_t MemoryObject::size() const
    {
        return detail::memObjectInfo<size_t>(_id, CL_MEM_SIZE);
    }

    void MemoryObject::setMemoryId(Context* ctx, cl_mem id)
    {
        _ctx = ctx;
        if (id)
            clRetainMemObject(id);
        if (_id)
            clReleaseMemObject(_id);
        _id = id;
    }
}
