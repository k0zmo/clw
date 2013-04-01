#pragma once

#include "Prerequisites.h"
#include "Platform.h"
#include "Device.h"
#include "Sampler.h"
#include "MemoryObject.h"

namespace clw
{
    class CLW_EXPORT Context
    {
    public:
        Context();
        ~Context();

        Context(const Context& other);		
        Context& operator=(const Context& other);

        bool create(EDeviceType type = EDeviceType::Default);
        bool create(const vector<Device>& devices);
        bool createOffline(const Platform& platform = Platform());
        bool createDefault(Device& device, CommandQueue& queue);
        void release();
        bool isCreated() const { return _isCreated; }

        cl_int lastError() const { return _eid; }
        void setLastError(cl_int error) { _eid = error; }

        CommandQueue createCommandQueue(const Device& device,
                                        cl_command_queue_properties properties = 0);

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

        UserEvent createUserEvent();

        vector<ImageFormat> supportedImage2DFormats() const;
        vector<ImageFormat> supportedImage3DFormats() const;

        size_t numDevices() const { return _devs.size(); }
        const vector<Device>& devices() const { return _devs; }
        cl_context contextId() const { return _id; }

        Program createProgramFromSourceCode(const string& sourceCode);
        Program createProgramFromSourceFile(const string& fileName);

        Program buildProgramFromSourceCode(const string& sourceCode,
                                           const string& options = string());
        Program buildProgramFromSourceFile(const string& fileName, 
                                           const string& options = string());

        // !TODO: createProgramFromBinaries()

    private:
        cl_context _id;
        bool _isCreated;
        cl_int _eid;
        vector<Device> _devs;
    };

    typedef std::function<void(cl_int errId, const string& message)> ErrorHandler;
    void CLW_EXPORT installErrorHandler(const ErrorHandler& handler);
}