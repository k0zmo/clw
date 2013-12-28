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
    enum EEventStatus
    {
        Status_Errored    = -1,
        Status_Complete   =  0,
        Status_Running    =  1,
        Status_Submitted  =  2,
        Status_Queued     =  3
    };

    enum ECommandType
    {
        Command_NDRangeKernel         = 0x11F0,
        Command_Task                  = 0x11F1,
        Command_NativeKernel          = 0x11F2,
        Command_ReadBuffer            = 0x11F3,
        Command_WriteBuffer           = 0x11F4,
        Command_CopyBuffer            = 0x11F5,
        Command_ReadImage             = 0x11F6,
        Command_WriteImage            = 0x11F7,
        Command_CopyImage             = 0x11F8,
        Command_CopyBufferToImage     = 0x11F9,
        Command_CopyImageToBuffer     = 0x11FA,
        Command_MapBuffer             = 0x11FB,
        Command_MapImage              = 0x11FC,
        Command_UnmapMemObject        = 0x11FD,
        Command_Marker                = 0x11FE,
        Command_AcquireGLObjects      = 0x11FF,
        Command_ReleaseGLObjects      = 0x1200,
        // OpenCL 1.1
        Command_ReadBufferRect        = 0x1201,
        Command_WriteBufferRect       = 0x1202,
        Command_CopyBufferRect        = 0x1203,
        Command_User                  = 0x1204,
        // OpenCL 1.2
        Command_Barrier               = 0x1205,
        Command_MigrateMemObjects     = 0x1206,
        Command_FillBuffer            = 0x1207,
        Command_FillImage             = 0x1208
    };

    typedef function<void(EEventStatus status)> EventCallback;

    class CLW_EXPORT Event
    {
    public:
        Event() : _id(0) {}
        Event(cl_event _id) : _id(_id) {}
        ~Event();

        Event(const Event& other);
        Event& operator=(const Event& other);

        bool isNull() const { return _id == 0; }

        cl_event eventId() const { return _id; }

        EEventStatus status() const;
        ECommandType commandType() const;

        void setCallback(EEventStatus status, EventCallback cb);
        void waitForFinished();

        uint64_t queueTime() const;
        uint64_t submitTime() const;
        uint64_t startTime() const;
        uint64_t finishTime() const;

    protected:
        cl_event _id;
        EventCallback _callback;
    };

    class CLW_EXPORT UserEvent : public Event
    {
    public:
        UserEvent() : Event() {}
        UserEvent(cl_event _id) : Event(_id) {}
        ~UserEvent() {}

        UserEvent(const UserEvent& other);
        UserEvent& operator=(const UserEvent& other);

        void setStatus(EEventStatus status);
    };

    class CLW_EXPORT EventList
    {
    public:
        EventList() {}
        EventList(const Event& event);
        ~EventList();

        EventList(const EventList& other);
        EventList& operator=(const EventList& other);

        void waitForFinished();

        bool isEmpty() const { return _events.empty(); }
        size_t size() const { return _events.size(); }

        void append(const Event& event);
        void append(const EventList& other);
        void remove(const Event& event);
        Event at(size_t index) const;
        bool contains(const Event& event) const;

        operator const cl_event*() const;

        EventList& operator+=(const Event& event);
        EventList& operator+=(const EventList& other);
    private:
        vector<cl_event> _events;
    };

    inline EventList::operator const cl_event*() const
    {
        return _events.empty() ? nullptr : _events.data();
    }

    inline EventList& EventList::operator+=(const Event& event)
    {
        append(event);
        return *this;
    }

    inline EventList& EventList::operator+=(const EventList& other)
    {
        append(other);
        return *this;
    }
}