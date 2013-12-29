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

#include "Device.h"
#include "Platform.h"
#include "Grid.h"
#include "details.h"

#include <cstring>

namespace clw 
{
    namespace detail
    {
        template<typename Value>
        Value deviceInfo(cl_device_id _id, cl_device_info info)
        {
            Value value;
            cl_int error = CL_SUCCESS;
            if(!_id || (error = clGetDeviceInfo(_id, info, 
                    sizeof(Value), &value, nullptr)) != CL_SUCCESS)
            {
                reportError("deviceInfo(): ", error);
                return Value(0);
            }
            return value;
        }

        template<>
        bool deviceInfo(cl_device_id _id, cl_device_info info)
        {
            cl_bool value = deviceInfo<cl_bool>(_id, info);
            return value != 0;
        }

        template<typename Value>
        vector<Value> deviceInfoVector(cl_device_id _id, cl_device_info info)
        {
            size_t size;
            cl_int error = CL_SUCCESS;
            if(!_id || (error = clGetDeviceInfo(_id, info, 0, nullptr, &size))
                    != CL_SUCCESS)
            {
                reportError("deviceInfoVector(): ", error);
                return vector<Value>();
            }
            size_t num(size / sizeof(Value));
            vector<Value> buf(num);
            if((error = clGetDeviceInfo(_id, info, size, buf.data(), nullptr))
                    != CL_SUCCESS)
            {
                reportError("deviceInfoVector(): ", error);	
                return vector<Value>();
            }
            return buf;
        }

        template<>
        string deviceInfo(cl_device_id _id, cl_device_info info)
        {
            return string(deviceInfoVector<char>(_id, info).data());
        }
    }

    EDeviceType Device::deviceType() const
    {
        return EDeviceType(detail::deviceInfo<cl_device_type>
            (_id, CL_DEVICE_TYPE));
    }

    Platform Device::platform() const
    {
        return Platform(detail::deviceInfo<cl_platform_id>
            (_id, CL_DEVICE_PLATFORM));
    }

    string Device::name() const
    {
        string devname = detail::deviceInfo<string>(_id, CL_DEVICE_NAME);
        detail::trim(&devname, true, true);
        return devname;
    }

    string Device::version() const
    {
        return detail::deviceInfo<string>(_id, CL_DEVICE_VERSION);
    }

    string Device::vendor() const
    {
        return detail::deviceInfo<string>(_id, CL_DEVICE_VENDOR);
    }

    string Device::driverVersion() const
    {
        return detail::deviceInfo<string>(_id, CL_DRIVER_VERSION);
    }

    string Device::languageVersion() const
    {
        return detail::deviceInfo<string>(_id, CL_DEVICE_OPENCL_C_VERSION);
    }
    
    string Device::profile() const
    {
        return detail::deviceInfo<string>(_id, CL_DEVICE_PROFILE);
    }

    vector<string> Device::extensions() const
    {
        string list = detail::deviceInfo<string>(_id, CL_DEVICE_EXTENSIONS);
        return detail::tokenize(list, ' ');
    }

    bool Device::supportsExtension(const char* ext) const
    {
        string list = detail::deviceInfo<string>(_id, CL_DEVICE_EXTENSIONS);
        return list.find(string(ext)) != string::npos;
    }

    bool Device::supportsDouble() const
    {
        return supportsExtension("cl_khr_fp64");
    }

    bool Device::supportsHalf() const
    {
        return supportsExtension("cl_khr_fp16");
    }

    bool Device::supportsErrorCorrection() const
    {
        return detail::deviceInfo<bool>(_id, CL_DEVICE_ERROR_CORRECTION_SUPPORT);
    }

    bool Device::supportsNativeKernel() const
    {
        cl_device_exec_capabilities caps = detail::deviceInfo<cl_device_exec_capabilities>
            (_id, CL_DEVICE_EXECUTION_CAPABILITIES);
        return (caps & CL_EXEC_NATIVE_KERNEL) != 0;
    }

    bool Device::supportsOutOfOrderExecution() const
    {
        cl_command_queue_properties props = detail::deviceInfo<cl_command_queue_properties>
            (_id, CL_DEVICE_QUEUE_PROPERTIES);
        return (props & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) != 0;
    }

