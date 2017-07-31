#pragma once

#include <unistd.h>  // for pid_t, socklen_t
struct addrinfo;     // lines 6-6

typedef int socktype;

int socket_client (const char* addr, const char* port, int socktype);
int socket_create (struct addrinfo* addrinfo);
int socket_server (const char* addr, const char* port, int socktype);
pid_t socket_fork_pass (int sockfd, char** arguments);
struct addrinfo* socket_lookup (const char* addr, const char* port, socktype type);
void socket_bind (int sockfd, struct addrinfo* addr);
void socket_connect (int sockfd, struct addrinfo* addr);
void socket_listen (int sockfd);
void socket_option (int sockfd, int level, int optname, void* optval, socklen_t optlen);
void socket_pass (int sockfd, char** arguments);
