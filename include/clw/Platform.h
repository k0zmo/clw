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

namespace clw
{
    enum class EPlatformVersion
    {
        v1_0 = 1,
        v1_1 = 2,
        v1_2 = 3,
        v2_0 = 4,
        v2_1 = 5,
        v2_2 = 6
    };

    enum class EPlatformVendor
    {
        AMD,
        Intel,
        NVIDIA,
        Undefined
    };

    // Thin wrapper over OpenCL platform 
    class CLW_EXPORT Platform
    {
    public:
        Platform() : _id(0) {}
        Platform(cl_platform_id id) : _id(id) {}

        bool isNull() const { return _id == 0; }
        EPlatformVersion version() const;
        string versionString() const;
        string name() const;
        string vendor() const;
        string extensionSuffix() const;
        string profile() const;
        vector<string> extensions() const;

        // Not fully tested - cl_khr_icd must be supported
        inline EPlatformVendor vendorEnum() const;

        cl_platform_id platformId() const { return _id; }

        // !TODO: unloadCompiler(); - only for 1.2

    private:
        cl_platform_id _id;
    };

    CLW_EXPORT vector<Platform> availablePlatforms();
    CLW_EXPORT Platform defaultPlatform();

    inline EPlatformVendor Platform::vendorEnum() const
    {
        string suffix = extensionSuffix();
        if(suffix == "AMD")
            return EPlatformVendor::AMD;
        else if(suffix == "INTEL")
            return EPlatformVendor::Intel;
        else if(suffix == "NV")
            return EPlatformVendor::NVIDIA;
        else
            return EPlatformVendor::Undefined;
    }
}