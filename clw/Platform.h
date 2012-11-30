#pragma once

#include "Prerequisites.h"

namespace clw
{
	enum EPlatformVersion
	{
		Version_1_0 = 1,
		Version_1_1 = 2,
		Version_1_2 = 3
	};

	enum EPlatformVendor
	{
		Vendor_AMD,
		Vendor_Intel,
		Vendor_NVIDIA,
		Vendor_Undefined
	};

	// Thin wrapper over OpenCL platform 
	class Platform
	{
	public:
		Platform() : id(0) {}
		Platform(cl_platform_id id) : id(id) {}

		bool isNull() const { return id == 0; }
		EPlatformVersion version() const;
		string versionString() const;
		string name() const;
		string vendor() const;
		string extensionSuffix() const;
		string profile() const;
		vector<string> extensions() const;

		// Not fully tested - cl_khr_icd must be supported
		inline EPlatformVendor vendorEnum() const;

		cl_platform_id platformId() const { return id; }

		// !TODO: unloadCompiler(); - only for 1.2

	private:
		cl_platform_id id;
	};

	vector<Platform> availablePlatforms();
	Platform defaultPlatform();

	inline EPlatformVendor Platform::vendorEnum() const
	{
		string suffix = extensionSuffix();
		if(suffix == "AMD")
			return Vendor_AMD;
		else if(suffix == "INTEL")
			return Vendor_Intel;
		else if(suffix == "NV")
			return Vendor_NVIDIA;
		else
			return Vendor_Undefined;
	}
}