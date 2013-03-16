#include "MemoryObject.h"

#if !defined(CL_MEM_USE_PERSISTENT_MEM_AMD)
#define CL_MEM_USE_PERSISTENT_MEM_AMD 0x40
#endif

namespace clw
{
    namespace detail
    {
        template<typename Value>
        Value memObjectInfo(cl_mem id, cl_mem_info info)
        {
            Value value;
            cl_int error;
            if((error = clGetMemObjectInfo(id, info, sizeof(Value), 
                    &value, nullptr)) != CL_SUCCESS)
            {
                reportError("memObjectInfo(): ", error);
                return Value(0);
            }
            return value;
        }
    }

    MemoryObject::~MemoryObject()
    {
        if(_id)
            clReleaseMemObject(_id);
    }

    EAccess MemoryObject::access() const
    {
        return EAccess(flags() & 
            (CL_MEM_READ_WRITE |
             CL_MEM_WRITE_ONLY |
             CL_MEM_READ_ONLY)
        );
    }

    EMemoryLocation MemoryObject::memoryLocation() const
    {
        return EMemoryLocation(flags() & 
            (CL_MEM_USE_HOST_PTR |
             CL_MEM_ALLOC_HOST_PTR |
             CL_MEM_USE_PERSISTENT_MEM_AMD)
        );
    }

    EObjectType MemoryObject::type() const
    {
        return EObjectType(detail::memObjectInfo<cl_mem_object_type>(_id, CL_MEM_TYPE));
    }

    cl_mem_flags MemoryObject::flags() const
    {
        return detail::memObjectInfo<cl_mem_flags>(_id, CL_MEM_FLAGS);
    }

    void* MemoryObject::hostPointer() const
    {
        return detail::memObjectInfo<void*>(_id, CL_MEM_HOST_PTR);
    }

    size_t MemoryObject::size() const
    {
        return detail::memObjectInfo<size_t>(_id, CL_MEM_SIZE);
    }

    void MemoryObject::setMemoryId(Context* ctx, cl_mem id)
    {
        _ctx = ctx;
        if (id)
            clRetainMemObject(id);
        if (_id)
            clReleaseMemObject(_id);
        _id = id;
    }
}
