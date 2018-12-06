#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "connector.h"

#define SIZE 4
#define FILE_NAME "MY_FILE"

void* buffer = NULL;
int f = 0;

void conn_create()
{
  f = shm_open(FILE_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
  if (f == -1)
    return;
  if (ftruncate(f, SIZE) == -1)
    return;
  buffer = (void*)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
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


int conn_write(void* data, size_t size)
{
  if(size > SIZE)
    return 0;
  memcpy(buffer, data, size);
  return 1;
}

int conn_isCreated()
{
  if(buffer)
    return 1;
  else
    return 0;
}

void conn_destroy()
{
  munmap(buffer, SIZE);
  shm_unlink(FILE_NAME);
}

