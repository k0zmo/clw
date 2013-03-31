#pragma once

#include "Prerequisites.h"

namespace clw
{
    namespace detail
    {
        void reportError(const char* name, cl_int eid);
        bool supportsExtension(const string& list, const char* ext);
        vector<string> tokenize(const string& str, char delim, char group = 0);
        void trim(string* str, bool left, bool right);
        bool readAsString(const string& filename, string* contents);
    }
}