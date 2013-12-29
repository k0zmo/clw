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

#include "Kernel.h"
#include "Program.h"
#include "CommandQueue.h"
#include "Device.h"
#include "EnumFlags.h"
#include "details.h"

namespace clw
{
    namespace detail
    {
        template<typename Value>
        Value kernelInfo(cl_kernel id, cl_kernel_info info)
        {
            Value value;
            cl_int error;
            if((error = clGetKernelInfo(id, info, sizeof(value),
                    &value, nullptr)) != CL_SUCCESS)
            {
                reportError("kernelInfo(): ", error);
                return Value(0);
            }
            return value;
        }

        template<typename Value>
        Value kernelWorkGroupInfo(cl_kernel id, cl_device_id device_id,
                                  cl_kernel_work_group_info info)
        {
            Value value;
            cl_int error;
            if((error = clGetKernelWorkGroupInfo(id, device_id, info, sizeof(value),
                    &value, nullptr)) != CL_SUCCESS)
            {
                reportError("kernelWorkGroupInfo(): ", error);
                return Value(0);
            }
            return value;
        }

#if defined(HAVE_OPENCL_1_2)
        template<typename Value>
        Value kernelWorkArgInfo(cl_kernel id, cl_uint arg_index,
                                cl_kernel_arg_info info)
        {
            Value value;
            cl_int error;
            if((error = clGetKernelArgInfo(id, arg_index, info, sizeof(value),
                    &value, nullptr)) != CL_SUCCESS)
            {
                reportError("kernelWorkArgInfo(): ", error);
                return Value(0);
            }
            return value;
        }

        template<>
        string kernelWorkArgInfo(cl_kernel id, cl_uint arg_index,
                                 cl_kernel_arg_info info)
        {
            size_t size;
            cl_int error = CL_SUCCESS;
            if(!id || (error = clGetKernelArgInfo(id, arg_index, 
                    info, 0, nullptr, &size)) != CL_SUCCESS)
            {
                reportError("kernelWorkArgInfo(): ", error);
                return string();
            }
            vector<char> infoBuf(size);
            if((error = clGetKernelArgInfo(id, arg_index, info,
                    size, infoBuf.data(), &size)) != CL_SUCCESS)
            {
                reportError("kernelWorkArgInfo(): ", error);
                return string();
            }
            return string(infoBuf.data());
        }
#endif

    }

    Kernel::~Kernel()
    {
        if(_id)
            clReleaseKernel(_id);
    }

    Kernel::Kernel(const Kernel& other)
        : _ctx(other._ctx)
        , _id(other._id)
        , _globalWorkOffset(other._globalWorkOffset)
        , _globalWorkSize(other._globalWorkSize)
        , _localWorkSize(other._localWorkSize)
    {
        if(_id)
            clRetainKernel(_id);
    }

    Kernel& Kernel::operator=(const Kernel& other)
    {
        _ctx = other._ctx;
        if(other._id)
            clRetainKernel(other._id);
        if(_id)
            clReleaseKernel(_id);
        _id = other._id;
        _globalWorkOffset = other._globalWorkOffset;
        _globalWorkSize = other._globalWorkSize;
        _localWorkSize = other._localWorkSize;
        return *this;
    }

    Program Kernel::program() const
    {
        if(isNull())
            return Program();
        cl_program pid = detail::kernelInfo<cl_program>(_id, CL_KERNEL_PROGRAM);
        if(!pid)
            return Program();
        clRetainProgram(pid);
        return Program(_ctx, pid);
    }

    string Kernel::name() const
    {
        if(isNull())
            return string();
        size_t size;
        cl_int error;
        if((error = clGetKernelInfo(_id, CL_KERNEL_FUNCTION_NAME,
                0, nullptr, &size)) != CL_SUCCESS || !size)
            return string();
        string n;
        n.resize(size);
        if((error = clGetKernelInfo(_id, CL_KERNEL_FUNCTION_NAME,
                size, const_cast<char*>(n.data()), nullptr)) != CL_SUCCESS)
            return string();
        return n;
    }

