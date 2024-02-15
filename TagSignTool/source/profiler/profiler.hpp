#pragma once

#include <time.h>

#define PROFILER_START(name) \
double name##_elapsed_time = 0.0; \
time_t name##_start; \
time_t name##_end; \
time(&name##_start);

#define PROFILER_END(name) \
time(&name##_end); \
name##_elapsed_time = difftime(name##_end, name##_start); \
printf(#name" took %.2lf seconds.\n", name##_elapsed_time);

#define PROFILE_FUNCTION(function, ...) \
PROFILER_START(function) \
function(__VA_ARGS__); \
PROFILER_END(function)

#define PROFILE_FUNCTION2(name, function, ...) \
{ \
  PROFILER_START(name) \
  function(__VA_ARGS__); \
  PROFILER_END(name) \
}