    bool Device::supportsImages() const
    {
        return detail::deviceInfo<bool>(_id, CL_DEVICE_IMAGE_SUPPORT);
    }

    bool Device::isAvailable() const
    {
        return detail::deviceInfo<bool>(_id, CL_DEVICE_AVAILABLE);
    }

    bool Device::isCompilerAvailable() const
    {
        return detail::deviceInfo<bool>(_id, CL_DEVICE_COMPILER_AVAILABLE);
    }

    bool Device::isUnifiedMemory() const
    {
        return detail::deviceInfo<bool>(_id, CL_DEVICE_HOST_UNIFIED_MEMORY);
    }

    bool Device::isLocalMemorySeparate() const
    {
        cl_device_local_mem_type mem_type = detail::deviceInfo<cl_device_local_mem_type>
            (_id, CL_DEVICE_LOCAL_MEM_TYPE);
        return mem_type == CL_LOCAL;
    }

    int Device::addressBitness() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_ADDRESS_BITS));
    }

    bool Device::isLittleEndian() const
    {
        return detail::deviceInfo<bool>(_id, CL_DEVICE_ENDIAN_LITTLE);
    }

    int Device::clockFrequency() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_MAX_CLOCK_FREQUENCY));
    }

    int Device::computeUnits() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_MAX_COMPUTE_UNITS));
    }

    int Device::defaultAlignment() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_MEM_BASE_ADDR_ALIGN));
    }

    int Device::minimumAlignment() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE));
    }

    FloatCapsFlags Device::floatCapabilities() const
    {
        cl_device_fp_config fp_config = detail::deviceInfo<cl_device_fp_config>
            (_id, CL_DEVICE_SINGLE_FP_CONFIG);
        return FloatCapsFlags(static_cast<FloatCapsFlags::enum_type>(fp_config));
    }

    FloatCapsFlags Device::doubleCapabilities() const
    {
        cl_device_fp_config fp_config = detail::deviceInfo<cl_device_fp_config>
            (_id, CL_DEVICE_DOUBLE_FP_CONFIG);
        return FloatCapsFlags(static_cast<FloatCapsFlags::enum_type>(fp_config));
    }

    FloatCapsFlags Device::halfCapabilities() const
    {
        cl_device_fp_config fp_config = detail::deviceInfo<cl_device_fp_config>
            (_id, CL_DEVICE_HALF_FP_CONFIG);
        return FloatCapsFlags(static_cast<FloatCapsFlags::enum_type>(fp_config));
    }

    uint64_t Device::globalMemoryCacheSize() const
    {
        return uint64_t(detail::deviceInfo<cl_ulong>(_id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE));
    }

    int Device::globalMemoryCacheLineSize() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE));
    }

    ECacheType Device::globalMemoryCacheType() const
    {
        cl_device_mem_cache_type mem_cache_type = detail::deviceInfo<cl_device_mem_cache_type>
            (_id, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE);
        return ECacheType(mem_cache_type);
    }

    uint64_t Device::globalMemorySize() const
    {
        return uint64_t(detail::deviceInfo<cl_ulong>(_id, CL_DEVICE_GLOBAL_MEM_SIZE));
    }

    uint64_t Device::localMemorySize() const
    {
        return uint64_t(detail::deviceInfo<cl_ulong>(_id, CL_DEVICE_LOCAL_MEM_SIZE));
    }

    uint64_t Device::maximumAllocationSize() const
    {
        return uint64_t(detail::deviceInfo<cl_ulong>(_id, CL_DEVICE_MAX_MEM_ALLOC_SIZE));
    }

    Grid Device::maximumImage2DSize() const
    {
        size_t maxWidth = detail::deviceInfo<size_t>(_id, CL_DEVICE_IMAGE2D_MAX_WIDTH);
        size_t maxHeight = detail::deviceInfo<size_t>(_id, CL_DEVICE_IMAGE2D_MAX_HEIGHT);
        return Grid(maxWidth, maxHeight, 1);
    }

    Grid Device::maximumImage3DSize() const
    {
        size_t maxWidth = detail::deviceInfo<size_t>(_id, CL_DEVICE_IMAGE3D_MAX_WIDTH);
        size_t maxHeight = detail::deviceInfo<size_t>(_id, CL_DEVICE_IMAGE3D_MAX_HEIGHT);
        size_t maxDepth = detail::deviceInfo<size_t>(_id, CL_DEVICE_IMAGE3D_MAX_DEPTH);
        return Grid(maxWidth, maxHeight, maxDepth);
    }

    int Device::maximumReadImages() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_MAX_READ_IMAGE_ARGS));
    }

    int Device::maximumWriteImages() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_MAX_WRITE_IMAGE_ARGS));
    }

    int Device::maximumSamplers() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_MAX_SAMPLERS));
    }

    int Device::maximumConstantArguments() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_MAX_CONSTANT_ARGS));
    }

    uint64_t Device::maximumConstantBufferSize() const
    {
        return uint64_t(detail::deviceInfo<cl_ulong>(_id, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE));
    }

    size_t Device::maximumParameterBytes() const
    {
        return detail::deviceInfo<size_t>(_id, CL_DEVICE_MAX_PARAMETER_SIZE);
    }

    Grid Device::maximumWorkItemSize() const
    {
        //cl_uint workDims = detail::deviceInfo<cl_uint>
        //    (_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
        size_t dims[3];
        cl_int error = CL_SUCCESS;
        if(!_id || (error = clGetDeviceInfo(_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, 
                sizeof(dims), dims, nullptr)) != CL_SUCCESS)
        {
            detail::reportError("deviceInfo(): ", error);
            return Grid();
        }
        return Grid(dims[0], dims[1], dims[2]);
    }

    size_t Device::maximumWorkItemsPerGroup() const
    {
        return detail::deviceInfo<size_t>(_id, CL_DEVICE_MAX_WORK_GROUP_SIZE);
    }
        
    size_t Device::profilingTimerResolution() const
    {
        return detail::deviceInfo<size_t>(_id, CL_DEVICE_PROFILING_TIMER_RESOLUTION);
    }

    int Device::preferredCharVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR));
    }

    int Device::preferredShortVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT));
    }

    int Device::preferredIntVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT));
    }

    int Device::preferredLongVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG));
    }

    int Device::preferredFloatVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT));
    }

    int Device::preferredDoubleVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE));
    }

    int Device::preferredHalfVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF));
    }

    int Device::nativeCharVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR));
    }

    int Device::nativeShortVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT));
    }

    int Device::nativeIntVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT));
    }

    int Device::nativeLongVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG));
    }

    int Device::nativeFloatVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT));
    }

    int Device::nativeDoubleVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE));
    }

    int Device::nativeHalfVectorWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF));
    }

    vector<Device> allDevices()
    {
        vector<Platform> platforms = availablePlatforms();
        vector<Device> devices;
        for(const Platform& platform : platforms)
        {
            cl_uint size;
            cl_int error;
            if((error = clGetDeviceIDs(platform.platformId(), CL_DEVICE_TYPE_ALL,
                    0, nullptr, &size)) != CL_SUCCESS)
            {
                detail::reportError("allDevices(): ", error);
                continue;
            }
            vector<cl_device_id> buf(size);
            clGetDeviceIDs(platform.platformId(), CL_DEVICE_TYPE_ALL,
                size, buf.data(), &size);
            for(size_t i = 0; i < buf.size(); ++i)
                devices.push_back(Device(buf[i]));
        }
        return devices;
    }

    vector<Device> devices(DeviceFlags deviceTypes, const Platform& platform)
    {
        cl_uint size;
        cl_int error = CL_SUCCESS;
        if(platform.isNull() || 
                (error = clGetDeviceIDs(platform.platformId(), deviceTypes.raw(), 
                               0, nullptr, &size)) != CL_SUCCESS)
        {
            detail::reportError("devices(): ", error);
            return vector<Device>();
        }
        vector<cl_device_id> buf(size);
        clGetDeviceIDs(platform.platformId(), deviceTypes.raw(),
                       size, buf.data(), &size);
        vector<Device> devs(size);
        for(size_t i = 0; i < buf.size(); ++i)
            devs[i] = Device(buf[i]);
        return devs;
    }

    /*
        AMD stuff only
    */

