#pragma once

#include "Prerequisites.h"

namespace clw
{
	enum EAccess
	{
		Access_ReadWrite          = 0x01,
		Access_WriteOnly          = 0x02,
		Access_ReadOnly           = 0x04,
	};

	enum EMemoryLocation
	{
		Location_Device             = 0x00,
		Location_UseHostMemory      = 0x08,
		Location_AllocHostMemory    = 0x10,
		// AMD Only - alloc from GPU's CPU visible heap
		Location_UsePinnedMemory    = 0x40 
	};

	enum EObjectType
	{
		Object_Buffer         = 0x10F0,
		Object_Image2D        = 0x10F1,
		Object_Image3D        = 0x10F2,
		Object_Image2DArray   = 0x10F3,
		Object_Image1D        = 0x10F4,
		Object_Image1DArray   = 0x10F5,
		Object_Image1DBuffer  = 0x10F6
	};

	class MemoryObject
	{
	public:
		bool isNull() const { return id == 0; }
		cl_mem memoryId() const { return id; }
		Context* context() const { return ctx; }

		EAccess access() const;
		EMemoryLocation memoryLocation() const;
		EObjectType type() const;
		cl_mem_flags flags() const;
		void* hostPointer() const;
		size_t size() const;

	protected:
		// Disable instantiating base class 
		MemoryObject(Context* ctx = nullptr) : ctx(ctx), id(0) {}
		MemoryObject(Context* ctx, cl_mem id)
			: ctx(ctx), id(id) {}
		~MemoryObject();

		void setMemoryId(Context* ctx, cl_mem id);

	protected:
		Context* ctx;
		cl_mem id;

	private:
		// Disable copying
		MemoryObject(const MemoryObject& other);
		MemoryObject& operator=(const MemoryObject& other);
	};
}
