#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>           
#include "connector.h"

int files[2];
int created = 0;

void conn_create()
{
	if(pipe(files) != -1)
		created = 1;
	else
		created = 0;
}

int conn_read(void* data, size_t size)
{
	if(read(files[0], data, size) == -1)
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
	if(write(files[1], data, size) == -1)
		return 0;
	else
		return 1;
}

void conn_destroy()
{
  close(files[0]);
  close(files[1]);
}