#if !defined(CL_DEVICE_PROFILING_TIMER_OFFSET_AMD)
#  define CL_DEVICE_PROFILING_TIMER_OFFSET_AMD        0x4036
#endif
#if !defined(CL_DEVICE_BOARD_NAME_AMD)
#  define CL_DEVICE_BOARD_NAME_AMD                    0x4038
#endif
#if !defined(CL_DEVICE_GLOBAL_FREE_MEMORY_AMD)
#  define CL_DEVICE_GLOBAL_FREE_MEMORY_AMD            0x4039
#endif
#if !defined(CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD)
#  define CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD         0x4040
#endif
#if !defined(CL_DEVICE_SIMD_WIDTH_AMD)
#  define CL_DEVICE_SIMD_WIDTH_AMD                    0x4041
#endif
#if !defined(CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD)
#  define CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD        0x4042
#endif
#if !defined(CL_DEVICE_WAVEFRONT_WIDTH_AMD)
#  define CL_DEVICE_WAVEFRONT_WIDTH_AMD               0x4043
#endif
#if !defined(CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD)
#  define CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD           0x4044
#endif
#if !defined(CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD)
#  define CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD      0x4045
#endif
#if !defined(CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD)
#  define CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD 0x4046
#endif
#if !defined(CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD)
#  define CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD   0x4047
#endif
#if !defined(CL_DEVICE_LOCAL_MEM_BANKS_AMD)
#  define CL_DEVICE_LOCAL_MEM_BANKS_AMD               0x4048
#endif

    uint64_t Device::profilingTimerOffset() const
    {
        return uint64_t(detail::deviceInfo<cl_ulong>(_id, CL_DEVICE_PROFILING_TIMER_OFFSET_AMD));
    }

    string Device::boardName() const
    {
        return detail::deviceInfo<string>(_id, CL_DEVICE_BOARD_NAME_AMD);
    }

    vector<size_t> Device::globalFreeMemory() const
    {
        return detail::deviceInfoVector<size_t>(_id, CL_DEVICE_GLOBAL_FREE_MEMORY_AMD);
    }

    int Device::simdPerComputeUnit() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD));
    }

    int Device::simdWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_SIMD_WIDTH_AMD));
    }

    int Device::simdInstructionWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD));
    }

    int Device::wavefrontWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_WAVEFRONT_WIDTH_AMD));
    }

    int Device::globalMemoryChannels() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD));
    }

    int Device::globalMemoryChannelBanks() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD));
    }

    int Device::globalMemoryChannelBankWidth() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD));
    }

    int Device::localMemorySizePerComputeUnit() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD));
    }

    int Device::localMemoryBanks() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_LOCAL_MEM_BANKS_AMD));
    }

    /*
        NVIDIA stuff only
    */

