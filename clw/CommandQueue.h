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

	class CommandQueue
	{
	public:
		CommandQueue() : ctx(nullptr), id(0) {}
		CommandQueue(Context* ctx, cl_command_queue id)
			: ctx(ctx), id(id) {}
		~CommandQueue();

		CommandQueue(const CommandQueue& other);		
		CommandQueue& operator=(const CommandQueue& other);

		bool isNull() const { return id == 0; }
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

		//bool writeBufferRect();
		//Event asyncWriteBufferRect();

		//bool copyBufferRect();
		//Event asyncCopyBufferRect();

		// !TODO OpenCL 1.2
		// clEnqueueFillBuffer

		//bool readImage();
		Event asyncReadImage2D(const clw::Image2D& image,
		                       void* data,
		                       int x,
		                       int y,
		                       int width,
		                       int height,
		                       int bytesPerLine = 0,
		                       const EventList& after = EventList());

		//bool writeImage();
		Event asyncWriteImage2D(clw::Image2D& image,
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
		                EAccess access);
		void* mapBuffer(Buffer& buffer, EAccess access);
		Event asyncMapBuffer(Buffer& buffer, 
		                     void** data,
		                     size_t offset, 
		                     size_t size, 
		                     EAccess access, 
		                     const EventList& after = EventList());

		//void* mapImage();
		//Event asyncMapImage();

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

		cl_command_queue commandQueueId() const { return id; }
		Context* context() const { return ctx; }

	private:
		Context* ctx;
		cl_command_queue id;		
	};
}
