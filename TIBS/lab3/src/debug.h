#ifndef DAV_TIBS_3_DEBUG_H
#define DAV_TIBS_3_DEBUG_H

void fatal(const char *, ...);

#ifdef DEBUG
#	define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#	define debug(...)
#endif

#endif

