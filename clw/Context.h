#pragma once

#include "Prerequisites.h"
#include "Device.h"
#include "Sampler.h"
#include "MemoryObject.h"

namespace clw
{
	class Context
	{
	public:
		Context();
		~Context();

		bool create(EDeviceType type = EDeviceType::Default);
		bool create(const vector<Device>& devices);
		void release();

		cl_int lastError() const { return eid; }
		void setLastError(cl_int error) { eid = error; }

		CommandQueue createCommandQueue(cl_command_queue_properties properties,
		                                const Device& device);

		Buffer createBuffer(EAccess access,
		                    EMemoryLocation location,
		                    size_t size,
		                    const void* data = nullptr);

		Image2D createImage2D(EAccess access, 
		                      EMemoryLocation location,
		                      const ImageFormat& format,
		                      size_t width, 
		                      size_t height,
		                      const void* data = nullptr);

		Sampler createSampler(bool normalizedCoords, 
		                      EAddressingMode addressingMode, 
		                      EFilterMode filterMode);

		vector<ImageFormat> supportedImage2DFormats() const;
		vector<ImageFormat> supportedImage3DFormats() const;

		size_t numDevices() const { return devs.size(); }
		const vector<Device>& devices() const { return devs; }
		cl_context contextId() const { return id; }

		Program createProgramFromSourceCode(const string& sourceCode);
		Program createProgramFromSourceFile(const string& fileName);

		Program buildProgramFromSourceCode(const string& sourceCode,
			                               const string& options = string());
		Program buildProgramFromSourceFile(const string& fileName, 
		                                   const string& options = string());

	private:
		cl_context id;
		bool isCreated;
		cl_int eid;
		vector<Device> devs;
	};
}