    int Kernel::argCount() const
    {
        if(isNull())
            return 0;
        return int(detail::kernelInfo<cl_uint>(_id, CL_KERNEL_NUM_ARGS));
    }

    int Kernel::maximumWorkItemsPerGroup(const Device& device) const
    {
        return detail::kernelWorkGroupInfo<size_t>(_id, device.deviceId(), CL_KERNEL_WORK_GROUP_SIZE);
    }

    Grid Kernel::requiredWorkGroupSize(const Device& device) const
    {
        size_t dims[3];
        cl_int error = CL_SUCCESS;
        if(!_id || (error = clGetKernelWorkGroupInfo(_id, device.deviceId(), 
                CL_KERNEL_COMPILE_WORK_GROUP_SIZE, sizeof(dims), 
                dims, nullptr)) != CL_SUCCESS)
        {
            detail::reportError("kernelWorkGroupInfo(): ", error);
            return Grid();
        }
        return Grid(dims[0], dims[1], dims[2]);
    }

    uint64_t Kernel::localMemoryUsage(const Device& device) const
    {
        return uint64_t(detail::kernelWorkGroupInfo<cl_ulong>
            (_id, device.deviceId(), CL_KERNEL_LOCAL_MEM_SIZE));
    }

    int Kernel::preferredMultipleWorkGroupSize(const Device& device) const
    {
        return int(detail::kernelWorkGroupInfo<size_t>
            (_id, device.deviceId(), CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE));
    }

    uint64_t Kernel::privateMemoryUsage(const Device& device) const
    {
        return uint64_t(detail::kernelWorkGroupInfo<cl_ulong>
            (_id, device.deviceId(), CL_KERNEL_PRIVATE_MEM_SIZE));
    }

    EKernelArgumentAddressQualifier Kernel::argumentAddressQualifier(int index) const
    {
#if defined(HAVE_OPENCL_1_2)
        return EKernelArgumentAddressQualifier(detail::kernelWorkArgInfo
            <cl_kernel_arg_address_qualifier>(_id, index, CL_KERNEL_ARG_ADDRESS_QUALIFIER));
#else
        (void) index;
        return EKernelArgumentAddressQualifier::Invalid;
#endif
    }

    EKernelArgumentAccessQualifier Kernel::argumentAccessQualifier(int index) const
    {
#if defined(HAVE_OPENCL_1_2)
        return EKernelArgumentAccessQualifier(detail::kernelWorkArgInfo
            <cl_kernel_arg_access_qualifier>(_id, index, CL_KERNEL_ARG_ACCESS_QUALIFIER));
#else
        (void) index;
        return EKernelArgumentAccessQualifier::Invalid;
#endif
    }

    KernelArgumentTypeQualifierFlags Kernel::argumentTypeQualifier(int index) const
    {
#if defined(HAVE_OPENCL_1_2)
        return KernelArgumentTypeQualifierFlags(
            static_cast<KernelArgumentTypeQualifierFlags::enum_type>(
                detail::kernelWorkArgInfo<cl_kernel_arg_type_qualifier>
                    (_id, index, CL_KERNEL_ARG_TYPE_QUALIFIER)));
#else
        (void) index;
        return EKernelArgumentTypeQualifier::None;
#endif
    }

    string Kernel::argumentTypeName(int index) const
    {
#if defined(HAVE_OPENCL_1_2)
        return detail::kernelWorkArgInfo<string>(_id, index, CL_KERNEL_ARG_TYPE_NAME);
#else
        (void) index;
        return string();
#endif
    }

    string Kernel::argumentName(int index) const
    {
#if defined(HAVE_OPENCL_1_2)
        return detail::kernelWorkArgInfo<string>(_id, index, CL_KERNEL_ARG_NAME);
#else
        (void) index;
        return string();
#endif
    }

    void Kernel::setArg(unsigned index, const void* data, size_t size)
    {
        cl_int error = clSetKernelArg(_id, index, size, data);
        detail::reportError("Kernel::setArg(): ", error);
    }

    clw::Event Kernel::operator()(CommandQueue& queue)
    {
        return queue.asyncRunKernel(*this);
    }
}
