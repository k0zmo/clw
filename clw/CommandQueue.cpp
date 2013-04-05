#include "CommandQueue.h"
#include "Buffer.h"
#include "Image.h"
#include "Kernel.h"
#include "Grid.h"
#include "details.h"

namespace clw
{
    namespace detail
    {
        bool commandQueueInfo(cl_command_queue _id, cl_ulong prop)
        {
            cl_command_queue_properties props;
            cl_int error = CL_SUCCESS;
            if(!_id || (error = clGetCommandQueueInfo(_id, CL_QUEUE_PROPERTIES, 
                    sizeof(cl_command_queue_properties), &props, nullptr))
                        != CL_SUCCESS)
            {
                detail::reportError("commandQueueInfo(): ", error);
                return false;
            }
            return (props & prop) != 0;
        }
    }

    CommandQueue::~CommandQueue()
    {
        if(_id)
            clReleaseCommandQueue(_id);
    }

    CommandQueue::CommandQueue(const CommandQueue& other)
        : _ctx(other._ctx), _id(other._id)
    {
        if(_id)
            clRetainCommandQueue(_id);
    }

    CommandQueue& CommandQueue::operator=(const CommandQueue& other)
    {
        _ctx = other._ctx;
        if(other._id)
            clRetainCommandQueue(other._id);
        if(_id)
            clReleaseCommandQueue(_id);
        _id = other._id;
        return *this;
    }

    bool CommandQueue::isProfilingEnabled() const
    {
        return detail::commandQueueInfo(_id, CL_QUEUE_PROFILING_ENABLE);
    }

