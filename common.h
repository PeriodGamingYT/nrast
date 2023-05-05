#ifndef __COMMON_H
#define __COMMON_H
// #define COMMON_DEBUG_MODE
// #define COMMON_DEBUG_ALONE
#define COMMON_FILL_TRI
#define COMMON_SPEED_AMOUNT 0.01
#define COMMON_Z_FAR 100.0
#define COMMON_Z_NEAR 1.0
#define COMMON_SHOW_DEPTH_BUFFER
// doesn't have COMMON_ naming convention to it because when debugging.
// it's so commonly used it would be unneeded to have COMMON_ at the start of it.
#define HIT(x) \
	printf("hit%s\n", #x);

#endif
