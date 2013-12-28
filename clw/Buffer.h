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
#include "MemoryObject.h"

namespace clw
{
    class CLW_EXPORT Buffer : public MemoryObject
    {
    public:
        Buffer() {}
        Buffer(Context* ctx, cl_mem id)
            : MemoryObject(ctx, id) {}

        Buffer(const Buffer& other);
        Buffer& operator=(const Buffer& other);

        Buffer createSubBuffer(size_t offset, size_t size, EAccess access);
        Buffer parentBuffer() const;
        size_t offset() const;
    };
}