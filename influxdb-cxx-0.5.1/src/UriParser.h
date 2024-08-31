/*
Copyright (c) 2013 Covenant Eyes

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef INFLUXDATA_HTTPPARSER_H
#define INFLUXDATA_HTTPPARSER_H

#include <iostream>
#include <string>
#include <stdlib.h>


namespace http {
    struct url {
        std::string protocol, user, password, host, path, search, url;
        int port;
    };


    //--- Helper Functions -------------------------------------------------------------~
    static inline std::string TailSlice(std::string &subject, std::string delimiter, bool keep_delim=false) {
        // Chops off the delimiter and everything that follows (destructively)
        // returns everything after the delimiter
        auto delimiter_location = subject.find(delimiter);
        auto delimiter_length = delimiter.length();
        std::string output = "";

        if (delimiter_location < std::string::npos) {
            auto start = keep_delim ? delimiter_location : delimiter_location + delimiter_length;
            auto end = subject.length() - start;
            output = subject.substr(start, end);
            subject = subject.substr(0, delimiter_location);
        }
        return output;
    }

    static inline std::string HeadSlice(std::string &subject, std::string delimiter) {
        // Chops off the delimiter and everything that precedes (destructively)
        // returns everthing before the delimeter
        auto delimiter_location = subject.find(delimiter);
        auto delimiter_length = delimiter.length();
        std::string output = "";
        if (delimiter_location < std::string::npos) {
            output = subject.substr(0, delimiter_location);
            subject = subject.substr(delimiter_location + delimiter_length, subject.length() - (delimiter_location + delimiter_length));
        }
        return output;
    }


    //--- Extractors -------------------------------------------------------------------~
    static inline int ExtractPort(std::string &hostport) {
        int port;
        std::string portstring = TailSlice(hostport, ":");
        try { port = atoi(portstring.c_str()); }
        catch (std::exception &e) { port = -1; }
        return port;
    }

    static inline std::string ExtractPath(std::string &in) { return TailSlice(in, "/", true); }
    static inline std::string ExtractProtocol(std::string &in) { return HeadSlice(in, "://"); }
    static inline std::string ExtractSearch(std::string &in) { return TailSlice(in, "?"); }
    static inline std::string ExtractPassword(std::string &userpass) { return TailSlice(userpass, ":"); }
    static inline std::string ExtractUserpass(std::string &in) { return HeadSlice(in, "@"); }


    //--- Public Interface -------------------------------------------------------------~
    static inline url ParseHttpUrl(std::string &in) {
        url ret;
        ret.url = in;
        ret.port = -1;
        ret.protocol = ExtractProtocol(in);
        ret.search = ExtractSearch(in);
        ret.path = ExtractPath(in);
        std::string userpass = ExtractUserpass(in);
        ret.password = ExtractPassword(userpass);
        ret.user = userpass;
        ret.port = ExtractPort(in);
        ret.host = in;

        return ret;
    }
}
#endif
