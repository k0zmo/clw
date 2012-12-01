#include "Device.h"
#include "Platform.h"
#include "StlUtils.h"

#include <cstring>

namespace clw 
{
	namespace detail
	{
		template<typename Value>
		Value deviceInfo(cl_device_id id, cl_device_info info)
		{
			Value value;
			cl_int error = CL_SUCCESS;
			if(!id || (error = clGetDeviceInfo(id, info, 
				    sizeof(Value), &value, nullptr)) != CL_SUCCESS)
			{
				reportError("deviceInfo(): ", error);
				return Value(0);
			}
			return value;
		}

		template<>
		bool deviceInfo(cl_device_id id, cl_device_info info)
		{
			cl_bool value = deviceInfo<cl_bool>(id, info);
			return value != 0;
		}

		template<typename Value>
		vector<Value> deviceInfoVector(cl_device_id id, cl_device_info info)
		{
			size_t size;
			cl_int error = CL_SUCCESS;
			if(!id || (error = clGetDeviceInfo(id, info, 0, nullptr, &size))
			        != CL_SUCCESS)
			{
				reportError("deviceInfoVector(): ", error);
				return vector<Value>();
			}
			size_t num(size / sizeof(Value));
			vector<Value> buf(num);
			if((error = clGetDeviceInfo(id, info, size, buf.data(), nullptr))
			        != CL_SUCCESS)
			{
				reportError("deviceInfoVector(): ", error);	
				return vector<Value>();
			}
			return buf;
		}

		template<>
		string deviceInfo(cl_device_id id, cl_device_info info)
		{
			return string(deviceInfoVector<char>(id, info).data());
		}
	}

	EDeviceType Device::deviceType() const
	{
		return EDeviceType(detail::deviceInfo<cl_device_type>
			(id, CL_DEVICE_TYPE));
	}

	Platform Device::platform() const
	{
		return Platform(detail::deviceInfo<cl_platform_id>
			(id, CL_DEVICE_PLATFORM));
	}

	bool Device::supportsExtension(const char* ext) const
	{
		string list = detail::deviceInfo<string>(id, CL_DEVICE_EXTENSIONS);
		return list.find(std::string(ext)) != std::string::npos;
	}

	bool Device::supportsDouble() const
	{
		return supportsExtension("cl_khr_fp64");
	}

	bool Device::supportsHalf() const
	{
		return supportsExtension("cl_khr_fp16");
	}

	string Device::version() const
	{
		return detail::deviceInfo<string>(id, CL_DEVICE_VERSION);
	}

	string Device::name() const
	{
		string devname = detail::deviceInfo<string>(id, CL_DEVICE_NAME);
		detail::trim(&devname, true, true);
		return devname;
	}

	string Device::vendor() const
	{
		return detail::deviceInfo<string>(id, CL_DEVICE_VENDOR);
	}

	string Device::driverVersion() const
	{
		return detail::deviceInfo<string>(id, CL_DRIVER_VERSION);
	}

	string Device::languageVersion() const
	{
		return detail::deviceInfo<string>(id, CL_DEVICE_OPENCL_C_VERSION);
	}

	vector<string> Device::extensions() const
	{
		string list = detail::deviceInfo<string>(id, CL_DEVICE_EXTENSIONS);
		return detail::tokenize(list, ' ');
	}

	vector<Device> allDevices()
	{
		vector<Platform> platforms = availablePlatforms();
		vector<Device> devices;
		clw::for_each(platforms, [&devices](const Platform& platform)
		{
			cl_uint size;
			cl_int error;
			if((error = clGetDeviceIDs(platform.platformId(), CL_DEVICE_TYPE_ALL,
					0, nullptr, &size)) != CL_SUCCESS)
			{
				detail::reportError("allDevices(): ", error);
				return; // works like continue in ordinary loop
			}
			vector<cl_device_id> buf(size);
			clGetDeviceIDs(platform.platformId(), CL_DEVICE_TYPE_ALL,
				size, buf.data(), &size);
			for(size_t i = 0; i < buf.size(); ++i)
				devices.push_back(Device(buf[i]));
		});
		return devices;
	}

	vector<Device> devices(EDeviceType deviceTypes, const Platform& platform)
	{
		cl_uint size;
		cl_int error = CL_SUCCESS;
		if(platform.isNull() || 
			    (error = clGetDeviceIDs(platform.platformId(), deviceTypes, 
				               0, nullptr, &size)) != CL_SUCCESS)
		{
			detail::reportError("devices(): ", error);
			return vector<Device>();
		}
		vector<cl_device_id> buf(size);
		clGetDeviceIDs(platform.platformId(), deviceTypes,
		               size, buf.data(), &size);
		vector<Device> devs(size);
		for(size_t i = 0; i < buf.size(); ++i)
			devs[i] = Device(buf[i]);
		return devs;
	}

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
		return uint64_t(detail::deviceInfo<cl_ulong>(id, CL_DEVICE_PROFILING_TIMER_OFFSET_AMD));
	}

	string Device::boardName() const
	{
		return detail::deviceInfo<string>(id, CL_DEVICE_BOARD_NAME_AMD);
	}

	vector<size_t> Device::globalFreeMemory() const
	{
		return detail::deviceInfoVector<size_t>(id, CL_DEVICE_GLOBAL_FREE_MEMORY_AMD);
	}

	int Device::simdPerComputeUnit() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD));
	}

	int Device::simdWidth() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_SIMD_WIDTH_AMD));
	}

	int Device::simdInstructionWidth() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD));
	}

	int Device::wavefrontWidth() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_WAVEFRONT_WIDTH_AMD));
	}

	int Device::globalMemoryChannels() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD));
	}

	int Device::globalMemoryChannelBanks() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD));
	}

	int Device::globalMemoryChannelBankWidth() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD));
	}

	int Device::localMemorySizePerComputeUnit() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD));
	}

	int Device::localMemoryBanks() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_LOCAL_MEM_BANKS_AMD));
	}

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
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV));
	}

	int Device::computeCapabilityMinor() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV));
	}

	int Device::registersPerBlock() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_REGISTERS_PER_BLOCK_NV));
	}

	int Device::warpSize() const
	{
		return int(detail::deviceInfo<cl_uint>(id, CL_DEVICE_WARP_SIZE_NV));
	}

	bool Device::gpuOverlap() const
	{
		return detail::deviceInfo<bool>(id, CL_DEVICE_GPU_OVERLAP_NV);
	}

	bool Device::kernelExecutionTimeout() const
	{
		return detail::deviceInfo<bool>(id, CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV);
	}

	bool Device::integratedMemory() const
	{
		return detail::deviceInfo<bool>(id, CL_DEVICE_INTEGRATED_MEMORY_NV);
	}
}
