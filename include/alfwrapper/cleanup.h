#pragma once

#define _cleanup_(x) __attribute__((cleanup(x)))
#define _cleanup_free_ _cleanup_(cleanup_free)

void cleanup_free (void* p);
