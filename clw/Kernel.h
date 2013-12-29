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
#include "Grid.h"
#include "Device.h"

#if defined(CLW_VARIADIC_TEMPLATES_SUPPORTED)
#  include "Event.h"
#endif

#include "KernelTypesTraits.h"
#include "EnumFlags.h"

namespace clw
{
    enum class EKernelArgumentAddressQualifier
    {
        Invalid  = 0x0000,
        Global   = 0x119B,
        Local    = 0x119C,
        Constant = 0x119D,
        Private  = 0x119E
    };

    enum class EKernelArgumentAccessQualifier
    {
        Invalid   = 0x0000,
        ReadOnly  = 0x11A0,
        WriteOnly = 0x11A1,
        ReadWrite = 0x11A2,
        None      = 0x11A3
    };

    enum class EKernelArgumentTypeQualifier
    {
        None     =        0,
        Const    = (1 << 0),
        Restrict = (1 << 1),
        Volatile = (1 << 2)		
    };
    typedef EnumFlags<EKernelArgumentTypeQualifier> KernelArgumentTypeQualifierFlags;
    CLW_DEFINE_ENUMFLAGS_OPERATORS(KernelArgumentTypeQualifierFlags)

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
        KernelArgumentTypeQualifierFlags argumentTypeQualifier(int index) const;
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

        template <typename T> 
        typename std::enable_if<detail::is_kernel_value_type<T>::value>::type
        setArg(unsigned index, T value);

        template <typename T>
        typename std::enable_if<detail::is_kernel_memory_object<T>::value>::type
        setArg(unsigned index, const T& memObject);

        template <typename T> 
        typename std::enable_if<detail::is_kernel_custom_arg<T>::value>::type
        setArg(unsigned index, const T& value);

        void setArg(unsigned index, const LocalMemorySize& localMemorySize);
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

    template <typename T> 
    typename std::enable_if<detail::is_kernel_value_type<T>::value>::type
    Kernel::setArg(unsigned index, T value)
    {
        setArg(index, &value, sizeof(T));
    }

    template <typename T>
    typename std::enable_if<detail::is_kernel_memory_object<T>::value>::type
    Kernel::setArg(unsigned index, const T& memObject)
    {
        cl_mem mem = memObject.memoryId();
        setArg(index, &mem, sizeof(cl_mem));
    }

    template <typename T> 
    typename std::enable_if<detail::is_kernel_custom_arg<T>::value>::type
    Kernel::setArg(unsigned index, const T& value)
    {
        setArg(index, &value, sizeof(T));
    }

    inline void Kernel::setArg(unsigned index, const LocalMemorySize& localMemorySize)
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
