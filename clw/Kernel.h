#pragma once

#include "Prerequisites.h"
#include "Grid.h"

#include "Buffer.h"
#include "Image.h"
#include "Sampler.h"
#include "Event.h"
#include "Device.h"
#include "details.h"

namespace clw
{
    enum EKernelArgumentAddressQualifier
    {
        AddressQ_Invalid  = 0x0000,
        AddressQ_Global   = 0x119B,
        AddressQ_Local    = 0x119C,
        AddressQ_Constant = 0x119D,
        AddressQ_Private  = 0x119E
    };

    enum EKernelArgumentAccessQualifier
    {
        AccessQ_Invalid   = 0x0000,
        AccessQ_ReadOnly  = 0x11A0,
        AccessQ_WriteOnly = 0x11A1,
        AccessQ_ReadWrite = 0x11A2,
        AccessQ_None      = 0x11A3
    };

    enum EKernelArgumentTypeQualifier
    {
        TypeQ_None     =        0,
        TypeQ_Const    = (1 << 0),
        TypeQ_Restrict = (1 << 1),
        TypeQ_Volatile = (1 << 2)		
    };

    class CLW_EXPORT Kernel
    {
    public:
        Kernel() 
            : _ctx(nullptr)
            , _id(0)
            , _globalWorkOffset(0)
            , _globalWorkSize(1)
            , _localWorkSize(0)
        {}
        Kernel(Context* _ctx, cl_kernel _id)
            : _ctx(_ctx)
            , _id(_id)
            , _globalWorkOffset(0)
            , _globalWorkSize(1)
            , _localWorkSize(0)
        {}
        ~Kernel();

        Kernel(const Kernel& other);
        Kernel& operator=(const Kernel& other);

        bool isNull() const { return _id == 0; }

        Context* context() const { return _ctx; }
        cl_kernel kernelId() const { return _id; }

        Program program() const;
        string name() const;
        int argCount() const;

        int maximumWorkItemsPerGroup(const Device& device = Device()) const;
        Grid requiredWorkGroupSize(const Device& device = Device()) const;
        uint64_t localMemoryUsage(const Device& device = Device()) const;
        int preferredMultipleWorkGroupSize(const Device& device = Device()) const;
        uint64_t privateMemoryUsage(const Device& device = Device()) const;

        EKernelArgumentAddressQualifier argumentAddressQualifier(int index) const;
        EKernelArgumentAccessQualifier argumentAccessQualifier(int index) const;
        int argumentTypeQualifier(int index) const;
        string argumentTypeName(int index) const;
        string argumentName(int index) const;

        Grid globalWorkSize() const;
        void setGlobalWorkSize(const Grid& range);
        void setGlobalWorkSize(size_t width);
        void setGlobalWorkSize(size_t width, size_t height);
        void setGlobalWorkSize(size_t width, size_t height, size_t depth);

        void setRoundedGlobalWorkSize(const Grid& range);
        void setRoundedGlobalWorkSize(size_t width);
        void setRoundedGlobalWorkSize(size_t width, size_t height);
        void setRoundedGlobalWorkSize(size_t width, size_t height, size_t depth);

        Grid localWorkSize() const;
        void setLocalWorkSize(const Grid& range);
        void setLocalWorkSize(size_t width);
        void setLocalWorkSize(size_t width, size_t height);
        void setLocalWorkSize(size_t width, size_t height, size_t depth);

        Grid globalWorkOffset() const;
        void setGlobalWorkOffset(const Grid& range);
        void setGlobalWorkOffset(size_t width);
        void setGlobalWorkOffset(size_t width, size_t height);
        void setGlobalWorkOffset(size_t width, size_t height, size_t depth);

        template<typename Value> void setArg(unsigned index, const Value& value);
        void setArg(unsigned index, const void* data, size_t size);

        clw::Event operator()(CommandQueue& queue);
#if defined(CLW_VARIADIC_TEMPLATES_SUPPORTED)
        // Variadic version with arguments
        template <class... Args>
        clw::Event operator()(CommandQueue& queue, const Args&... args);

        template <class... Args>
        clw::Event operator()(CommandQueue& queue, const Grid& local,
            const Grid& global, const Args&... args);
#endif

    private:
        Context* _ctx;
        cl_kernel _id;

        Grid _globalWorkOffset;
        Grid _globalWorkSize;
        Grid _localWorkSize;
#if defined(CLW_VARIADIC_TEMPLATES_SUPPORTED)
        template <class Head, class... Tail>
        void setArgVariadic(unsigned& pos, const Head& head, const Tail&... tail);
        void setArgVariadic(unsigned& pos) { (void) pos; }; // terminator
#endif
    };

    // Thin wrapper for dynamically setting local memory size as kernel argument 
    class CLW_EXPORT LocalMemorySize
    {
    public:
        LocalMemorySize(size_t size)
            : _size(size)
        {}

