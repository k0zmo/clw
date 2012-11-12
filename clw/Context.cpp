#include "Context.h"
#include "Platform.h"
#include "CommandQueue.h"
#include "Program.h"
#include "Buffer.h"
#include "Image.h"

#include <iostream>

#if !defined(CL_CONTEXT_OFFLINE_DEVICES_AMD)
#define CL_CONTEXT_OFFLINE_DEVICES_AMD 0x403F
#endif

namespace clw
{
	namespace detail
	{
		extern "C" void CL_API_CALL contextNotify(const char* errInfo,
		                                                 const void* privateInfo,
		                                                 size_t cb,
		                                                 void* userData)
		{
			(void) privateInfo;
			(void) cb;
			(void) userData;
			std::cerr << "Context notification: " << errInfo << std::endl;
		}

		string errorName(cl_int eid)
		{
			#define CASE(X) case X: return string(#X);
			switch(eid)
			{
			CASE(CL_SUCCESS);
			CASE(CL_DEVICE_NOT_FOUND);
			CASE(CL_DEVICE_NOT_AVAILABLE);
			CASE(CL_COMPILER_NOT_AVAILABLE);
			CASE(CL_MEM_OBJECT_ALLOCATION_FAILURE);
			CASE(CL_OUT_OF_RESOURCES);
			CASE(CL_OUT_OF_HOST_MEMORY);
			CASE(CL_PROFILING_INFO_NOT_AVAILABLE);
			CASE(CL_MEM_COPY_OVERLAP);
			CASE(CL_IMAGE_FORMAT_MISMATCH);
			CASE(CL_IMAGE_FORMAT_NOT_SUPPORTED);
			CASE(CL_BUILD_PROGRAM_FAILURE);
			CASE(CL_MAP_FAILURE);
			CASE(CL_MISALIGNED_SUB_BUFFER_OFFSET);
			CASE(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST);
			CASE(CL_INVALID_VALUE);
			CASE(CL_INVALID_DEVICE_TYPE);
			CASE(CL_INVALID_PLATFORM);
			CASE(CL_INVALID_DEVICE);
			CASE(CL_INVALID_CONTEXT);
			CASE(CL_INVALID_QUEUE_PROPERTIES);
			CASE(CL_INVALID_COMMAND_QUEUE);
			CASE(CL_INVALID_HOST_PTR);
			CASE(CL_INVALID_MEM_OBJECT);
			CASE(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
			CASE(CL_INVALID_IMAGE_SIZE);
			CASE(CL_INVALID_SAMPLER);
			CASE(CL_INVALID_BINARY);
			CASE(CL_INVALID_BUILD_OPTIONS);
			CASE(CL_INVALID_PROGRAM);
			CASE(CL_INVALID_PROGRAM_EXECUTABLE);
			CASE(CL_INVALID_KERNEL_NAME);
			CASE(CL_INVALID_KERNEL_DEFINITION);
			CASE(CL_INVALID_KERNEL);
			CASE(CL_INVALID_ARG_INDEX);
			CASE(CL_INVALID_ARG_VALUE);
			CASE(CL_INVALID_ARG_SIZE);
			CASE(CL_INVALID_KERNEL_ARGS);
			CASE(CL_INVALID_WORK_DIMENSION);
			CASE(CL_INVALID_WORK_GROUP_SIZE);
			CASE(CL_INVALID_WORK_ITEM_SIZE);
			CASE(CL_INVALID_GLOBAL_OFFSET);
			CASE(CL_INVALID_EVENT_WAIT_LIST);
			CASE(CL_INVALID_EVENT);
			CASE(CL_INVALID_OPERATION);
			CASE(CL_INVALID_GL_OBJECT);
			CASE(CL_INVALID_BUFFER_SIZE);
			CASE(CL_INVALID_MIP_LEVEL);
			CASE(CL_INVALID_GLOBAL_WORK_SIZE);
			CASE(CL_INVALID_PROPERTY);
#ifdef HAVE_OPENCL_1_2
			CASE(CL_COMPILE_PROGRAM_FAILURE);
			CASE(CL_LINKER_NOT_AVAILABLE);
			CASE(CL_LINK_PROGRAM_FAILURE);
			CASE(CL_DEVICE_PARTITION_FAILED);
			CASE(CL_KERNEL_ARG_INFO_NOT_AVAILABLE);
			CASE(CL_INVALID_IMAGE_DESCRIPTOR);
			CASE(CL_INVALID_COMPILER_OPTIONS);
			CASE(CL_INVALID_LINKER_OPTIONS);
			CASE(CL_INVALID_DEVICE_PARTITION_COUNT);
#endif
			// cl_khr_icd extension
			CASE(CL_PLATFORM_NOT_FOUND_KHR);
			// cl_khr_gl_sharing
			CASE(CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
			default: return string("Error ") + std::to_string(
				static_cast<unsigned long long>(eid));
			}
			#undef CASE
		}

		void reportError(const char* name, cl_int eid)
		{
			if(eid != CL_SUCCESS)
				std::cerr << name << errorName(eid) << std::endl;;
		}

		vector<ImageFormat> supportedImageFormats(
			cl_context id,
			cl_mem_object_type image_type)
		{
			cl_uint num;
			cl_int error;
			if((error = clGetSupportedImageFormats(id, CL_MEM_READ_WRITE,
			        image_type, 0, nullptr, &num)) != CL_SUCCESS || !num)
			{
				reportError("supportedImageFormats() ", error);
				return vector<ImageFormat>();
			}
			vector<cl_image_format> buf(num);
			if((error = clGetSupportedImageFormats(id, CL_MEM_READ_WRITE,
			        image_type, num, buf.data(), nullptr)) != CL_SUCCESS)
			{
				reportError("supportedImageFormats() ", error);
				return vector<ImageFormat>();
			}
			vector<ImageFormat> imageFormats(num);
			for(cl_uint i = 0; i < num; ++i)
				imageFormats[i] = ImageFormat(
					EChannelOrder(buf[i].image_channel_order), 
					EChannelType(buf[i].image_channel_data_type));
			return imageFormats;
		}
	}

	Context::Context() 
		: id(0)
		, isCreated(false)
		, eid(CL_SUCCESS)
	{
	}

	Context::~Context()
	{
		if(isCreated && id != 0)
			clReleaseContext(id);
	}

	bool Context::create(EDeviceType type)
	{
		if(isCreated)
			return true;
		vector<Platform> pls = clw::availablePlatforms();
		for(size_t i = 0; i < pls.size(); ++i)
		{
			vector<Device> devices = clw::devices(type, pls[i]);
			for(size_t j = 0; j < devices.size(); ++j)
			{
				cl_device_id did = devices[j].deviceId();
				cl_context_properties props[] = {
					CL_CONTEXT_PLATFORM,
					cl_context_properties(pls[i].platformId()),
					0
				};
				if((id = clCreateContext(props, 1, &did, 
					    &detail::contextNotify, nullptr, &eid)) != 0)
				{
					devs.clear();
					devs.push_back(Device(did));
					isCreated = true;
					return true;
				}
				detail::reportError("Context::create(type): ", eid);
			}
		}
		isCreated = false;
		return false;		
	}

	bool Context::create(const vector<Device>& devices)
	{
		if(isCreated)
			return true;
		if(devices.empty())
			return false;
		vector<cl_device_id> dids(devices.size());
		for(size_t i = 0; i < devices.size(); ++i)
			dids[i] = devices[i].deviceId();
		cl_platform_id pid = devices[0].platform().platformId();
		cl_context_properties props[] = {
			CL_CONTEXT_PLATFORM, 
			cl_context_properties(pid),
			0
		};
		if((id = clCreateContext(props, dids.size(), dids.data(), 
		        &detail::contextNotify, nullptr, &eid)) != 0)
		{
			size_t size;
			if((eid = clGetContextInfo(id, CL_CONTEXT_DEVICES, 0,
			        nullptr, &size)) == CL_SUCCESS)
			{
				// !FIXME: assert(size == dids.size())
				devs = devices;
				isCreated = true;
				return true;
			}			
		}
		detail::reportError("Context::create(devices): ", eid);
		return false;
	}

	bool Context::createOffline(const Platform& platform)
	{
		if(isCreated)
			return true;
		cl_platform_id plid = platform.platformId();
		if(plid == 0)
			plid = clw::defaultPlatform().platformId();
		cl_context_properties props[] = {
			CL_CONTEXT_PLATFORM, 
			cl_context_properties(plid),
			CL_CONTEXT_OFFLINE_DEVICES_AMD,
			cl_context_properties(1),
			0
		};
		id = clCreateContextFromType
			(props, CL_DEVICE_TYPE_ALL, nullptr, nullptr, &eid);
		detail::reportError("Context::createOffline(devices): ", eid);
		if(eid == CL_SUCCESS)
		{
			size_t size;
			if((eid = clGetContextInfo(id, CL_CONTEXT_DEVICES, 0,
			        nullptr, &size)) == CL_SUCCESS)
			{
				int numDevices = size / sizeof(cl_device_id);
				vector<cl_device_id> devices_id(numDevices);
				clGetContextInfo(id, CL_CONTEXT_DEVICES, size,
					devices_id.data(), nullptr);
				devs.clear();
				for(int i = 0; i < numDevices; ++i)
					devs.push_back(Device(devices_id[i]));
				isCreated = true;
				return true;
			}
		}	
		return false;
	}

	void Context::release()
	{
		if(isCreated)
		{
			clReleaseContext(id);
			id = 0;
			isCreated = false;
		}
	}

	CommandQueue Context::createCommandQueue(cl_command_queue_properties properties,
	                                         const Device& device)
	{
		cl_command_queue cid = clCreateCommandQueue
			(id, device.deviceId(),
			properties, &eid);
		detail::reportError("Context::createCommandQueue(): ", eid);
		return cid ? CommandQueue(this, cid) : CommandQueue();
	}

	Buffer Context::createBuffer(EAccess access,
	                             EMemoryLocation location,
	                             size_t size,
	                             const void* data)
	{
		cl_mem_flags mem_flags = cl_mem_flags(access);
		if(data && location != Location_UseHostMemory)
			mem_flags |= CL_MEM_COPY_HOST_PTR;
		else
			data = nullptr;
		mem_flags |= cl_mem_flags(location);
		cl_mem bid = clCreateBuffer
			(id, mem_flags, size, const_cast<void*>(data), &eid);
		detail::reportError("Context::createBuffer(): ", eid);
		return bid ? Buffer(this, bid) : Buffer();
	}

	Image2D Context::createImage2D(EAccess access, 
	                               EMemoryLocation location,
	                               const ImageFormat& format,
	                               size_t width, 
	                               size_t height,
	                               const void* data)
	{
		cl_image_format image_format;
		image_format.image_channel_order = cl_channel_order(format.order);
		image_format.image_channel_data_type = cl_channel_type(format.type);
		cl_mem_flags mem_flags = cl_mem_flags(access);
		if(data && location != Location_UseHostMemory)
			mem_flags |= CL_MEM_COPY_HOST_PTR;
		else
			data = nullptr;
		mem_flags |= cl_mem_flags(location);
		cl_mem iid = clCreateImage2D
			(id, mem_flags, &image_format, 
			 width, height, 0, const_cast<void*>(data), &eid);
		detail::reportError("Context::createImage2D(): ", eid);
		return iid ? Image2D(this, iid) : Image2D();
	}

	Sampler Context::createSampler(bool normalizedCoords, 
		                           EAddressingMode addressingMode, 
		                           EFilterMode filterMode)
	{
		cl_sampler sampler = clCreateSampler
			(id, normalizedCoords ? CL_TRUE : CL_FALSE, 
			 cl_addressing_mode(addressingMode), 
			 cl_filter_mode(filterMode), &eid);
		detail::reportError("Context::createSampler() ", eid);
		return sampler ? Sampler(this, sampler) : Sampler();
	}


	clw::UserEvent Context::createUserEvent()
	{
#if defined(HAVE_OPENCL_1_1)
		cl_event event = clCreateUserEvent(id, &eid);
		detail::reportError("Context::createUserEvent() ", eid);
		return UserEvent(event);
#else
		return UserEvent();
#endif
	}

	vector<ImageFormat> Context::supportedImage2DFormats() const
	{
		return detail::supportedImageFormats(id, CL_MEM_OBJECT_IMAGE2D);
	}

	vector<ImageFormat> Context::supportedImage3DFormats() const
	{
		return detail::supportedImageFormats(id, CL_MEM_OBJECT_IMAGE3D);
	}

	Program Context::createProgramFromSourceCode(const string& sourceCode)
	{
		size_t length = sourceCode.length();
		const char* code = sourceCode.c_str();
		cl_program pid = clCreateProgramWithSource(id, 1, &code, &length, &eid);
		detail::reportError("Context::createProgramFromSourceCode(): ", eid);
		return pid ? Program(this, pid) : Program();
	}

	Program Context::createProgramFromSourceFile(const string& fileName)
	{
		string sourceCode;
		if(!detail::readAsString(fileName, &sourceCode))
			return Program();
		return createProgramFromSourceCode(sourceCode);
	}

	Program Context::buildProgramFromSourceCode(const string& sourceCode,
		                                        const string& options)
	{
		Program program = createProgramFromSourceCode(sourceCode);
		if(program.isNull() || program.build(options))
			return program;
		return Program();
	}

	Program Context::buildProgramFromSourceFile(const string& fileName,
		                                        const string& options)
	{
		Program program = createProgramFromSourceFile(fileName);
		if(program.isNull() || program.build(options))
			return program;
		return Program();
	}
}
