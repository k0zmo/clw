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

#include "Context.h"
#include "Platform.h"
#include "CommandQueue.h"
#include "Program.h"
#include "Buffer.h"
#include "Image.h"
#include "details.h"

#include <iostream>

#if !defined(CL_CONTEXT_OFFLINE_DEVICES_AMD)
#  define CL_CONTEXT_OFFLINE_DEVICES_AMD 0x403F
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

        string errorName(cl_int _eid)
        {
            #define CASE(X) case X: return string(#X);
            switch(_eid)
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
#if defined(HAVE_OPENCL_1_2)
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
            default: return string("Error ") + std::to_string(
                static_cast<unsigned long long>(_eid));
            }
            #undef CASE
        }

        static ErrorHandler reportErrorHandler;

        void reportError(const char* name, cl_int _eid)
        {
            if(_eid != CL_SUCCESS)
            {
                if(reportErrorHandler)
                    reportErrorHandler(_eid, string(name) + errorName(_eid));
                else
                    std::cerr << name << errorName(_eid) << std::endl;;
            }
        }

        vector<ImageFormat> supportedImageFormats(
            cl_context _id,
            cl_mem_object_type image_type)
        {
            cl_uint num;
            cl_int error;
            if((error = clGetSupportedImageFormats(_id, CL_MEM_READ_WRITE,
                    image_type, 0, nullptr, &num)) != CL_SUCCESS || !num)
            {
                reportError("supportedImageFormats() ", error);
                return vector<ImageFormat>();
            }
            vector<cl_image_format> buf(num);
            if((error = clGetSupportedImageFormats(_id, CL_MEM_READ_WRITE,
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

    void installErrorHandler(const ErrorHandler& handler)
    {
        detail::reportErrorHandler = handler;
    }

    Context::Context() 
        : _id(0)
        , _isCreated(false)
        , _eid(CL_SUCCESS)
    {
    }

    Context::~Context()
    {
        if(_isCreated && _id != 0)
            clReleaseContext(_id);
    }

    Context::Context(const Context& other)
        : _id(other._id), _isCreated(other._isCreated),
        _eid(other._eid), _devs(other._devs)
    {
        if(_id)
            clRetainContext(_id);
    }

    Context& Context::operator=(const Context& other)
    {
        if(other._id)
            clRetainContext(other._id);
        if(_id)
            clReleaseContext(_id);
        _id = other._id;
        _isCreated = other._isCreated;
        _eid = other._eid;
        _devs = other._devs;
        return *this;
    }

    bool Context::create(EDeviceType type)
    {
        if(_isCreated)
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
                if((_id = clCreateContext(props, 1, &did, 
                        &detail::contextNotify, nullptr, &_eid)) != 0)
                {
                    _devs.clear();
                    _devs.push_back(Device(did));
                    _isCreated = true;
                    return true;
                }
                detail::reportError("Context::create(type): ", _eid);
            }
        }
        _isCreated = false;
        return false;		
    }

    bool Context::create(const vector<Device>& devices)
    {
        if(_isCreated)
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
        if((_id = clCreateContext(props, cl_uint(dids.size()), dids.data(), 
                &detail::contextNotify, nullptr, &_eid)) != 0)
        {
            size_t size;
            if((_eid = clGetContextInfo(_id, CL_CONTEXT_DEVICES, 0,
                    nullptr, &size)) == CL_SUCCESS)
            {
                // !FIXME: assert(size == dids.size())
                _devs = devices;
                _isCreated = true;
                return true;
            }			
        }
        detail::reportError("Context::create(devices): ", _eid);
        return false;
    }

    bool Context::createOffline(const Platform& platform)
    {
        if(_isCreated)
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
        _id = clCreateContextFromType
            (props, CL_DEVICE_TYPE_ALL, nullptr, nullptr, &_eid);
        detail::reportError("Context::createOffline(devices): ", _eid);
        if(_eid == CL_SUCCESS)
        {
            size_t size;
            if((_eid = clGetContextInfo(_id, CL_CONTEXT_DEVICES, 0,
                    nullptr, &size)) == CL_SUCCESS)
            {
                size_t numDevices = size / sizeof(cl_device_id);
                vector<cl_device_id> devices_id(numDevices);
                clGetContextInfo(_id, CL_CONTEXT_DEVICES, size,
                    devices_id.data(), nullptr);
                _devs.clear();
                for(size_t i = 0; i < numDevices; ++i)
                    _devs.push_back(Device(devices_id[i]));
                _isCreated = true;
                return true;
            }
        }	
        return false;
    }

    bool Context::createDefault(Device& device, CommandQueue& queue)
    {
        if(!create() || numDevices() == 0)
            return false;
        device = devices()[0];
        if(device.isNull())
            return false;
        queue = createCommandQueue(device, CommandQueueFlags());
        return !queue.isNull();
    }

    void Context::release()
    {
        if(_isCreated)
        {
            clReleaseContext(_id);
            _id = 0;
            _isCreated = false;
        }
    }

    CommandQueue Context::createCommandQueue(const Device& device,
                                             CommandQueueFlags properties)
    {
        cl_command_queue cid = clCreateCommandQueue
            (_id, device.deviceId(),
            properties.raw(), &_eid);
        detail::reportError("Context::createCommandQueue(): ", _eid);
        return cid ? CommandQueue(this, cid) : CommandQueue();
    }

    Buffer Context::createBuffer(EAccess access,
                                 EMemoryLocation location,
                                 size_t size,
                                 const void* data)
    {
        cl_mem_flags mem_flags = cl_mem_flags(access);
        if(data && location != EMemoryLocation::UseHostMemory)
            mem_flags |= CL_MEM_COPY_HOST_PTR;
        mem_flags |= cl_mem_flags(location);
        cl_mem bid = clCreateBuffer
            (_id, mem_flags, size, const_cast<void*>(data), &_eid);
        detail::reportError("Context::createBuffer(): ", _eid);
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
        if(data && location != EMemoryLocation::UseHostMemory)
            mem_flags |= CL_MEM_COPY_HOST_PTR;
        mem_flags |= cl_mem_flags(location);
#if defined(HAVE_OPENCL_1_2)
        cl_image_desc desc;
        desc.image_type = CL_MEM_OBJECT_IMAGE2D;
        desc.image_width = width;
        desc.image_height = height;
        desc.image_depth = 0;
        desc.image_array_size = 1;
        desc.image_row_pitch = 0;
        desc.image_slice_pitch = 0;
        desc.num_mip_levels = 0;
        desc.num_samples = 0;
        desc.buffer = nullptr;
        cl_mem iid = clCreateImage
            (_id, mem_flags, &image_format,
             &desc, const_cast<void*>(data), &_eid);
#else

        cl_mem iid = clCreateImage2D
            (_id, mem_flags, &image_format, 
             width, height, 0, const_cast<void*>(data), &_eid);
#endif
        detail::reportError("Context::createImage2D(): ", _eid);
        return iid ? Image2D(this, iid) : Image2D();
    }

    Sampler Context::createSampler(bool normalizedCoords, 
                                   EAddressingMode addressingMode, 
                                   EFilterMode filterMode)
    {
        cl_sampler sampler = clCreateSampler
            (_id, normalizedCoords ? CL_TRUE : CL_FALSE, 
             cl_addressing_mode(addressingMode), 
             cl_filter_mode(filterMode), &_eid);
        detail::reportError("Context::createSampler() ", _eid);
        return sampler ? Sampler(this, sampler) : Sampler();
    }


    UserEvent Context::createUserEvent()
    {
#if defined(HAVE_OPENCL_1_1)
        cl_event event = clCreateUserEvent(_id, &_eid);
        detail::reportError("Context::createUserEvent() ", _eid);
        return UserEvent(event);
#else
        return UserEvent();
#endif
    }

    vector<ImageFormat> Context::supportedImage2DFormats() const
    {
        return detail::supportedImageFormats(_id, CL_MEM_OBJECT_IMAGE2D);
    }

    vector<ImageFormat> Context::supportedImage3DFormats() const
    {
        return detail::supportedImageFormats(_id, CL_MEM_OBJECT_IMAGE3D);
    }

    Program Context::createProgramFromSourceCode(const string& sourceCode)
    {
        size_t length = sourceCode.length();
        const char* code = sourceCode.c_str();
        cl_program pid = clCreateProgramWithSource(_id, 1, &code, &length, &_eid);
        detail::reportError("Context::createProgramFromSourceCode(): ", _eid);
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
