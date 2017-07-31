#pragma once

typedef void (*argv_handler) (const char*);

void argv_finish (char** argv, argv_handler handle);
void argv_option (char** argv, char* option, argv_handler handle);
