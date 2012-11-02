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
		All           =  0xFFFFFFFF,
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

		cl_device_id deviceId() const { return id; }
	private:
		cl_device_id id;
	};

	vector<Device> allDevices();
	vector<Device> devices(EDeviceType deviceTypes, const Platform& platform);
}