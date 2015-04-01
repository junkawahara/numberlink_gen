//
// TimeCount.h
//
// Copyright (c) 2012 - 2015 JST ERATO Minato project, Jun Kawahara, and other contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#endif

struct TimeCount {
    long long int user_time;
    long long int system_time;
};

inline TimeCount GetProcessTime() {
    TimeCount tc;
#ifndef WIN32
    struct tms buf;
#endif
    
#ifdef WIN32
    FILETIME creationTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    
    GetProcessTimes(GetCurrentProcess(),
        &creationTime,
        &exitTime,
        &kernelTime,
        &userTime);
    
    tc.user_time = userTime.dwHighDateTime * 0x100000000 + userTime.dwLowDateTime;
    tc.system_time = kernelTime.dwHighDateTime * 0x100000000 +  kernelTime.dwLowDateTime;
#else
    times(&buf);
    tc.user_time = buf.tms_utime;
    tc.system_time = buf.tms_stime;
#endif
    
    return tc;
}

inline double GetUserTime(TimeCount start_tc, TimeCount end_tc) {
#ifdef WIN32
    return (double)(end_tc.user_time - start_tc.user_time) / 10000000;
#else
    return (double)(end_tc.user_time - start_tc.user_time) / sysconf(_SC_CLK_TCK);
#endif
}

inline double GetSysTime(TimeCount start_tc, TimeCount end_tc) {
#ifdef WIN32
    return (double)(end_tc.system_time - start_tc.system_time) / 10000000;
#else
    return (double)(end_tc.system_time - start_tc.system_time) / sysconf(_SC_CLK_TCK);
#endif
}
