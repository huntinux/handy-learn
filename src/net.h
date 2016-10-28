#pragma once 

#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h> 		
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <functional>
#include <memory>

#define INVALID_SOCKET (-1)
#define TCP_NODELAY 0x0001

static void printf_address(int fd, struct sockaddr *in_addr, socklen_t in_len, const char *msg = "")
{
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	if (getnameinfo(in_addr, in_len, hbuf, sizeof hbuf, sbuf, sizeof sbuf, NI_NUMERICHOST | NI_NUMERICSERV) == 0)
	{
		printf("[%8d] %s:  (host=%s, port=%s)\n", fd, msg, hbuf, sbuf);
	}
}
static bool make_socket_nonblock(int sfd)
{
	int flags;
	flags = fcntl(sfd, F_GETFL, 0);
	if (flags == -1)
	{
        perror("fcntl getfl");
		return false;
	}
	flags |= O_NONBLOCK;
	if (fcntl(sfd, F_SETFL, flags) == -1)
	{
        perror("fcntl setfl");
		return false;
	}
	flags = 1;
	if (setsockopt(sfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flags, sizeof(int)) < 0)
	{
        perror("setcockopt TCP_NODELAY");
	}
	return true;
}

static int create_and_bind(const char *port, const char *address=NULL)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s;
	int sfd;

	memset(&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
	hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
	hints.ai_flags = AI_PASSIVE;     /* All interfaces */

	s = getaddrinfo(address, port, &hints, &result);
	if (s != 0)
	{
		return INVALID_SOCKET;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == INVALID_SOCKET)
			continue;
		int enable = 1;
		if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&enable, sizeof(int)) < 0)
		{
            perror("setcockopt SOCKET REUSEADDR");
		}

		s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0)
		{
			/* We managed to bind successfully! */
			printf_address(sfd, rp->ai_addr, rp->ai_addrlen, "Listen on");
			break;
		}
		close(sfd);
	}

	if (rp == NULL)
	{
		sfd = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	return sfd;
}
