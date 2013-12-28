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

#include <clw/clw.h>
#include <iostream>
#include <string>

std::string deviceTypeName(clw::EDeviceType type)
{
    switch(type)
    {
    case clw::Cpu: return std::string("Cpu");
    case clw::Gpu: return std::string("Gpu");
    case clw::Accelerator: return std::string("Accelerator"); 
    case clw::Custom: return std::string("Custom");
    default: return std::string("Undefined");
    }
}

std::string boolName(bool v)
{
    return v ? std::string("yes") : std::string("no");
}

std::string cacheTypeName(clw::ECacheType type)
{
    switch(type)
    {
    case clw::Cache_NoChache: return "No chache";
    case clw::Cache_ReadOnlyCache: return "Read-only cache";
    case clw::Cache_ReadWriteCache: return "Read-write cache";
    default: return std::string("Undefined");
    }
}

std::string floatCapabilitiesName(clw::EFloatCapabilities caps)
{
    std::string accum;
    if(caps & clw::Capability_Denorm)
        accum += "Denorm ";
    if(caps & clw::Capability_InfinityAndNaN)
        accum += "InfinityAndNaN ";
    if(caps & clw::Capability_RoundToNearest)
        accum += "RoundToNearest ";
    if(caps & clw::Capability_RoundToZero)
        accum += "RoundToZero ";
    if(caps & clw::Capability_RoundToInf)
        accum += "RoundToInf ";
    if(caps & clw::Capability_FusedMultiplyAdd)
        accum += "FusedMultiplyAdd ";
    if(caps & clw::Capability_CorrectlyRoundedDivideSqrt)
        accum += "CorrectlyRoundedDivideSqrt ";
    if(caps & clw::Capability_SoftwareFloat)
        accum += "SoftwareFloat ";

    if(accum.empty())
        accum += "Not supported";
    return accum;
}

std::string memorySizeName(uint64_t memSize)
{
#if defined(_MSC_VER)
#  define snprintf _snprintf
#endif
    char buf[64];
    if(memSize >= 1024 * 1024)
        snprintf(buf, sizeof(buf)/sizeof(char), "%d MB", int(memSize / (1024 * 1024)));
    else
        snprintf(buf, sizeof(buf)/sizeof(char), "%d kB", int(memSize / 1024));
    return std::string(buf);
}

