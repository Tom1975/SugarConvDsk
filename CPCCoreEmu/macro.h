#pragma once


#ifndef TXT_OUT_PROF
#define TXT_OUT_PROF "Duration"
#endif

#define PROF

#ifdef PROF

#ifdef _WIN32
static __int64 s1, s2, freq;
#else
static int64_t s1, s2, freq;
#endif

static DWORD t;
static char s [1024];

#define START_CHRONO  QueryPerformanceFrequency((LARGE_INTEGER*)&freq);;QueryPerformanceCounter ((LARGE_INTEGER*)&s1);
#define STOP_CHRONO   QueryPerformanceCounter ((LARGE_INTEGER*)&s2);t=(DWORD)(((s2 - s1) * 1000000) / freq);
#define PROF_DISPLAY _stprintf_s(s, 1024, "%s: %d us\n",TXT_OUT_PROF, t);OutputDebugString (s);

#else
   #define START_CHRONO
   #define STOP_CHRONO
   #define PROF_DISPLAY
#endif

#ifdef LOGFDC
   #define LOG(str)  if (log_) log_->WriteLog (str);
   #define LOGEOL if (log_) log_->EndOfLine ();
   #define LOGB(str)  if (log_) log_->WriteLogByte (str);
#else
   #define LOG(str)
   #define LOGB(str)
   #define LOGEOL
#endif
