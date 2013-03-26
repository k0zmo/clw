#pragma once

#include "Prerequisites.h"

namespace clw
{
    typedef vector<unsigned char> ByteCode;

    class Program
    {
    public:
        Program() : _ctx(nullptr), _id(0), _built(false) {}
        Program(Context* ctx, cl_program id)
            : _ctx(ctx), _id(id), _built(false) {}
        ~Program();
        
        Program(const Program& other);
        Program& operator=(const Program& other);
        
        bool isNull() const { return _id == 0; }
        
        Context* context() const { return _ctx; }
        cl_program programId() const { return _id; }
        
        bool isBuilt() const { return _built; }
        bool build(const string& options = string());
        // !TODO:
        //bool build(const vector<Device> devices, const string& options = string());
        
        string builtOptions() const { return _options; }
        string log() const;
        
        vector<Device> devices() const;
        string sourceCode() const;
        vector<ByteCode> binaries() const;

        Kernel createKernel(const char* name) const;
        Kernel createKernel(const string& name) const;
        vector<Kernel> createKernels() const;
        
    private:
        Context* _ctx;
        cl_program _id;
        string _options;
        bool _built;
    };
}