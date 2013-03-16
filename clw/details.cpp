#include "Prerequisites.h"

#include <fstream>
#include <iostream>
#include <cstring>

namespace clw
{
    int compiledOpenCLVersion()
    {
#if defined(HAVE_OPENCL_1_2)
        return 120;
#elif defined(HAVE_OPENCL_1_1)
        return 110;
#else
        return 100;
#endif
    }

    namespace detail
    {
        vector<string> tokenize(const string& str, char delim, char group)
        {
            vector<string> tokens;
            if(str.empty())
                return tokens;

            int curr = 0;
            int start = 0;

            // upewnij sie czy string nie rozpoczyna sie od delimitersow
            start = curr = static_cast<int>(str.find_first_not_of(delim));

            while(str[curr])
            {
                if(str[curr] == group)
                {
                    curr = static_cast<int>(str.find_first_of(group, curr+1));
                    if((size_t)curr == string::npos)
                        return vector<string>();

                    string token = str.substr(start+1, curr-start-1);
                    tokens.push_back(token);
                    start = curr + 1;
                }
                else if(str[curr] == delim)
                {
                    if(str[curr-1] != delim && str[curr-1] != group)
                    {
                        string token = str.substr(start, curr-start);
                        tokens.push_back(token);
                    }
                    start = curr + 1;
                }
                ++curr;
            }

            if(tokens.size() == 0)
            {
                tokens.push_back(str);
                return tokens;
            }

            // dla ostatniego token'a
            if(str[curr-1] != delim && str[curr-1] != group)
            {
                string token = str.substr(start, curr - 1);
                tokens.push_back(token);
            }

            return tokens;
        }

        void trim(string* str, bool left, bool right)
        {
            if(left)
            {
                size_t pos = str->find_first_not_of(" \t\r");
                if(pos != 0 && pos != string::npos)
                {
                    *str = str->substr(pos);
                }
            }

            if(right)
            {
                size_t pos = str->find_last_not_of(" \t\r");
                if(pos < str->length()-1)
                {
                    *str = str->substr(0, pos+1);
                }
            }
        }

        bool readAsString(const string& filename, string* contents)
        {
            std::ifstream strm;
            strm.open(filename.c_str(), std::ios::binary | std::ios_base::in);
            if(!strm.is_open())
            {
                std::cerr << "Unable to open file " << filename << std::endl;
                return false;
            }
            strm.seekg(0, std::ios::end);
            contents->reserve(static_cast<size_t>(strm.tellg()));
            strm.seekg(0);

            contents->assign(std::istreambuf_iterator<char>(strm),
                std::istreambuf_iterator<char>());
            return true;
        }
    }
}