#if !defined(CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV)
#  define CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV       0x4000
#endif
#if !defined(CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV)
#  define CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV       0x4001
#endif
#if !defined(CL_DEVICE_REGISTERS_PER_BLOCK_NV)
#  define CL_DEVICE_REGISTERS_PER_BLOCK_NV            0x4002
#endif
#if !defined(CL_DEVICE_WARP_SIZE_NV)
#  define CL_DEVICE_WARP_SIZE_NV                      0x4003
#endif
#if !defined(CL_DEVICE_GPU_OVERLAP_NV)
#  define CL_DEVICE_GPU_OVERLAP_NV                    0x4004
#endif
#if !defined(CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV)
#  define CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV            0x4005
#endif
#if !defined(CL_DEVICE_INTEGRATED_MEMORY_NV)
#  define CL_DEVICE_INTEGRATED_MEMORY_NV              0x4006
#endif

    int Device::computeCapabilityMajor() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV));
    }

    int Device::computeCapabilityMinor() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV));
    }

    int Device::registersPerBlock() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_REGISTERS_PER_BLOCK_NV));
    }

    int Device::warpSize() const
    {
        return int(detail::deviceInfo<cl_uint>(_id, CL_DEVICE_WARP_SIZE_NV));
    }

    bool Device::gpuOverlap() const
    {
        return detail::deviceInfo<bool>(_id, CL_DEVICE_GPU_OVERLAP_NV);
    }

    bool Device::kernelExecutionTimeout() const
    {
        return detail::deviceInfo<bool>(_id, CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV);
    }

    bool Device::integratedMemory() const
    {
        return detail::deviceInfo<bool>(_id, CL_DEVICE_INTEGRATED_MEMORY_NV);
    }
}