int main()
{
    auto platforms = clw::availablePlatforms();
    for(const clw::Platform& platform : platforms)
    {
        std::cout << "OpenCL Platform:\n";
        std::cout << "    Profile          : " << platform.profile() << std::endl;
        std::cout << "    Version          : " << platform.versionString() << std::endl;
        std::cout << "    Name             : " << platform.name() << std::endl;
        std::cout << "    Vendor           : " << platform.vendor() << std::endl;
        std::cout << "    Extension suffix : " << platform.extensionSuffix() << std::endl;

        auto exts = platform.extensions();
        std::cout << "    Extensions       : (" << exts.size() << ")\n";
        for(const std::string& extension : exts)
            std::cout << "        " << extension << std::endl;
        std::cout << "\n";

        auto devices = clw::devices(clw::All, platform);
        for(const clw::Device& device : devices)
        {
            std::cout << "OpenCL Device:                    (available: " << boolName(device.isAvailable()) << ")\n";
            std::cout << "    Name                        : " << device.name() << std::endl;
            std::cout << "    Version                     : " << device.version() << std::endl;
            std::cout << "    Vendor                      : " << device.vendor() << std::endl;
            std::cout << "    Device type                 : " << deviceTypeName(device.deviceType()) << std::endl;											  
            std::cout << "    Driver version              : " << device.driverVersion() << std::endl;
            std::cout << "    Language version            : " << device.languageVersion() << std::endl;
            std::cout << "    Is compiler available       : " << boolName(device.isCompilerAvailable()) << std::endl;
            std::cout << "    Supports double             : " << boolName(device.supportsDouble()) << std::endl;
            std::cout << "    Supports half float         : " << boolName(device.supportsHalf()) << std::endl;
            std::cout << "    Supports error correction   : " << boolName(device.supportsErrorCorrection()) << std::endl;
            std::cout << "    Supports native kernel      : " << boolName(device.supportsNativeKernel()) << std::endl;
            std::cout << "    Supports out of order exec. : " << boolName(device.supportsOutOfOrderExecution()) << std::endl;
            std::cout << "    Supports images             : " << boolName(device.supportsImages()) << std::endl;
            std::cout << "    Is unified memory           : " << boolName(device.isUnifiedMemory()) << std::endl;
            std::cout << "    Separate local memory       : " << boolName(device.isLocalMemorySeparate()) << std::endl;
            std::cout << "    Address bitness             : " << device.addressBitness() << std::endl;
            std::cout << "    Endianess                   : " << (device.isLittleEndian() ? "little" : "big") << std::endl;
            std::cout << "    Clock frequency             : " << device.clockFrequency() << " MHz" << std::endl;
            std::cout << "    Compute units               : " << device.computeUnits() << std::endl;
            std::cout << "    Default alignment           : " << device.defaultAlignment() << " bits" << std::endl;
            std::cout << "    Minimum alignment           : " << device.minimumAlignment() << " bytes" << std::endl;
            std::cout << "    Float capabilities          : " << floatCapabilitiesName(device.floatCapabilities()) << std::endl;
            std::cout << "    Double capabilities         : " << floatCapabilitiesName(device.doubleCapabilities()) << std::endl;
            std::cout << "    Half capabilities           : " << floatCapabilitiesName(device.halfCapabilities()) << std::endl;
            std::cout << "    GMEM cache size             : " << memorySizeName(device.globalMemoryCacheSize()) << std::endl;
            std::cout << "    GMEM cache line size        : " << device.globalMemoryCacheLineSize() << " bytes" << std::endl;
            std::cout << "    GMEM cache type             : " << cacheTypeName(device.globalMemoryCacheType()) << std::endl;
            std::cout << "    GMEM size                   : " << memorySizeName(device.globalMemorySize()) << std::endl;
            std::cout << "    LMEM size                   : " << memorySizeName(device.localMemorySize()) << std::endl;
            std::cout << "    Max. single allocation size : " << memorySizeName(device.maximumAllocationSize()) << std::endl;
            std::cout << "    Max. image 2D size          : " << device.maximumImage2DSize() << std::endl;
            std::cout << "    Max. image 3D size          : " << device.maximumImage3DSize() << std::endl;
            std::cout << "    Max. images (read)          : " << device.maximumReadImages() << std::endl;
            std::cout << "    Max. images (write)         : " << device.maximumWriteImages() << std::endl;
            std::cout << "    Max. samplers               : " << device.maximumSamplers() << std::endl;
            std::cout << "    Max. const arguments        : " << device.maximumConstantArguments() << std::endl;
            std::cout << "    Max. constant buffer size   : " << memorySizeName(device.maximumConstantBufferSize()) << std::endl;
            std::cout << "    Max. parameter size         : " << device.maximumParameterBytes() << " bytes" << std::endl;
            std::cout << "    Max. work items             : " << device.maximumWorkItemSize() << std::endl;
            std::cout << "    Max. work items per group   : " << device.maximumWorkItemsPerGroup() << std::endl;
            std::cout << "    Timer resolution            : " << device.profilingTimerResolution() << " nsec" << std::endl;
            std::cout << "    Native vectors width        : char" << device.nativeCharVectorWidth()
                      << " short" << device.nativeShortVectorWidth() 
                      << " int" << device.nativeIntVectorWidth() 
                      << " long" << device.nativeLongVectorWidth() 
                      << " float" << device.nativeFloatVectorWidth() 
                      << " double" << device.nativeDoubleVectorWidth() 
                      << " half" << device.nativeHalfVectorWidth()
                      << std::endl;
            std::cout << "    Preferred vectors width     : char" << device.preferredCharVectorWidth()
                      << " short" << device.preferredShortVectorWidth() 
                      << " int" << device.preferredIntVectorWidth() 
                      << " long" << device.preferredLongVectorWidth() 
                      << " float" << device.preferredFloatVectorWidth() 
                      << " double" << device.preferredDoubleVectorWidth() 
                      << " half" << device.preferredHalfVectorWidth()
                      << std::endl;

            // Radeons specific device attributes
            if(device.vendor() == std::string("Advanced Micro Devices, Inc.") &&
               device.deviceType() == clw::Gpu)
            {
                auto gmemFree = device.globalFreeMemory();
                std::cout << "    AMD specific                : " << std::endl;
                std::cout << "      Board name                : " << device.boardName() << std::endl;
                if(gmemFree.size() == 2)
                    std::cout << "      Free GMEM                 : " << memorySizeName(gmemFree[1] * 1024) 
                              << "/" << memorySizeName(gmemFree[0] * 1024) << std::endl;
                std::cout << "      SIMD per Compute Unit     : " << device.simdPerComputeUnit() << std::endl;
                std::cout << "      SIMD width                : " << device.simdWidth() << std::endl;
                std::cout << "      SIMD Instruction width    : " << device.simdInstructionWidth() << std::endl;
                std::cout << "      Wavefront width           : " << device.wavefrontWidth() << std::endl;
                std::cout << "      GMEM channels             : " << device.globalMemoryChannels() << std::endl;
                std::cout << "      GMEM channel banks        : " << device.globalMemoryChannelBanks() << std::endl;
                std::cout << "      GMEM channel bank width   : " << device.globalMemoryChannelBankWidth() << std::endl;											     
                std::cout << "      LMEM size per CU          : " << device.localMemorySizePerComputeUnit() << std::endl;											     
                std::cout << "      LMEM memory banks         : " << device.localMemoryBanks() << std::endl;				
            }
            else if(device.vendor() == std::string("NVIDIA") &&
                    device.deviceType() == clw::Gpu)
            {
                std::cout << "    NVIDIA specific             : " << std::endl;
                std::cout << "      Compute Capability        : " << device.computeCapabilityMajor() << "." << device.computeCapabilityMinor() << std::endl;
                std::cout << "      Registers per Block       : " << device.registersPerBlock() << std::endl;
                std::cout << "      Warp size                 : " << device.warpSize() << std::endl;
                std::cout << "      GPU overlap               : " << device.gpuOverlap() << std::endl;
                std::cout << "      Kernel execution timeout  : " << device.kernelExecutionTimeout() << std::endl;
                std::cout << "      Integrated memory         : " << device.integratedMemory() << std::endl;
            }

            auto exts = device.extensions();
            std::cout << "    Extensions                  : (" << exts.size() << ")\n";
            for(const std::string& extension : exts)
                std::cout << "        " << extension << std::endl;
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}