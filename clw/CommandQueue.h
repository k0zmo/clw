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

#pragma once

#include "Prerequisites.h"
#include "Event.h"
#include "Image.h"
#include "Buffer.h"
#include "EnumFlags.h"

namespace clw
{
    enum class ECommandQueueProperty
    {
        OutOfOrderExecModeEnabled   = (1 << 0),
        ProfilingEnabled            = (1 << 1)
    };
    typedef EnumFlags<ECommandQueueProperty> CommandQueueFlags;

    enum class EMapAccess
    {
        Read                      = 1 << 0,
        Write                     = 1 << 1,
        ReadWrite                 = (1 << 1) | (1 << 0),
#if defined(HAVE_OPENCL_1_2)
        InvalidateRegion          = 1 << 2
#endif
    };
    typedef EnumFlags<EMapAccess> MapAccessFlags;

    class CLW_EXPORT CommandQueue
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
        // Overload version of readBuffer to read whole buffer.
        // data must be at least the size of buffer's size
        bool readBuffer(const Buffer& buffer,
                        void* data);

        Event asyncReadBuffer(const Buffer& buffer,
                              void* data,
                              size_t offset,
                              size_t size,
                              const EventList& after = EventList());
        Event asyncReadBuffer(const Buffer& buffer,
                              void* data,
                              const EventList& after = EventList());

        bool writeBuffer(Buffer& buffer,
                         const void* data,
                         size_t offset,
                         size_t size);
        bool writeBuffer(Buffer& buffer,
                         const void* data);

        Event asyncWriteBuffer(Buffer& buffer,
                               const void* data,
                               size_t offset, 
                               size_t size,
                               const EventList& after = EventList());
        Event asyncWriteBuffer(Buffer& buffer,
                               const void* data,
                               const EventList& after = EventList());

        // NOTE: I don't see a point to implement blocking copy buffer - 
        //       at least when in-order queue is in use. For out-of-order 
        //       queue the lack of mentioned methods is slightly inconvient
        //       and isn't worth a boilerplate code needed to implement it

        Event asyncCopyBuffer(const Buffer& src,
                              size_t srcOffset,
                              const Buffer& dst,
                              size_t dstOffset,
                              size_t size,
                              const EventList& after = EventList());
        Event asyncCopyBuffer(const Buffer& src,
                              const Buffer& dst,
                              const EventList& after = EventList());

        bool writeBufferRect(Buffer& buffer,
                             const void* data,
                             const Rect& rect,
                             size_t bytesPerLine,
                             size_t bufferBytesPerLine,
                             size_t bytesPerSlice = 0,
                             size_t bufferBytesPerSlice = 0);
        bool readBufferRect(const Buffer& buffer,
                            void* data,
                            const Rect& rect,
                            size_t bytesPerLine,
                            size_t bufferBytesPerLine,
                            size_t bytesPerSlice = 0,
                            size_t bufferBytesPerSlice = 0);

        Event asyncWriteBufferRect(Buffer& buffer,
                                   const void* data,
                                   const Rect& rect,
                                   size_t bytesPerLine,
                                   size_t bufferBytesPerLine,
                                   size_t bytesPerSlice = 0,
                                   size_t bufferBytesPerSlice = 0,
                                   const EventList& after = EventList());
        Event asyncReadBufferRect(const Buffer& buffer,
                                  void* data,
                                  const Rect& rect,
                                  size_t bytesPerLine,
                                  size_t bufferBytesPerLine,
                                  size_t bytesPerSlice = 0,
                                  size_t bufferBytesPerSlice = 0,
                                  const EventList& after = EventList());
        Event asyncCopyBufferRect(const Buffer& src,
                                  const Rect& rect,
                                  Buffer& dst,
                                  const Point& dstOrigin,
                                  size_t srcBytesPerLine,
                                  size_t dstBytesPerLine,
                                  size_t srcBytesPerSlice = 0,
                                  size_t dstBytesPerSlice = 0,
                                  const EventList& after = EventList());

        bool readImage2D(const Image2D& image,
                         void* data,
                         const Rect& rect,
                         int bytesPerLine = 0);
        bool readImage2D(const Image2D& image,
                         void* data,
                         int bytesPerLine = 0);

        Event asyncReadImage2D(const Image2D& image,
                               void* data,
                               const Rect& rect,
                               int bytesPerLine = 0,
                               const EventList& after = EventList());
        Event asyncReadImage2D(const Image2D& image,
                               void* data,
                               int bytesPerLine = 0,
                               const EventList& after = EventList());

        bool writeImage2D(Image2D& image,
                          const void* data,
                          const Rect& rect,
                          int bytesPerLine = 0);
        bool writeImage2D(Image2D& image,
                          const void* data,
                          int bytesPerLine = 0);

        Event asyncWriteImage2D(Image2D& image,
                                const void* data,
                                const Rect& rect,
                                int bytesPerLine = 0,
                                const EventList& after = EventList());
        Event asyncWriteImage2D(Image2D& image,
                                const void* data,
                                int bytesPerLine = 0,
                                const EventList& after = EventList());

        Event asyncCopyImage(const Image2D& src,
                             const Rect& srcRect,
                             Image2D& dst,
                             const Point& dstOrigin,
                             const EventList& after = EventList());
        Event asyncCopyImage(const Image2D& src,
                             Image2D& dst,
                             const EventList& after = EventList());

        Event asyncCopyImageToBuffer(const clw::Image2D& image,
                                     const Rect& rect, 
                                     Buffer& buffer, 
                                     size_t offset = 0,
                                     const EventList& after = EventList());
        Event asyncCopyImageToBuffer(const clw::Image2D& image,
                                     Buffer& buffer,
                                     const EventList& after = EventList());

