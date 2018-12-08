#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include "connector.h"


#define FIFO_NAME "MY_FIFO"
int created = 0;
int fdr= 0;
int fdw = 0;

void conn_create()
{
  if(mkfifo(FIFO_NAME, S_IRWXU) == 0)
    created = 1;
  else
    created = 0;
  
  if (!created)
    return;
  fdr = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
  fdw = open(FIFO_NAME, O_WRONLY | O_NONBLOCK);
  if(fdr != -1 && fdw != -1)
    created = 1;
  else
    created = 0;
  }

int conn_read(void* data, size_t size)
{
  if(read(fdr, data, size) == -1)
    return 0;
  else
    return 1;
}

int conn_isCreated()
{
  if(created)
    return 1;
  else
    return 0;
}

int conn_write(void* data, size_t size)
{
  if(write(fdw, data, size) == -1)
    return 0;
  else
    return 1;
}

void conn_destroy()
{
  unlink(FIFO_NAME);
}
