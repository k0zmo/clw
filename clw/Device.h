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
		Device() : id(0) {}
		Device(cl_device_id id) : id(id) {}

		bool isNull() const { return id == 0; }
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
		cl_ulong profilingTimerOffset() const;
		string boardName() const;
		vector<size_t> globalFreeMemory() const;
		cl_uint simdPerComputeUnit() const;
		cl_uint simdWidth() const;
		cl_uint simdInstructionWidth() const;
		cl_uint wavefrontWidth() const;
		cl_uint globalMemoryChannels() const;
		cl_uint globalMemoryChannelBanks() const;
		cl_uint globalMemoryChannelBankWidth() const;
		cl_uint localMemorySizePerComputeUnit() const;
		cl_uint localMemoryBanks() const;

		// NVIDIA only
		cl_uint computeCapabilityMajor() const;
		cl_uint computeCapabilityMinor() const;
		cl_uint registersPerBlock() const;
		cl_uint warpSize() const;
		bool gpuOverlap() const;
		bool kernelExecutionTimeout() const;
		bool integratedMemory() const;

		cl_device_id deviceId() const { return id; }

		//! TODO: Device fission (OpenCL 1.2 or 1.1 with extensions)
	private:
		cl_device_id id;
	};

	vector<Device> allDevices();
	vector<Device> devices(EDeviceType deviceTypes, const Platform& platform);
}
