#include "Sampler.h"
#include "details.h"

namespace clw
{
    namespace detail
    {
        template<typename Value>
        Value samplerInfo(cl_sampler _id, cl_sampler_info info)
        {
            Value value;
            cl_int error = CL_SUCCESS;
            if(!_id || (error = clGetSamplerInfo(_id, info, 
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