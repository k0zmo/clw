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

    class Device
    {
    public:
        Device() : _id(0) {}
        Device(cl_device_id id) : _id(id) {}

        bool isNull() const { return _id == 0; }
        EDeviceType deviceType() const;
        Platform platform() const;

        string version() const;
        string name() const;
        string vendor() const;
        string driverVersion() const;
        string languageVersion() const;
        vector<string> extensions() const;

        bool supportsExtension(const char* ext) const;
        bool supportsDouble() const;
        bool supportsHalf() const;

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
