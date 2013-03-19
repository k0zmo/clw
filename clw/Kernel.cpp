#include "Kernel.h"
#include "Program.h"
#include "CommandQueue.h"

namespace clw
{
    namespace detail
    {
        template<typename Value>
        Value kernelInfo(cl_kernel _id, cl_kernel_info info)
        {
            Value value;
            cl_int error;
            if((error = clGetKernelInfo(_id, info, sizeof(value),
                    &value, nullptr)) != CL_SUCCESS)
            {
                reportError("kernelInfo(): ", error);
                return Value(0);
            }
            return value;
        }
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

    bool Kernel::operator()(CommandQueue& queue)
    {
        return queue.runKernel(*this);
    }
}
