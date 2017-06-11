#ifndef MINLOG_H
#define MINLOG_H

// NOTE: 
// Set level with, e.g.,
//
//    #define MINLOG_LEVEL MINLOG_LEVEL_INFO
//

#define MINLOG_LEVEL_DEBUG 0
#define MINLOG_LEVEL_INFO 1
#define MINLOG_LEVEL_WARN 2
#define MINLOG_LEVEL_FATAL 3
#define MINLOG_LEVEL_OFF 999 

#ifndef MINLOG_LEVEL
#define MINLOG_LEVEL MINLOG_LEVEL_OFF
#endif

namespace minlog
{
  extern void debug(const char *msg, ...);
  extern void info(const char *msg, ...);
  extern void warn(const char *msg, ...);
  extern void fatal(const char *msg, ...);
}

#endif
