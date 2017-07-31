#pragma once

#include <stdbool.h>  // for bool
#include <stddef.h>   // for size_t

bool string_equal (const char* a, const char* b);
bool string_equal_prefix (const char* a, const char* b, size_t n);
char* string_duplicate (const char *s);
