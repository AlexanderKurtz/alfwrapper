#pragma once

void bcc_attach (int sockfd, void* module, const char* function);
void bcc_get (void* module, const char* name, void* key, void* value);
void* bcc_load (const char* file);
void bcc_set (void* module, const char* name, const char* key, const char* value);
