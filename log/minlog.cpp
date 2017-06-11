#include "minlog.h"
#include <stdio.h>
#include <stdarg.h>

void minlog::debug(const char *fmt, ...)
{
    va_list arg;
    if (MINLOG_LEVEL > MINLOG_LEVEL_DEBUG) return;
    va_start(arg, fmt);
    vfprintf(stderr, fmt, arg);
    va_end(arg);
}

void minlog::info(const char *fmt, ...)
{
    va_list arg;
    if (MINLOG_LEVEL > MINLOG_LEVEL_INFO) return;
    va_start(arg, fmt);
    vfprintf(stderr, fmt, arg);
    va_end(arg);
}

void minlog::warn(const char *fmt, ...)
{
    va_list arg;
    if (MINLOG_LEVEL > MINLOG_LEVEL_WARN) return;
    va_start(arg, fmt);
    vfprintf(stderr, fmt, arg);
    va_end(arg);
}

void minlog::fatal(const char *fmt, ...)
{
    va_list arg;
    if (MINLOG_LEVEL > MINLOG_LEVEL_FATAL) return;
    va_start(arg, fmt);
    vfprintf(stderr, fmt, arg);
    va_end(arg);
}
