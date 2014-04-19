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

#include "Sampler.h"
#include "details.h"

namespace clw
{
    namespace detail
    {
        template<typename Value>
        Value samplerInfo(cl_sampler id, cl_sampler_info info)
        {
            Value value;
            cl_int error = CL_SUCCESS;
            if(!id || (error = clGetSamplerInfo(id, info, 
                sizeof(Value), &value, nullptr)) != CL_SUCCESS)
            {
                reportError("samplerInfo(): ", error);
                return Value(0);
            }
            return value;
        }

        template<>
        bool samplerInfo(cl_sampler _id, cl_sampler_info info)
        {
            cl_bool value = samplerInfo<cl_bool>(_id, info);
            return value != 0;
        }
    }

    Sampler::~Sampler()
    {
        if(_id)
            clReleaseSampler(_id);
    }

    Sampler::Sampler(const Sampler& other)
        : _ctx(other._ctx), _id(other._id)
    {
        if(_id)
            clRetainSampler(_id);
    }

    Sampler& Sampler::operator=(const Sampler& other)
    {
        _ctx = other._ctx;
        if(other._id)
            clRetainSampler(other._id);
        if(_id)
            clReleaseSampler(_id);
        _id = other._id;
        return *this;
    }

    Sampler::Sampler(Sampler&& other)
        : _ctx(nullptr), _id(0)
    {
        *this = std::move(other);
    }

    Sampler& Sampler::operator=(Sampler&& other)
    {
        if(this != &other)
        {
            if(_id)
                clReleaseSampler(_id);
            _ctx = other._ctx;
            _id = other._id;
            other._ctx = nullptr;
            other._id = 0;
        }
        return *this;
    }

    EFilterMode Sampler::filterMode() const
    {
        return EFilterMode(detail::samplerInfo<cl_filter_mode>
            (_id, CL_SAMPLER_FILTER_MODE));
    }

    EAddressingMode Sampler::addressingMode() const
    {
        return EAddressingMode(detail::samplerInfo<cl_addressing_mode>
            (_id, CL_SAMPLER_ADDRESSING_MODE));
    }

    bool Sampler::normalizedCoords() const
    {
        return detail::samplerInfo<bool>(_id, CL_SAMPLER_NORMALIZED_COORDS);
    }
}