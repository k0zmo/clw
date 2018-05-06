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
    enum class EFilterMode
    {
        Nearest = 0x1140,
        Linear  = 0x1141
    };

    enum class EAddressingMode
    {
        None           = 0x1130,
        ClampToEdge    = 0x1131,
        Clamp          = 0x1132,
        Repeat         = 0x1133,
        MirroredRepeat = 0x1134
    };

    class CLW_EXPORT Sampler
    {
    public:
        Sampler() : _ctx(nullptr), _id(0) {}
        Sampler(Context* ctx, cl_sampler id)
            : _ctx(ctx), _id(id) {}
        ~Sampler();

        Sampler(const Sampler& other);
        Sampler& operator=(const Sampler& other);

        Sampler(Sampler&& other);
        Sampler& operator=(Sampler&& other);

        bool isNull() const { return _id == 0; }
        
        Context* context() const { return _ctx; }
        cl_sampler samplerId() const { return _id; }

        EFilterMode filterMode() const;
        EAddressingMode addressingMode() const;
        bool normalizedCoords() const;

    private:
        Context* _ctx;
        cl_sampler _id;
    };
}