        operator size_t() const { return _size; }
    private:
        size_t _size;
    };

    template<typename Value> inline void Kernel::setArg(unsigned index, const Value& value)
    {
        setArg(index, &value, sizeof(Value));
    }

    template<> inline void Kernel::setArg<Buffer>(unsigned index, const Buffer& buffer)
    {
        cl_mem mem = buffer.memoryId();
        setArg(index, &mem, sizeof(cl_mem));
    }

    template<> inline void Kernel::setArg<Image2D>(unsigned index, const Image2D& image2d)
    {
        cl_mem mem = image2d.memoryId();
        setArg(index, &mem, sizeof(cl_mem));
    }

    template<> inline void Kernel::setArg<Image3D>(unsigned index, const Image3D& image3d)
    {
        cl_mem mem = image3d.memoryId();
        setArg(index, &mem, sizeof(cl_mem));
    }

    template<> inline void Kernel::setArg<Sampler>(unsigned index, const Sampler& sampler)
    {
        cl_sampler samplerId = sampler.samplerId();
        setArg(index, &samplerId, sizeof(cl_sampler));
    }

    template<> inline void Kernel::setArg<LocalMemorySize>(unsigned index, const LocalMemorySize& localMemorySize)
    {
        setArg(index, nullptr, localMemorySize);
    }

    inline Grid Kernel::globalWorkSize() const
    {
        return _globalWorkSize;
    }

    inline void Kernel::setGlobalWorkSize(const Grid& range)
    { 
        _globalWorkSize = range;
    }

    inline void Kernel::setGlobalWorkSize(size_t width)
    {
        setGlobalWorkSize(Grid(width));
    }

    inline void Kernel::setGlobalWorkSize(size_t width, size_t height) 
    { 
        setGlobalWorkSize(Grid(width, height));
    }

    inline void Kernel::setGlobalWorkSize(size_t width, size_t height, size_t depth)
    {
        setGlobalWorkSize(Grid(width, height, depth));
    }

    inline void Kernel::setRoundedGlobalWorkSize(const Grid& range)
    {
        setGlobalWorkSize(range.roundTo(localWorkSize()));
    }

    inline void Kernel::setRoundedGlobalWorkSize(size_t width)
    {
        setRoundedGlobalWorkSize(Grid(width));
    }

    inline void Kernel::setRoundedGlobalWorkSize(size_t width, size_t height)
    {
        setRoundedGlobalWorkSize(Grid(width, height));
    }

    inline void Kernel::setRoundedGlobalWorkSize(size_t width, size_t height, size_t depth)
    {
        setRoundedGlobalWorkSize(Grid(width, height, depth));
    }

    inline Grid Kernel::localWorkSize() const
    {
        return _localWorkSize;
    }

    inline void Kernel::setLocalWorkSize(const Grid& range)
    { 
        _localWorkSize = range;
    }

    inline void Kernel::setLocalWorkSize(size_t width)
    {
        setLocalWorkSize(Grid(width));
    }

    inline void Kernel::setLocalWorkSize(size_t width, size_t height)
    {
        setLocalWorkSize(Grid(width, height));
    }

    inline void Kernel::setLocalWorkSize(size_t width, size_t height, size_t depth)
    {
        setLocalWorkSize(Grid(width, height, depth));
    }

    inline Grid Kernel::globalWorkOffset() const
    {
        return _globalWorkOffset;
    }

    inline void Kernel::setGlobalWorkOffset(const Grid& range)
    {
#if defined(HAVE_OPENCL_1_1)
        _globalWorkOffset = range;
#endif        
    }

    inline void Kernel::setGlobalWorkOffset(size_t width)
    {
        setGlobalWorkOffset(Grid(width));
    }

    inline void Kernel::setGlobalWorkOffset(size_t width, size_t height)
    {
        setGlobalWorkOffset(Grid(width, height));
    }

    inline void Kernel::setGlobalWorkOffset(size_t width, size_t height, size_t depth)
    {
        setGlobalWorkOffset(Grid(width, height, depth));
    }
#if defined(CLW_VARIADIC_TEMPLATES_SUPPORTED)
    template <class Head, class... Tail>
    void Kernel::setArgVariadic(unsigned& pos, const Head& head, const Tail&... tail)
    {
        setArg(pos, head);
        ++pos;
        setArgVariadic(pos, tail...);
    }

    template <class... Args>
    clw::Event Kernel::operator()(CommandQueue& queue, const Args&... args)
    {
        unsigned pos = 0;
        setArgVariadic(pos, args...);
        return (*this)(queue);
    }

    template <class... Args>
    clw::Event Kernel::operator()(CommandQueue& queue, const Grid& local,
                                  const Grid& global, const Args&... args)
    {
        unsigned pos = 0;
        setArgVariadic(pos, args...);
        setLocalWorkSize(local);
        setRoundedGlobalWorkSize(global);
        return (*this)(queue);
    }
#endif
}
