#pragma once

#include "Prerequisites.h"
#include "Grid.h"

#include "Buffer.h"
#include "Image.h"
#include "Sampler.h"

namespace clw
{
    class Kernel
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

        bool operator()(CommandQueue& queue);
#if !defined(_MSC_VER)
        // Variadic version with arguments
        template <class... Args>
        bool operator()(CommandQueue& queue, const Args&... args);
#endif
    private:
        Context* _ctx;
        cl_kernel _id;

        Grid _globalWorkOffset;
        Grid _globalWorkSize;
        Grid _localWorkSize;
#if !defined(_MSC_VER)
        template <class Head, class... Tail>
        void setArgVariadic(unsigned& pos, const Head& head, const Tail&... tail);
        void setArgVariadic(unsigned& pos) { (void) pos; }; // terminator
#endif
    };

    // Thin wrapper for dynamically setting local memory size as kernel argument 
    class LocalMemorySize
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
        cl_int error = clSetKernelArg(_id, index, sizeof(Value), &value);
        detail::reportError("Kernel::setArg(): ", error);
    }

    template<> inline void Kernel::setArg<Buffer>(unsigned index, const Buffer& buffer)
    {
        cl_mem mem = buffer.memoryId();
        cl_int error = clSetKernelArg(_id, index, sizeof(cl_mem), &mem);
        detail::reportError("Kernel::setArg(): ", error);
    }

    template<> inline void Kernel::setArg<Image2D>(unsigned index, const Image2D& image2d)
    {
        cl_mem mem = image2d.memoryId();
        cl_int error = clSetKernelArg(_id, index, sizeof(cl_mem), &mem);
        detail::reportError("Kernel::setArg(): ", error);
    }

    template<> inline void Kernel::setArg<Image3D>(unsigned index, const Image3D& image3d)
    {
        cl_mem mem = image3d.memoryId();
        cl_int error = clSetKernelArg(_id, index, sizeof(cl_mem), &mem);
        detail::reportError("Kernel::setArg(): ", error);
    }

    template<> inline void Kernel::setArg<Sampler>(unsigned index, const Sampler& sampler)
    {
        cl_sampler samplerId = sampler.samplerId();
        cl_int error = clSetKernelArg(_id, index, sizeof(cl_sampler), samplerId);
        detail::reportError("Kernel::setArg(): ", error);
    }

    template<> inline void Kernel::setArg<LocalMemorySize>(unsigned index, const LocalMemorySize& localMemorySize)
    {
        cl_int error = clSetKernelArg(_id, index, localMemorySize, nullptr);
        detail::reportError("Kernel::setArg(): ", error);
    }

    inline void Kernel::setArg(unsigned index, const void* data, size_t size)
    {
        cl_int error = clSetKernelArg(_id, index, size, data);
        detail::reportError("Kernel::setArg(): ", error);
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
        _globalWorkOffset = range;
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
#if !defined(_MSC_VER)
    template <class Head, class... Tail>
    void Kernel::setArgVariadic(unsigned& pos, const Head& head, const Tail&... tail)
    {
        setArg(pos, head);
        ++pos;
        setArgVariadic(pos, tail...);
    }

    template <class... Args>
    bool Kernel::operator()(CommandQueue& queue, const Args&... args)
    {
        unsigned pos = 0;
        setArgVariadic(pos, args...);
        return (*this)(std::forward<CommandQueue&>(queue));
    }
#endif
}
