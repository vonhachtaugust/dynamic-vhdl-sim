/*
 * server.c
 *
 *  Created on: 24 Oct 2022
 *      Author: developer
 */
#include "server.h"

static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
		// printf("Connection attempted! Callback reason %d\n", (int) reason);
        switch (reason) {
                case LWS_CALLBACK_HTTP:
                        lws_serve_http_file(wsi, "index.html", "text/html", NULL, 0);
                        break;
                default:
                        break;
        }
        return 0;
}

void *thread_main(void *vargp) {
    printf("Running server!\n");
    Server *server = (Server*) vargp;
    while (server->running) {
        lws_service(server->context, 0);
    }
    printf("Server stopped running!\n");
    return NULL;
}

enum protocols {
    PROTOCOL_HTTP = 0,
    PROTOCOL_COUNT
};

static struct lws_protocols protocols[] = {
    /* The first protocol must always be the HTTP handler */
    {
        "http-only", /* name */
        callback_http, /* callback */
        0, /* No per session data */
        0, /* Max frame size / rx buffer */
    },
    { NULL, NULL, 0, 0 } /* terminator */
};

void run(Server* server, uint16_t port) {
    if (server->running) {
        printf("Server already running.\n");
        return;
    }

    printf("Setting up server with name %s on port %d!\n", server->name, port);
    memset(&server->info, 0, sizeof(server->info));

    server->info.port = port;
    server->info.protocols = protocols;
    server->info.gid = -1;
    server->info.uid = -1;

    server->context = lws_create_context(&server->info);

    server->running = true;
    pthread_create(&server->thread_id, NULL, thread_main, (void *) server);
}

void terminate(Server* server) {
    if (!server->running) {
        printf("No server is running!");
        return;
    }

    server->running = false;
    printf("Waiting for thread to join...\n");
    pthread_join(server->thread_id, NULL);
    printf("Thread joined\n");

    lws_context_destroy(server->context);
}