    bool CommandQueue::isOutOfOrder() const
    {
        return detail::commandQueueInfo(_id, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
    }

    void CommandQueue::finish()
    {
        cl_int err = clFinish(_id);
        detail::reportError("CommandQueue::finish(): ", err);
    }

    void CommandQueue::flush()
    {
        cl_int err = clFlush(_id);
        detail::reportError("CommandQueue::flush(): ", err);
    }

    bool CommandQueue::readBuffer(const Buffer& buffer,
                                  void* data,
                                  size_t offset,
                                  size_t size)
    {
        cl_int error;
        if((error = clEnqueueReadBuffer(_id, buffer.memoryId(), 
                CL_TRUE, offset, size, data,
                0, nullptr, nullptr)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::readBuffer() ", error);
            return false;
        }
        return true;
    }

    bool CommandQueue::readBuffer(const Buffer& buffer,
                                  void* data)
    {
        return readBuffer(buffer, data, 0, buffer.size());
    }

    Event CommandQueue::asyncReadBuffer(const Buffer& buffer,
                                        void* data,
                                        size_t offset,
                                        size_t size,
                                        const EventList& after)
    {
        cl_event event;
        cl_int error;
        if((error = clEnqueueReadBuffer(_id, buffer.memoryId(), 
                CL_FALSE, offset, size, data,
                cl_uint(after.size()), after, &event)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncReadBuffer() ", error);
            return Event(event);
        }
        return Event(event);
    }

    Event CommandQueue::asyncReadBuffer(const Buffer& buffer,
                                        void* data,
                                        const EventList& after)
    {
        return asyncReadBuffer(buffer, data, 0, buffer.size(), after);
    }

    bool CommandQueue::writeBuffer(Buffer& buffer,
                                   const void* data, 
                                   size_t offset, 
                                   size_t size)
    {
        cl_int error;
        if((error = clEnqueueWriteBuffer(_id, buffer.memoryId(), 
                CL_TRUE, offset, size, data,
                0, nullptr, nullptr)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::writeBuffer() ", error);
            return false;
        }
        return true;
    }

    bool CommandQueue::writeBuffer(Buffer& buffer,
                                   const void* data)
    {
        return writeBuffer(buffer, data, 0, buffer.size());
    }

    Event CommandQueue::asyncWriteBuffer(Buffer& buffer,
                                         const void* data, 
                                         size_t offset, 
                                         size_t size,
                                         const EventList& after)
    {
        cl_event event;
        cl_int error;
        if((error = clEnqueueWriteBuffer(_id, buffer.memoryId(), 
                CL_FALSE, offset, size, data,
                cl_uint(after.size()), after, &event)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncWriteBuffer() ", error);
            return Event(event);
        }
        return Event(event);
    }

    Event CommandQueue::asyncWriteBuffer(Buffer& buffer,
                                         const void* data,
                                         const EventList& after)
    {
        return asyncWriteBuffer(buffer, data, 0, buffer.size(), after);
    }

    Event CommandQueue::asyncCopyBuffer(const Buffer& src,
                                        const Buffer& dst,
                                        size_t srcOffset,
                                        size_t dstOffset,
                                        size_t size,
                                        const EventList& after)
    {
        cl_int error;
        cl_event event;
        if((error = clEnqueueCopyBuffer(_id, src.memoryId(), dst.memoryId(), 
                srcOffset, dstOffset, size, cl_uint(after.size()), 
                after, &event)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncCopyBuffer() ", error);
            return Event();
        }
        return Event(event);
    }
    Event CommandQueue::asyncCopyBuffer(const Buffer& src,
                                        const Buffer& dst,
                                        const EventList& after)
    {
        return asyncCopyBuffer(src, dst, 0, 0, src.size(), after);
    }

    bool CommandQueue::writeBufferRect(Buffer& buffer,
                                       const void* data,
                                       const size_t origin[3],
                                       const size_t size[3],
                                       size_t bytesPerLine,
                                       size_t bufferBytesPerLine)
    {
        cl_int error;
        if((error = clEnqueueWriteBufferRect(_id, buffer.memoryId(), 
                CL_TRUE, origin, 0, size, bufferBytesPerLine, 0, 
                bytesPerLine, 0, data, 0, nullptr, nullptr)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::writeBufferRect() ", error);
            return false;
        }
        return true;
    }

    bool CommandQueue::readImage2D(const Image2D& image,
                                   void* data,
                                   int x,
                                   int y,
                                   int width,
                                   int height,
                                   int bytesPerLine)
    {
        cl_int error;
        size_t origin[3] = { size_t(x), size_t(y), 0 };
        size_t region[3] = { size_t(width), size_t(height), 1 };
        if((error = clEnqueueReadImage(_id, image.memoryId(),
                CL_TRUE, origin, region, bytesPerLine, 0, 
                data, 0, nullptr, nullptr)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::readImage2D() ", error);
            return false;
        }
        return true;
    }

    bool CommandQueue::readImage2D(const Image2D& image,
                                   void* data,
                                   int bytesPerLine)
    {
        return readImage2D(image, data, 0, 0,
            image.width(), image.height(), bytesPerLine);
    }

    Event CommandQueue::asyncReadImage2D(const Image2D& image,
                                         void* data,
                                         int x,
                                         int y,
                                         int width,
                                         int height,
                                         int bytesPerLine,
                                         const EventList& after)
    {
        cl_event event;
        cl_int error;
        size_t origin[3] = { size_t(x), size_t(y), 0 };
        size_t region[3] = { size_t(width), size_t(height), 1 };
        if((error = clEnqueueReadImage(_id, image.memoryId(),
                CL_FALSE, origin, region, bytesPerLine, 0, 
                data, cl_uint(after.size()), after, &event)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncReadImage2D() ", error);
            return Event(event);
        }
        return Event(event);
    }

    Event CommandQueue::asyncReadImage2D(const Image2D& image,
                                         void* data,
                                         int bytesPerLine,
                                         const EventList& after)
    {
        return asyncReadImage2D(image, data, 0, 0,
            image.width(), image.height(), bytesPerLine, after);
    }

    bool CommandQueue::writeImage2D(Image2D& image,
                                    const void* data,
                                    int x, 
                                    int y,
                                    int width, 
                                    int height,
                                    int bytesPerLine)
    {
        cl_int error;
        size_t origin[3] = { size_t(x), size_t(y), 0 };
        size_t region[3] = { size_t(width), size_t(height), 1 };
        if((error = clEnqueueWriteImage(_id, image.memoryId(),
                CL_TRUE, origin, region, bytesPerLine, 0, 
                data, 0, nullptr, nullptr)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::writeImage2D() ", error);
            return false;
        }
        return true;
    }

    bool CommandQueue::writeImage2D(Image2D& image,
                                    const void* data,
                                    int bytesPerLine)
    {
        return writeImage2D(image, data, 0, 0, 
            image.width(), image.height(), bytesPerLine);
    }

    Event CommandQueue::asyncWriteImage2D(Image2D& image,
                                          const void* data,
                                          int x,
                                          int y,
                                          int width,
                                          int height, 
                                          int bytesPerLine,
                                          const EventList& after)
    {
        cl_event event;
        cl_int error;
        size_t origin[3] = { size_t(x), size_t(y), 0 };
        size_t region[3] = { size_t(width), size_t(height), 1 };
        if((error = clEnqueueWriteImage(_id, image.memoryId(),
            CL_FALSE, origin, region, bytesPerLine, 0, 
            data, cl_uint(after.size()), after, &event)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncWriteImage2D() ", error);
            return Event(event);
        }
        return Event(event);
    }

    Event CommandQueue::asyncWriteImage2D(Image2D& image,
                                          const void* data,
                                          int bytesPerLine,
                                          const EventList& after)
    {
        return asyncWriteImage2D(image, data, 0, 0, 
            image.width(), image.height(), bytesPerLine, after);
    }

    Event CommandQueue::asyncCopyImage(const Image2D& src,
                                       Image2D& dst,
                                       int srcX,
                                       int srcY,
                                       int width,
                                       int height,
                                       int dstX,
                                       int dstY,
                                       const EventList& after)
    {
        cl_int error;
        cl_event event;
        size_t src_origin[3] = { size_t(srcX), size_t(srcY), 0 };
        size_t dst_origin[3] = { size_t(dstX), size_t(dstY), 0 };
        size_t region[3] = { size_t(width), size_t(height), 1 };
        if((error = clEnqueueCopyImage(_id, src.memoryId(), dst.memoryId(),
                src_origin, dst_origin, region,
                cl_uint(after.size()), after, &event)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncCopyImage() ", error);
            return Event();
        }
        return Event(event);
    }

    Event CommandQueue::asyncCopyImage(const Image2D& src,
                                       Image2D& dst,
                                       const EventList& after)
    {
        return asyncCopyImage(src, dst, 0, 0,
            src.width(), src.height(), 0, 0, after);
    }

    Event CommandQueue::asyncCopyImageToBuffer(const clw::Image2D& image,
                                               Buffer& buffer, 
                                               int x, 
                                               int y,
                                               int width,
                                               int height, 
                                               int offset,
                                               const EventList& after)
    {
        cl_int error;
        size_t origin[3] = { size_t(x), size_t(y), 0 };
        size_t region[3] = { size_t(width), size_t(height), 1 };
        cl_event event;
        if((error = clEnqueueCopyImageToBuffer(_id, image.memoryId(),
                buffer.memoryId(), origin, region, offset,
                cl_uint(after.size()), after, &event)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncCopyImageToBuffer() ", error);
            return Event();
        }
        return Event(event);
    }

    Event CommandQueue::asyncCopyImageToBuffer(const clw::Image2D& image,
                                               Buffer& buffer,
                                               const EventList& after)
    {
        return asyncCopyImageToBuffer(image, buffer, 0, 0,
            image.width(), image.height(), 0, after);
    }

    void* CommandQueue::mapBuffer(Buffer& buffer, 
                                  size_t offset, 
                                  size_t size,
                                  EMapAccess access)
    {
        cl_int error;
        void* data = clEnqueueMapBuffer(_id, buffer.memoryId(), CL_TRUE,
            access, offset, size, 0, nullptr, nullptr, &error);
        detail::reportError("CommandQueue::mapBuffer() ", error);
        return data;
    }

    void* CommandQueue::mapBuffer(Buffer& buffer, EMapAccess access)
    {
        return mapBuffer(buffer, 0, buffer.size(), access);
    }

    Event CommandQueue::asyncMapBuffer(Buffer& buffer, 
                                       void** data,
                                       size_t offset, 
                                       size_t size, 
                                       EMapAccess access, 
                                       const EventList& after)
    {
        cl_int error;
        cl_event event;
        *data = clEnqueueMapBuffer(_id, buffer.memoryId(), CL_FALSE,
            access, offset, size, cl_uint(after.size()), after, &event, &error);
        if(error != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncMapBuffer() ", error);
            return Event();
        }
        return Event(event);
    }

    Event CommandQueue::asyncMapBuffer(Buffer& buffer, 
                                       void** data,
                                       EMapAccess access, 
                                       const EventList& after)
    {
        return asyncMapBuffer(buffer, data, 0, buffer.size(), access, after);
    }

    void* CommandQueue::mapImage2D(Image2D& image,
                                   int x, 
                                   int y,
                                   int width,
                                   int height,
                                   EMapAccess access)
    {
        cl_int error;
        size_t origin[3] = { size_t(x), size_t(y), 0 };
        size_t region[3] = { size_t(width), size_t(height), 1 };
        size_t pitch;
        void* data = clEnqueueMapImage(_id, image.memoryId(), CL_TRUE,
            access, origin, region, &pitch, nullptr,
            0, nullptr, nullptr, &error);
        detail::reportError("CommandQueue::mapImage2D() ", error);
        return data;
    }

    void* CommandQueue::mapImage2D(Image2D& image,
                                   EMapAccess access)
    {
        return mapImage2D(image, 0, 0, image.width(), image.height(), access);
    }

    Event CommandQueue::asyncMapImage2D(Image2D& image,
                                        void** data,
                                        int x, 
                                        int y,
                                        int width,
                                        int height,
                                        EMapAccess access,
                                        const EventList& after)
    {
        cl_int error;
        cl_event event;
        size_t origin[3] = { size_t(x), size_t(y), 0 };
        size_t region[3] = { size_t(width), size_t(height), 1 };
        size_t pitch;
        *data = clEnqueueMapImage(_id, image.memoryId(), CL_FALSE,
            access, origin, region, &pitch, nullptr,
            cl_uint(after.size()), after, &event, &error);
        if(error != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncMapImage2D() ", error);
            return Event();
        }
        return Event(event);
    }

    Event CommandQueue::asyncMapImage2D(Image2D& image,
                                        void** data,
                                        EMapAccess access,
                                        const EventList& after)
    {
        return asyncMapImage2D(image, data, 0, 0,
            image.width(), image.height(), access, after);
    }

    bool CommandQueue::unmap(MemoryObject& obj, void* ptr)
    {
        cl_event event;
        cl_int error;
        if((error = clEnqueueUnmapMemObject(_id, obj.memoryId(),
                ptr, 0, nullptr, &event)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue()::unmap() ", error);
            return false;
        }
        else
        {
            clWaitForEvents(1, &event);
            clReleaseEvent(event);
            return true;
        }
    }

    Event CommandQueue::asyncUnmap(MemoryObject& obj,
                                   void* ptr,
                                   const EventList& after)
    {
        cl_event event;
        cl_int error;
        if((error = clEnqueueUnmapMemObject(_id, obj.memoryId(),
            ptr, cl_uint(after.size()), after, &event)) != CL_SUCCESS)
        {
            detail::reportError("CommandQueue()::asyncUnmap() ", error);
            return Event();
        }
        else
        {
            return Event(event);
        }
    }

    bool CommandQueue::runKernel(const Kernel& kernel)
    {
        const Grid& offset = kernel.globalWorkOffset();
        const Grid& global = kernel.globalWorkSize();		
        const Grid& local = kernel.localWorkSize();
        size_t dims = global.dimensions();
        const size_t* l = local;
        if(local.width() == 0)
            l = nullptr;

        cl_event event;
        cl_int error = clEnqueueNDRangeKernel
            (_id, kernel.kernelId(), 
            cl_uint(dims), offset, global, l,
            0, nullptr, &event);
        if(error != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::runKernel() ", error);
            return false;
        }
        else
        {
            clWaitForEvents(1, &event);
            clReleaseEvent(event);
            return true;
        }
    }

    Event CommandQueue::asyncRunKernel(
        const Kernel& kernel,
        const EventList& after)
    {
        const Grid& offset = kernel.globalWorkOffset();
        const Grid& global = kernel.globalWorkSize();		
        const Grid& local = kernel.localWorkSize();
        size_t dims = global.dimensions();
        const size_t* l = local;
        if(local.width() == 0)
            l = nullptr;

        cl_event event;
        cl_int error = clEnqueueNDRangeKernel
            (_id, kernel.kernelId(), 
             cl_uint(dims), offset, global, l,
             cl_uint(after.size()), after, &event);
        if(error != CL_SUCCESS)
        {
            detail::reportError("CommandQueue::asyncRunKernel() ", error);
            return Event();
        }
        return Event(event);
    }
}
