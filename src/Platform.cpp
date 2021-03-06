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

#include "clw/Platform.h"
#include "details.h"

namespace clw
{
    namespace detail
    {
        vector<cl_platform_id> platformIDs()
        {
            cl_uint num;
            cl_int error;
            if((error = clGetPlatformIDs(0, 
                    nullptr, &num)) != CL_SUCCESS)
            {
                reportError("platformIDs(): ", error);
                return vector<cl_platform_id>();
            }
            vector<cl_platform_id> pids(num);
            if((error = clGetPlatformIDs(num, 
                    pids.data(), nullptr)) != CL_SUCCESS)
            {
                reportError("platformIDs(): ", error);
                return vector<cl_platform_id>();
            }
            return pids;
        }

        string platformInfo(cl_platform_id _id, cl_platform_info name)
        {
            size_t size;
            cl_int error = CL_SUCCESS;
            if(!_id || (error = clGetPlatformInfo(_id, name,
                    0, nullptr, &size)) != CL_SUCCESS)
            {
                reportError("platfromInfo(): ", error);
                return string();
            }
            vector<char> infoBuf(size);
            if((error = clGetPlatformInfo(_id, name, size, 
                    infoBuf.data(), &size)) != CL_SUCCESS)
            {
                reportError("platfromInfo(): ", error);
                return string();
            }
            return string(infoBuf.data());
        }
    }

    string Platform::versionString() const
    {
        return detail::platformInfo(_id, CL_PLATFORM_VERSION);
    }

    EPlatformVersion Platform::version() const
    {
        // Format returned by clGetPlatformInfo with CL_PLATFORM_VERSION is: 
        // OpenCL<space><major_version.minor_version><space><platform-specific information>

        static const string prefix("OpenCL ");

        string ver = detail::platformInfo(_id, CL_PLATFORM_VERSION);
        if(!prefix.compare(0, prefix.length(), ver))
            return EPlatformVersion(0);

        ver = ver.substr(prefix.length());
        string::size_type pos = 0;
        while(ver[pos] != '.' && pos < ver.length())
            ++pos;
        if(pos == ver.length())
            return EPlatformVersion(0);

        int major = std::stoi(ver.substr(0, pos));
        string::size_type mpos = pos + 1;

        while(ver[mpos] != ' ' && mpos < ver.length())
            ++mpos;
        if(mpos == ver.length())
            return EPlatformVersion(0);

        int minor = std::stoi(ver.substr(pos+1, mpos-(pos+1)));

        switch(major)
        {
        case 1:
            switch(minor)
            {
            case 0: return EPlatformVersion::v1_0;
            case 1: return EPlatformVersion::v1_1;
            case 2: return EPlatformVersion::v1_2;
            default: return EPlatformVersion(0);
            }
        case 2:
            switch (minor)
            {
            case 0: return EPlatformVersion::v2_0;
            case 1: return EPlatformVersion::v2_1;
            case 2: return EPlatformVersion::v2_2;
            default: return EPlatformVersion(0);
            }
        default:
            return EPlatformVersion(0);
        }
    }

    string Platform::name() const
    {
        return detail::platformInfo(_id, CL_PLATFORM_NAME);
    }

    string Platform::vendor() const
    {
        return detail::platformInfo(_id, CL_PLATFORM_VENDOR);
    }

    string Platform::extensionSuffix() const
    {
        return detail::platformInfo(_id, CL_PLATFORM_ICD_SUFFIX_KHR);
    }

    string Platform::profile() const
    {
        return detail::platformInfo(_id, CL_PLATFORM_PROFILE);
    }

    vector<string> Platform::extensions() const
    {
        string exts = detail::platformInfo(_id, CL_PLATFORM_EXTENSIONS);
        return detail::tokenize(exts, ' ');
    }

    vector<Platform> availablePlatforms()
    {
        vector<cl_platform_id> pids = detail::platformIDs();
        if(pids.empty())
            return vector<Platform>();
        vector<Platform> platforms(pids.size());
        for(size_t i = 0; i < pids.size(); ++i)
            platforms[i] = Platform(pids[i]);
        return platforms;
    }

    Platform defaultPlatform()
    {
        cl_platform_id pid;
        if(clGetPlatformIDs(1, &pid, nullptr) != CL_SUCCESS)
            return Platform();
        return Platform(pid);
    }
}
