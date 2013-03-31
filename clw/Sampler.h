#pragma once

#include "Prerequisites.h"

namespace clw
{
    enum EFilterMode
    {
        Filter_Nearest = 0x1140,
        Filter_Linear  = 0x1141
    };

    enum EAddressingMode
    {
        Addressing_None             = 0x1130,
        Addressing_ClampToEdge    = 0x1131,
        Addressing_Clamp            = 0x1132,
        Addressing_Repeat           = 0x1133,
        Addressing_MirroredRepeat  = 0x1134
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