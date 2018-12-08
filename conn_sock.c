#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "connector.h"

#define SOCKET_NAME "MY_SOCKET"

int sockets[2];
int created = 0;

void conn_create()
{
	if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) != -1)
		created = 1;
	else
		created = 0;
}

int conn_read(void* data, size_t size)
{
	if(read(sockets[0], data, size) == -1)
		return 0;
	else
		return 1;
}

int conn_isCreated()
{
	if(created != 0)
		return 1;
	else
		return 0;
}

int conn_write(void* data, size_t size)
{
	if(write(sockets[1], data, size) == -1)
		return 0;
	else
		return 1;
}

void conn_destroy()
{
  close(sockets[0]);
  close(sockets[1]);
}

