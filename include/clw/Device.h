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
#include "clw/EnumFlags.h"

namespace clw
{
    enum class EDeviceType : unsigned
    {
        Default       = (1 << 0),
        Cpu           = (1 << 1),
        Gpu           = (1 << 2),
        Accelerator   = (1 << 3),
        Custom        = (1 << 4),
        All           =  0xFFFFFFFF
    };
    typedef EnumFlags<EDeviceType> DeviceFlags;
    CLW_DEFINE_ENUMFLAGS_OPERATORS(DeviceFlags)

    enum class ECacheType
    {
        NoChache        = 0,
        ReadOnlyCache   = 1,
        ReadWriteCache  = 2
    };

    enum class EFloatCaps
    {
        NotSupported               = 0,
        Denorm                     = (1 << 0),
        InfinityAndNaN             = (1 << 1),
        RoundToNearest             = (1 << 2),
        RoundToZero                = (1 << 3),
        RoundToInf                 = (1 << 4),
        FusedMultiplyAdd           = (1 << 5),
        // Supported in OpenCL 1.2+
        SoftwareFloat              = (1 << 6),
        CorrectlyRoundedDivideSqrt = (1 << 7)
    };
    typedef EnumFlags<EFloatCaps> FloatCapsFlags;
    CLW_DEFINE_ENUMFLAGS_OPERATORS(FloatCapsFlags)

    class CLW_EXPORT Device
    {
    public:
        Device() : _id(0) {}
        Device(cl_device_id id) : _id(id) {}

        bool isNull() const { return _id == 0; }
        EDeviceType deviceType() const;
        Platform platform() const;

        string name() const;
        string version() const;
        string vendor() const;
        string driverVersion() const;
        string languageVersion() const;
        string profile() const;
        vector<string> extensions() const;
       
        bool supportsExtension(const char* ext) const;
        bool supportsDouble() const;
        bool supportsHalf() const;
        bool supportsErrorCorrection() const;
        bool supportsNativeKernel() const;
        bool supportsOutOfOrderExecution() const;
        bool supportsImages() const;

        bool isAvailable() const;
        bool isCompilerAvailable() const;
        bool isUnifiedMemory() const;
        bool isLocalMemorySeparate() const;
        int addressBitness() const;
        bool isLittleEndian() const;
        int clockFrequency() const;
        int computeUnits() const;
        int defaultAlignment() const;
        int minimumAlignment() const;

        FloatCapsFlags floatCapabilities() const;
        FloatCapsFlags doubleCapabilities() const;
        FloatCapsFlags halfCapabilities() const;

        uint64_t globalMemoryCacheSize() const;
        int globalMemoryCacheLineSize() const;
        ECacheType globalMemoryCacheType() const;
        uint64_t globalMemorySize() const;
        uint64_t localMemorySize() const;
        uint64_t maximumAllocationSize() const;

        Grid maximumImage2DSize() const;
        Grid maximumImage3DSize() const;
        int maximumReadImages() const;
        int maximumWriteImages() const;
        int maximumSamplers() const;

        int maximumConstantArguments() const;
        uint64_t maximumConstantBufferSize() const;
        size_t maximumParameterBytes() const;

        Grid maximumWorkItemSize() const;
        size_t maximumWorkItemsPerGroup() const;

        size_t profilingTimerResolution() const;

        int preferredCharVectorWidth() const;
        int preferredShortVectorWidth() const;
        int preferredIntVectorWidth() const;
        int preferredLongVectorWidth() const;
        int preferredFloatVectorWidth() const;
        int preferredDoubleVectorWidth() const;
        int preferredHalfVectorWidth() const;

        int nativeCharVectorWidth() const;
        int nativeShortVectorWidth() const;
        int nativeIntVectorWidth() const;
        int nativeLongVectorWidth() const;
        int nativeFloatVectorWidth() const;
        int nativeDoubleVectorWidth() const;
        int nativeHalfVectorWidth() const;

        // AMD only
        uint64_t profilingTimerOffset() const;
        string boardName() const;
        vector<size_t> globalFreeMemory() const;
        int simdPerComputeUnit() const;
        int simdWidth() const;
        int simdInstructionWidth() const;
        int wavefrontWidth() const;
        int globalMemoryChannels() const;
        int globalMemoryChannelBanks() const;
        int globalMemoryChannelBankWidth() const;
        int localMemorySizePerComputeUnit() const;
        int localMemoryBanks() const;

        // NVIDIA only
        int computeCapabilityMajor() const;
        int computeCapabilityMinor() const;
        int registersPerBlock() const;
        int warpSize() const;
        bool gpuOverlap() const;
        bool kernelExecutionTimeout() const;
        bool integratedMemory() const;

        cl_device_id deviceId() const { return _id; }

        //! TODO: Device fission (OpenCL 1.2 or 1.1 with extensions)
    private:
        cl_device_id _id;
    };

    CLW_EXPORT vector<Device> allDevices();
    CLW_EXPORT vector<Device> devices(DeviceFlags deviceTypes, const Platform& platform);
}
