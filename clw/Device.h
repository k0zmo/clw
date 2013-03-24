#pragma once

#include "Prerequisites.h"

namespace clw
{
    enum EDeviceType
    {
        Default       = (1 << 0),
        Cpu           = (1 << 1),
        Gpu           = (1 << 2),
        Accelerator   = (1 << 3),
        Custom        = (1 << 4),
        All           =  0xFFFFFFFF
    };

    enum ECacheType
    {
        Cache_NoChache        = 0,
        Cache_ReadOnlyCache   = 1,
        Cache_ReadWriteCache  = 2
    };

    enum EFloatCapabilities
    {
        Capability_NotSupported               = 0,
        Capability_Denorm                     = (1 << 0),
        Capability_InfinityAndNaN             = (1 << 1),
        Capability_RoundToNearest             = (1 << 2),
        Capability_RoundToZero                = (1 << 3),
        Capability_RoundToInf                 = (1 << 4),
        Capability_FusedMultiplyAdd           = (1 << 5),
        // Supported in OpenCL 1.2+
        Capability_SoftwareFloat              = (1 << 6),
        Capability_CorrectlyRoundedDivideSqrt = (1 << 7)
    };

    class Device
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

        EFloatCapabilities floatCapabilities() const;
        EFloatCapabilities doubleCapabilities() const;
        EFloatCapabilities halfCapabilities() const;

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

    vector<Device> allDevices();
    vector<Device> devices(EDeviceType deviceTypes, const Platform& platform);
}
