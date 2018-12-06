#include <string.h>
#include <stdio.h>
#include <sys/mman.h>

#include "connector.h"

#define SIZE 4

void* buffer = NULL;

void conn_create()
{
  buffer = (void*)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
  if (buffer == MAP_FAILED)
    buffer = NULL;
}


int conn_read(void* data, size_t size)
{
  if(size > SIZE)
    return 0;
  memcpy(data, buffer, size);
  return 1;
}

int conn_isCreated()
{
  if(buffer)
    return 1;
  else
    return 0;
}

int conn_write(void* data, size_t size)
{
  if(size > SIZE)
    return 0;
  memcpy(buffer, data, size);
  return 1;
}

void conn_destroy()
{
  munmap(buffer, SIZE);
}

