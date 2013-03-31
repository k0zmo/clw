#pragma once

#include "Prerequisites.h"
#include "Event.h"
#include "MemoryObject.h"

namespace clw
{
    enum ECommandQueueProperty
    {
        Property_OutOfOrderExecModeEnabled   = (1 << 0),
        Property_ProfilingEnabled            = (1 << 1)
    };

    enum EMapAccess
    {
        MapAccess_Read                      = 1 << 0,
        MapAccess_Write                     = 1 << 1,
#if defined(HAVE_OPENCL_1_2)
        MapAccess_InvalidateRegion          = 1 << 2
#endif
    };

    class CommandQueue
    {
    public:
        CommandQueue() : _ctx(nullptr), _id(0) {}
        CommandQueue(Context* ctx, cl_command_queue id)
            : _ctx(ctx), _id(id) {}
        ~CommandQueue();

        CommandQueue(const CommandQueue& other);		
        CommandQueue& operator=(const CommandQueue& other);

        bool isNull() const { return _id == 0; }
        bool isProfilingEnabled() const;
        bool isOutOfOrder() const;

        void finish();
        void flush();

        bool readBuffer(const Buffer& buffer,
                        void* data, 
                        size_t offset, 
                        size_t size);
        Event asyncReadBuffer(const Buffer& buffer,
                              void* data,
                              size_t offset,
                              size_t size,
                              const EventList& after = EventList());

        bool writeBuffer(Buffer& buffer,
                         const void* data, 
                         size_t offset, 
                         size_t size);
        Event asyncWriteBuffer(Buffer& buffer,
                               const void* data,
                               size_t offset, 
                               size_t size,
                               const EventList& after = EventList());

        //bool copyBuffer();
        //Event asyncCopyBuffer();

        //bool readBufferRect();
        //Event asyncReadBufferRect();

        bool writeBufferRect(Buffer& buffer,
                             const void* data,
                             const size_t origin[3],
                             const size_t size[3],
                             size_t bytesPerLine,
                             size_t bufferBytesPerLine);
        //Event asyncWriteBufferRect();

        //bool copyBufferRect();
        //Event asyncCopyBufferRect();

        // !TODO OpenCL 1.2
        // clEnqueueFillBuffer

        bool readImage2D(const Image2D& image,
                         void* data,
                         int x,
                         int y,
                         int width,
                         int height,
                         int bytesPerLine = 0);
        Event asyncReadImage2D(const Image2D& image,
                               void* data,
                               int x,
                               int y,
                               int width,
                               int height,
                               int bytesPerLine = 0,
                               const EventList& after = EventList());

        bool writeImage2D(Image2D& image,
                          const void* data,
                          int x, 
                          int y,
                          int width, 
                          int height,
                          int bytesPerLine = 0);
        Event asyncWriteImage2D(Image2D& image,
                                const void* data,
                                int x,
                                int y,
                                int width,
                                int height,
                                int bytesPerLine = 0,
                                const EventList& after = EventList());

        //bool copyImage();
        //Event asyncCopyImage();

        // !TODO OpenCL 1.2
        // clEnqueueFillImage

        //bool copyImageToBuffer();
        //Event asyncCopyImageToBuffer();

        //bool copyBufferToImage();
        //Event asyncCopyBufferToImage();

        void* mapBuffer(Buffer& buffer, 
                        size_t offset, 
                        size_t size,
                        EMapAccess access);
        void* mapBuffer(Buffer& buffer, EMapAccess access);

        Event asyncMapBuffer(Buffer& buffer, 
                             void** data,
                             size_t offset, 
                             size_t size, 
                             EMapAccess access, 
                             const EventList& after = EventList());

        void* mapImage2D(Image2D& image,
                         int x, 
                         int y,
                         int width,
                         int height,
                         EMapAccess access);

        Event asyncMapImage2D(Image2D& image,
                              void** data,
                              int x, 
                              int y,
                              int width,
                              int height,
                              EMapAccess access,
                              const EventList& after = EventList());

        bool unmap(MemoryObject& obj,
                   void* ptr);
        Event asyncUnmap(MemoryObject& obj,
                         void* ptr,
                         const EventList& after = EventList());

        bool runKernel(const Kernel& kernel);
        Event asyncRunKernel(
            const Kernel& kernel,
            const EventList& after = EventList());

        //bool runTask();
        //Event asyncRunTask();

        //bool runNativeKernel();
        //Event asyncRunNativeKernel();

        // !TODO OpenCL 1.2
        // clEnqueueMigrateMemObjects
        // clEnqueueMarkerWithWaitList

        cl_command_queue commandQueueId() const { return _id; }
        Context* context() const { return _ctx; }

    private:
        Context* _ctx;
        cl_command_queue _id;		
    };
}
