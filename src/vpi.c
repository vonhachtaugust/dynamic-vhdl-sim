/*
 * vpi.c
 *
 *  Created on: 17 Oct 2022
 *      Author: developer
 */

#include <stdio.h>
#include <unistd.h>

#include <vpi_user.h>

static PLI_INT32 cb_shutdown_server() {
	return 0;
}

static PLI_INT32 cb_startup_server() {
	return 0;
}

void entry_point_cb() {
    s_cb_data cb;

    cb.reason = cbStartOfSimulation;
    cb.cb_rtn = &cb_startup_server;
    cb.user_data = NULL;

    if (vpi_register_cb(&cb) == NULL) {
        vpi_printf("cannot register cbStartOfSimulation callback\n");
    }

    cb.reason = cbEndOfSimulation;
    cb.cb_rtn = &cb_shutdown_server;
    cb.user_data = NULL;

    if (vpi_register_cb(&cb) == NULL) {
            vpi_printf("cannot register cbEndOfSimulation callback\n");
    }
}

// List of entry points called when the plugin is loaded
void (*vlog_startup_routines[]) () = { entry_point_cb, 0};

