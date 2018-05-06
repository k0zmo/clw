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

namespace clw
{
    enum class EAccess
    {
        ReadWrite          = 0x01,
        WriteOnly          = 0x02,
        ReadOnly           = 0x04,
    };

    enum class EObjectType
    {
        Buffer         = 0x10F0,
        Image2D        = 0x10F1,
        Image3D        = 0x10F2,
        Image2DArray   = 0x10F3,
        Image1D        = 0x10F4,
        Image1DArray   = 0x10F5,
        Image1DBuffer  = 0x10F6
    };

    enum class EMemoryLocation
    {
        Device             = 0x00,
        UseHostMemory      = 0x08,
        AllocHostMemory    = 0x10,
        // AMD Only - alloc from GPU's CPU visible heap
        UsePinnedMemory    = 0x40 
    };

    class CLW_EXPORT MemoryObject
    {
    public:
        bool isNull() const { return _id == 0; }
        cl_mem memoryId() const { return _id; }
        Context* context() const { return _ctx; }

        EAccess access() const;
        EMemoryLocation memoryLocation() const;
        EObjectType type() const;
        cl_mem_flags flags() const;
        void* hostPointer() const;
        size_t size() const;

    protected:
        // Disable instantiating base class 
        MemoryObject(Context* _ctx = nullptr) : _ctx(_ctx), _id(0) {}
        MemoryObject(Context* _ctx, cl_mem _id)
            : _ctx(_ctx), _id(_id) {}
        ~MemoryObject();

        void setMemoryId(Context* _ctx, cl_mem _id);

    protected:
        Context* _ctx;
        cl_mem _id;

    private:
        // Disable copying
        MemoryObject(const MemoryObject& other);
        MemoryObject& operator=(const MemoryObject& other);
    };
}
