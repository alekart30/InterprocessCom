#ifndef CONNECTOR_H
#define CONNECTOR_H

void conn_create();
//0 - failed 1 - succeeded
int conn_isCreated();
int conn_read();
int conn_write();
void conn_destroy();

#endif