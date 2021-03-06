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

#include "clw/Prerequisites.h"

namespace clw
{
    typedef vector<unsigned char> ByteCode;

    class CLW_EXPORT Program
    {
    public:
        Program() : _ctx(nullptr), _id(0), _built(false) {}
        Program(Context* ctx, cl_program id)
            : _ctx(ctx), _id(id), _built(false) {}
        ~Program();
        
        Program(const Program& other);
        Program& operator=(const Program& other);

        Program(Program&& other);
        Program& operator=(Program&& other);
        
        bool isNull() const { return _id == 0; }
        
        Context* context() const { return _ctx; }
        cl_program programId() const { return _id; }
        
        bool isBuilt() const { return _built; }
        bool build(string options = string());
        // !TODO:
        //bool build(const vector<Device> devices, const string& options = string());
        
        string buildOptions() const { return _options; }
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