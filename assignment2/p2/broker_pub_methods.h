#ifndef _BROKER_SUB_METHODS_H
#define _BROKER_SUB_METHODS_H

#include "broker.h"

void handle_topic_create (int fd, char *topic, struct shared_mem_structure *addr);
void handle_msg_recv (int fd, char *topic, char *msg, struct shared_mem_structure *addr);

#endif
