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

#include "Device.h"
#include "Platform.h"

namespace clw
{
    namespace Filter
    {
        struct AllFilter
        {
            bool operator()(const clw::Device&) const { return true; }
        };
        
        template<class FilterLeft, class FilterRight>
        struct FilterAnd
        {
            FilterAnd(const FilterLeft& left, const FilterRight& right)
                : _left(left), _right(right) {}
                
            bool operator()(const clw::Device& dev) const
            {
                return _left(dev) && _right(dev);
            }

        private:
            FilterLeft _left;
            FilterRight _right;
        };
        
        template<class FilterLeft, class FilterRight>
        struct FilterOr
        {
            FilterOr(const FilterLeft& left, const FilterRight& right)
                : _left(left), _right(right) {}
                
            bool operator()(const clw::Device& dev) const
            {
                return _left(dev) || _right(dev);
            }

        private:
            FilterLeft _left;
            FilterRight _right;
        };
        
        template<class Filter>
        struct FilterNot
        {
            FilterNot(const Filter& filter)
                : _filter(filter) {}
                
            bool operator()(const clw::Device& dev) const
            {
                return !_filter(dev);
            }
            
        private:
            Filter _filter;
        };
        
        template<class FilterLeft, class FilterRight>
        FilterAnd<FilterLeft, FilterRight> operator&&(const FilterLeft& left, const FilterRight& right)
        {
            return FilterAnd<FilterLeft, FilterRight>(left, right);
        }
        
        template<class FilterLeft, class FilterRight>
        FilterOr<FilterLeft, FilterRight> operator||(const FilterLeft& left, const FilterRight& right)
        {
            return FilterOr<FilterLeft, FilterRight>(left, right);
        }
        
        template<class Filter>
        FilterNot<Filter> operator!(const Filter& filter)
        {
            return FilterNot<Filter>(filter);
        }

        struct PlatformVendor
        {
            PlatformVendor(EPlatformVendor platformVendor)
                : _platformVendor(platformVendor) {}

            bool operator()(const clw::Device& dev) const 
            {
                return dev.platform().vendorEnum() == _platformVendor;
            }

        private:
            EPlatformVendor _platformVendor;
        };

        struct DeviceType
        {
            DeviceType(EDeviceType deviceType)
                : _deviceType(deviceType) {}

            bool operator()(const clw::Device& dev) const 
            {
                return dev.deviceType() == _deviceType;
            }

        private:
            EDeviceType _deviceType;
        };
    }

    // This function should be use with much care.
    // Context must be created with devices from only one platform
    template<class Filter>
    vector<Device> deviceFiltered(Filter filter = Filter::AllFilter)
    {
        vector<Device> devs = allDevices();
        vector<Device> res;
        for(const Device& dev : devs)
        {
            if(filter(dev))
                res.push_back(dev);
        }
        return res;
    }
}