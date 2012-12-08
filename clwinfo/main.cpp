#include <clw/clw.h>
#include <clw/StlUtils.h>
#include <iostream>
#include <string>

std::string deviceTypeName(clw::EDeviceType type)
{
	switch(type)
	{
	case clw::Cpu: return std::string("Cpu");
	case clw::Gpu: return std::string("Gpu");
	case clw::Accelerator: return std::string("Accelerator"); 
	case clw::Custom: return std::string("Custom");
	default: return std::string("Undefined");
	}
}

std::string boolName(bool v)
{
	return v ? std::string("yes") : std::string("no");
}

std::string memorySizeName(size_t memSize)
{
#if defined(_MSC_VER)
#  define snprintf _snprintf
#endif
	char buf[64];
	if(memSize >= 1024 * 1024)
		snprintf(buf, sizeof(buf)/sizeof(char), "%d MB", int(memSize / (1024 * 1024)));
	else
		snprintf(buf, sizeof(buf)/sizeof(char), "%d kB", int(memSize / 1024));
	return std::string(buf);
}

int main()
{
	auto platforms = clw::availablePlatforms();
	clw::for_each(platforms, [&](const clw::Platform& platform)
	{
		std::cout << "OpenCL Platform:\n";
		std::cout << "    Profile          : " << platform.profile() << std::endl;
		std::cout << "    Version          : " << platform.versionString() << std::endl;
		std::cout << "    Name             : " << platform.name() << std::endl;
		std::cout << "    Vendor           : " << platform.vendor() << std::endl;
		std::cout << "    Extension suffix : " << platform.extensionSuffix() << std::endl;

		auto exts = platform.extensions();
		std::cout << "    Extensions       : (" << exts.size() << ")\n";
		clw::for_each(exts, [&](const std::string& extension)
		{
			std::cout << "        " << extension << std::endl;
		});
		std::cout << "\n";

		auto devices = clw::devices(clw::All, platform);
		clw::for_each(devices, [&](const clw::Device& device)
		{
			std::cout << "OpenCL Device:\n";
			std::cout << "    Name                    : " << device.name() << std::endl;
			std::cout << "    Vendor                  : " << device.vendor() << std::endl;
			std::cout << "    Device type             : " << deviceTypeName(device.deviceType()) << std::endl;											  
			std::cout << "    Driver version          : " << device.driverVersion() << std::endl;
			std::cout << "    Language Version        : " << device.languageVersion() << std::endl;
			std::cout << "    Supports Double         : " << boolName(device.supportsDouble()) << std::endl;
			std::cout << "    Supports Half Float     : " << boolName(device.supportsHalf()) << std::endl;

			// Radeons specific device attributes
			if(device.vendor() == std::string("Advanced Micro Devices, Inc.") &&
			   device.deviceType() == clw::Gpu)
			{
				auto gmemFree = device.globalFreeMemory();
				std::cout << "    Board name              : " << device.boardName() << std::endl;
				if(gmemFree.size() == 2)
					std::cout << "    Free GMEM               : " << memorySizeName(gmemFree[1] * 1024) 
					          << "/" << memorySizeName(gmemFree[0] * 1024) << std::endl;
				std::cout << "    SIMD per Compute Unit   : " << device.simdPerComputeUnit() << std::endl;
				std::cout << "    SIMD width              : " << device.simdWidth() << std::endl;
				std::cout << "    SIMD Instruction width  : " << device.simdInstructionWidth() << std::endl;
				std::cout << "    Wavefront width         : " << device.wavefrontWidth() << std::endl;
				std::cout << "    GMEM channels           : " << device.globalMemoryChannels() << std::endl;
				std::cout << "    GMEM channel banks      : " << device.globalMemoryChannelBanks() << std::endl;
				std::cout << "    GMEM channel bank width : " << device.globalMemoryChannelBankWidth() << std::endl;											     
				std::cout << "    LMEM size per CU        : " << device.localMemorySizePerComputeUnit() << std::endl;											     
				std::cout << "    LMEM memory banks       : " << device.localMemoryBanks() << std::endl;				
			}
			else if(device.vendor() == std::string("NVIDIA") &&
			        device.deviceType() == clw::Gpu)
			{
				std::cout << "    Compute Capability       : " << device.computeCapabilityMajor() << "." << device.computeCapabilityMinor() << std::endl;
				std::cout << "    Registers per Block      : " << device.registersPerBlock() << std::endl;
				std::cout << "    Warp size                : " << device.warpSize() << std::endl;
				std::cout << "    GPU overlap              : " << device.gpuOverlap() << std::endl;
				std::cout << "    Kernel execution timeout : " << device.kernelExecutionTimeout() << std::endl;
				std::cout << "    Integrated memory        : " << device.integratedMemory() << std::endl;
			}

			auto exts = device.extensions();
			std::cout << "    Extensions              : (" << exts.size() << ")\n";
			clw::for_each(exts, [&](const std::string& extension)
			{
				std::cout << "        " << extension << std::endl;
			});

			std::cout << "\n";
		});
		std::cout << "\n";
	});
	return 0;
}