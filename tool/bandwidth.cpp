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
#include <memory>

void profileCopies(clw::CommandQueue& queue, float* hostA, float* hostB,
                   clw::Buffer& device, unsigned nElems, const char* desc)
{
    std::cout << "  " << desc << " transfers\n";
    const auto bytes = sizeof(float) * nElems;

    clw::Event evt = queue.asyncWriteBuffer(device, hostA);
    evt.waitForFinished();
    uint64_t timeNs = evt.finishTime() - evt.startTime();
    std::cout << "  Host to device bandwidth (GB/s): "
              << (bytes * 1e-9) / (timeNs * 1e-9) << '\n';

    evt = queue.asyncReadBuffer(device, hostB);
    evt.waitForFinished();
    timeNs = evt.finishTime() - evt.startTime();
    std::cout << "  Device to host bandwidth (GB/s): "
              << (bytes * 1e-9) / (timeNs * 1e-9) << '\n';

    for (unsigned i = 0; i < nElems; ++i)
    {
        if (hostA[i] != hostB[i])
        {
            std::cout << "*** " << desc << " transfers failed ***\n";
            break;
        }
    }
}

void testBandwidth(clw::Context& ctx, clw::CommandQueue& queue, unsigned nElements)
{
    const unsigned bytes = nElements * sizeof(float);

    std::cout << "  Transfer size (MB): " << bytes / (1024 * 1024) << '\n';

    auto host1Buf = std::make_unique<float[]>(nElements);
    auto host2Buf = std::make_unique<float[]>(nElements);
    auto hostPinned1Buf = ctx.createBuffer(
        clw::EAccess::ReadOnly, clw::EMemoryLocation::AllocHostMemory, bytes);
    auto hostPinned2Buf = ctx.createBuffer(
        clw::EAccess::ReadOnly, clw::EMemoryLocation::AllocHostMemory, bytes);
    auto deviceBuf = ctx.createBuffer(clw::EAccess::ReadOnly,
                                      clw::EMemoryLocation::Device, bytes);

    auto hostPinned1 = queue.mapBuffer(
        hostPinned1Buf, clw::EMapAccess::Read | clw::EMapAccess::Write |
                            clw::EMapAccess::InvalidateRegion);
    auto hostPinned2 = queue.mapBuffer(
        hostPinned2Buf, clw::EMapAccess::Read | clw::EMapAccess::Write |
                            clw::EMapAccess::InvalidateRegion);

    for (unsigned i = 0U; i < nElements; ++i)
        host1Buf[i] = static_cast<float>(i);
    std::memcpy(hostPinned1, host1Buf.get(), bytes);

    std::memset(host2Buf.get(), 0, bytes);
    std::memset(hostPinned2, 0, bytes);

    profileCopies(queue, host1Buf.get(), host2Buf.get(), deviceBuf, nElements,
                  "Pageable");
    profileCopies(queue, reinterpret_cast<float*>(hostPinned1),
                  reinterpret_cast<float*>(hostPinned2), deviceBuf, nElements,
                  "Pinned");

    queue.unmap(hostPinned1Buf, hostPinned1);
    queue.unmap(hostPinned2Buf, hostPinned2);
}

int main()
{
    auto platforms = clw::availablePlatforms();
    for (const clw::Platform& platform : platforms)
    {
        auto devices = clw::devices(clw::EDeviceType::All, platform);
        for (const clw::Device& device : devices)
        {
            std::cout << device.name() << '/' << platform.name() << '\n';

            clw::Context ctx;
            if (ctx.create({device}))
            {
                auto queue = ctx.createCommandQueue(
                    device, clw::ECommandQueueProperty::ProfilingEnabled);
                testBandwidth(ctx, queue, 4 * 1024 * 1024);
            }
        }
    }
}
