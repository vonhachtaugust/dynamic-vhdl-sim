/*
 * server.h
 *
 *  Created on: 24 Oct 2022
 *      Author: developer
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <libwebsockets.h>

typedef struct server_st Server;

struct server_st {
    const char* name;
    uint16_t port;
    bool running;
    pthread_t thread_id;
    struct lws_context *context;
    struct lws_context_creation_info info;
};

void run(Server* server, uint16_t port);
void terminate(Server* server);


#endif /* SERVER_H_ */
