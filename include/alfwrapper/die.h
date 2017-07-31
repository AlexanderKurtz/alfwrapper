#pragma once

#include <stddef.h>  // for size_t

void die (const char* format, ...) __attribute__ ((format (printf, 1, 2)));
