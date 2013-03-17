#include "Event.h"
#include <algorithm>

namespace clw
{
    namespace detail
    {
        template<typename Value>
        Value eventInfo(cl_event _id, cl_event_info info)
        {
            Value value;
            cl_int error = CL_SUCCESS;			
            if(!_id || (error = clGetEventInfo(_id, info, 
                    sizeof(Value), &value, nullptr)) != CL_SUCCESS)
            {
                reportError("eventInfo(): ", error);
                return Value(-1);
            }
            return value;
        }

        cl_ulong eventProfilingInfo(cl_event _id, cl_profiling_info info)
        {
            cl_ulong value;
            cl_int error = CL_SUCCESS;
            if(!_id || (error = clGetEventProfilingInfo(_id, info, 
                    sizeof(cl_ulong), &value, nullptr)) != CL_SUCCESS)
            {
                reportError("eventInfo(): ", error);
                return cl_ulong(0);
            }
            return value;
        }

        void CL_CALLBACK callback_priv(cl_event event, 
                                       cl_int event_command_exec_status,
                                       void *user_data)
        {
            EventCallback* pfn = static_cast<EventCallback*>(user_data);
            (*pfn)(EEventStatus(event_command_exec_status));
        }
    }

    Event::~Event()
    {
        if(_id)
            clReleaseEvent(_id);
    }

    Event::Event(const Event& other)
        : _id(other._id)
    {
        if(_id)
            clRetainEvent(_id);
    }

    Event& Event::operator=(const Event& other)
    {
        if(other._id)
            clRetainEvent(other._id);
        if(_id)
            clReleaseEvent(_id);
        _id = other._id;
        return *this;
    }

    EEventStatus Event::status() const
    {
        return EEventStatus(detail::eventInfo<cl_int>
            (_id, CL_EVENT_COMMAND_EXECUTION_STATUS));
    }

    ECommandType Event::commandType() const
    {
        return ECommandType(detail::eventInfo<cl_command_type>
            (_id, CL_EVENT_COMMAND_TYPE));
    }

    void Event::setCallback(EEventStatus status, EventCallback cb)
    {
        _callback = cb;
        clSetEventCallback(_id, cl_int(status), 
            &detail::callback_priv, &_callback);
    }

    void Event::waitForFinished()
    {
        if(_id)
        {
            cl_int error;
            if((error = clWaitForEvents(1, &_id)) != CL_SUCCESS)
                detail::reportError("Event::waitForFinished(): ", error);
        }
    }

    uint64_t Event::queueTime() const
    {
        return uint64_t(detail::eventProfilingInfo
            (_id, CL_PROFILING_COMMAND_QUEUED));
    }

    uint64_t Event::submitTime() const
    {
        return uint64_t(detail::eventProfilingInfo
            (_id, CL_PROFILING_COMMAND_SUBMIT));
    }

    uint64_t Event::startTime() const
    {
        return uint64_t(detail::eventProfilingInfo
            (_id, CL_PROFILING_COMMAND_START));
    }

    uint64_t Event::finishTime() const
    {
        return uint64_t(detail::eventProfilingInfo
            (_id, CL_PROFILING_COMMAND_END));
    }

    UserEvent::UserEvent(const UserEvent& other)
        : Event(other._id)
    {
        if(_id)
            clRetainEvent(_id);
    }

    UserEvent& UserEvent::operator=(const UserEvent& other)
    {
        if(other._id)
            clRetainEvent(other._id);
        if(_id)
            clReleaseEvent(_id);
        _id = other._id;
        return *this;
    }

    void UserEvent::setStatus(EEventStatus status)
    {
#if defined(HAVE_OPENCL_1_1)
        cl_int error = CL_SUCCESS;
        if(!_id || ((error = 
                clSetUserEventStatus(_id, cl_int(status))) != CL_SUCCESS))
        {
            detail::reportError("UserEvent::setStatus(): ", error);
        }
#else
        (void)status;
#endif
    }

    EventList::EventList(const Event& event)
    {
        append(event);
    }

    EventList::~EventList()
    {
        for(size_t i = 0; i < _events.size(); ++i)
            clReleaseEvent(_events[i]);
    }

    EventList::EventList(const EventList& other)
        : _events(other._events)
    {
        for(size_t i = 0; i < _events.size(); ++i)
            clRetainEvent(_events[i]);
    }

    EventList& EventList::operator=(const EventList& other)
    {
        for(size_t i = 0; i < _events.size(); ++i)
            clReleaseEvent(_events[i]);
        _events = other._events;
        for(size_t i = 0; i < _events.size(); ++i)
            clRetainEvent(_events[i]);
        return *this;
    }

    void EventList::waitForFinished()
    {
        if(_events.empty())
            return;
        cl_int error;
        if((error = clWaitForEvents(cl_uint(_events.size()),
                operator const cl_event*())) != CL_SUCCESS)
            detail::reportError("EventList::waitForFinished() ", error);
    }

    void EventList::append(const Event& event)
    {
        cl_event _id = event.eventId();
        if(_id)
        {
            clRetainEvent(_id);
            _events.push_back(_id);
        }
    }

    void EventList::append(const EventList& other)
    {
        for(size_t i = 0; i < other.size(); ++i)
        {
            cl_event e = other._events[i];
            clRetainEvent(e);
            _events.push_back(e);
        }
    }

    void EventList::remove(const Event& event)
    {
        _events.erase(
            std::remove(_events.begin(), _events.end(), event.eventId()),
            _events.end());
    }

    Event EventList::at(size_t index) const
    {
        if(index < _events.size())
        {
            cl_event e = _events[index];
            clRetainEvent(e);
            return Event(e);
        }
        return Event();
    }

    bool EventList::contains(const Event& event) const
    {
        auto it = std::find_if(_events.begin(), _events.end(),
            [&event](const cl_event& _id)
            {
                return _id == event.eventId();
            });
        return it != _events.end();
    }
}