        Event asyncCopyBufferToImage(const clw::Buffer& buffer,
                                     size_t offset,
                                     Image2D& image, 
                                     const Rect& rect,
                                     const EventList& after = EventList());
        Event asyncCopyBufferToImage(const clw::Buffer& buffer,
                                     Image2D& image,
                                     const EventList& after = EventList());

        void* mapBuffer(Buffer& buffer, 
                        size_t offset, 
                        size_t size,
                        MapAccessFlags access);
        void* mapBuffer(Buffer& buffer, MapAccessFlags access);

        Event asyncMapBuffer(Buffer& buffer, 
                             void** data,
                             size_t offset, 
                             size_t size, 
                             MapAccessFlags access, 
                             const EventList& after = EventList());
        Event asyncMapBuffer(Buffer& buffer, 
                             void** data,
                             MapAccessFlags access, 
                             const EventList& after = EventList());

        void* mapImage2D(Image2D& image,
                         const Rect& rect,
                         MapAccessFlags access);
        void* mapImage2D(Image2D& image,
                         MapAccessFlags access);

        Event asyncMapImage2D(Image2D& image,
                              void** data,
                              const Rect& rect,
                              MapAccessFlags access,
                              const EventList& after = EventList());
        Event asyncMapImage2D(Image2D& image,
                              void** data,
                              MapAccessFlags access,
                              const EventList& after = EventList());

        bool unmap(MemoryObject& obj,
                   void* ptr);
        Event asyncUnmap(MemoryObject& obj,
                         void* ptr,
                         const EventList& after = EventList());

        // !TODO OpenCL 1.2
        // clEnqueueFillBuffer
        // clEnqueueFillImage

        bool runKernel(const Kernel& kernel);
        Event asyncRunKernel(const Kernel& kernel,
                             const EventList& after = EventList());

        bool runTask(const Kernel& kernel);
        Event asyncRunTask(const Kernel& kernel,
                           const EventList& after = EventList());

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

    inline bool CommandQueue::readBuffer(const Buffer& buffer,
                                         void* data)
    {
        return readBuffer(buffer, data, 0, buffer.size());
    }

    inline Event CommandQueue::asyncReadBuffer(const Buffer& buffer,
                                               void* data,
                                               const EventList& after)
    {
        return asyncReadBuffer(buffer, data, 0, buffer.size(), after);
    }

    inline bool CommandQueue::writeBuffer(Buffer& buffer,
                                          const void* data)
    {
        return writeBuffer(buffer, data, 0, buffer.size());
    }

    inline Event CommandQueue::asyncWriteBuffer(Buffer& buffer,
                                                const void* data,
                                                const EventList& after)
    {
        return asyncWriteBuffer(buffer, data, 0, buffer.size(), after);
    }

    inline Event CommandQueue::asyncCopyBuffer(const Buffer& src,
                                               const Buffer& dst,
                                               const EventList& after)
    {
        return asyncCopyBuffer(src, 0, dst, 0, src.size(), after);
    }

    inline bool CommandQueue::readImage2D(const Image2D& image,
                                          void* data,
                                          int bytesPerLine)
    {
        return readImage2D(image, data, 
            Rect(0, 0, image.width(), image.height()),
            bytesPerLine);
    }

    inline Event CommandQueue::asyncReadImage2D(const Image2D& image,
                                                void* data,
                                                int bytesPerLine,
                                                const EventList& after)
    {
        return asyncReadImage2D(image, data, 
            Rect(0, 0, image.width(), image.height()),
            bytesPerLine, after);
    }

    inline bool CommandQueue::writeImage2D(Image2D& image,
                                           const void* data,
                                           int bytesPerLine)
    {
        return writeImage2D(image, data,
            Rect(0, 0, image.width(), image.height()), 
            bytesPerLine);
    }

    inline Event CommandQueue::asyncWriteImage2D(Image2D& image,
                                                 const void* data,
                                                 int bytesPerLine,
                                                 const EventList& after)
    {
        return asyncWriteImage2D(image, data,
            Rect(0, 0, image.width(), image.height()), 
            bytesPerLine, after);
    }

    inline Event CommandQueue::asyncCopyImage(const Image2D& src,
                                              Image2D& dst,
                                              const EventList& after)
    {
        return asyncCopyImage(src,
            Rect(0, 0, src.width(), src.height()),
            dst, Point(0, 0), after);
    }

    inline Event CommandQueue::asyncCopyImageToBuffer(const clw::Image2D& image,
                                                      Buffer& buffer,
                                                      const EventList& after)
    {
        return asyncCopyImageToBuffer(image,
            Rect(0, 0, image.width(), image.height()), 
            buffer, 0, after);
    }

    inline Event CommandQueue::asyncCopyBufferToImage(const clw::Buffer& buffer,
                                                      Image2D& image,
                                                      const EventList& after)
    {
        return asyncCopyBufferToImage(buffer, 0, 
            image, Rect(0, 0, image.width(), image.height()),
            after);
    }

    inline void* CommandQueue::mapImage2D(Image2D& image, MapAccessFlags access)
    {
        return mapImage2D(image, Rect(0, 0, image.width(), image.height()), access);
    }

    inline Event CommandQueue::asyncMapImage2D(Image2D& image,
                                               void** data,
                                               MapAccessFlags access,
                                               const EventList& after)
    {
        return asyncMapImage2D(image, data,
            Rect(0, 0, image.width(), image.height()), 
            access, after);
    }

}