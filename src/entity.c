/*
 * dut.c
 *
 *  Created on: 17 Oct 2022
 *      Author: developer
 */
#include "entity.h"

#include <stdio.h>
#include <unistd.h>


void scan_dut(entity_t *dut) {
	vpiHandle iter, top, net;
	const char *net_name;
	int net_width;
	int net_dir;

	iter = vpi_iterate(vpiModule, NULL);
	top = vpi_scan(iter);
	vpi_free_object(iter);

	dut->name = vpi_get_str(vpiName, top);
	printf("VHDL top name: %s", dut->name);

	iter = vpi_iterate(vpiNet, top);
	if (iter) {
		while ((net = vpi_scan(iter))) {
			net_name = vpi_get_str(vpiName, net);
			net_width = vpi_get(vpiSize, net);
			net_dir = vpi_get(vpiDirection, net);
			printf("Net %s[%d] %s\n", net_name, net_width,
					(net_dir == vpiInput) ?
							"Input" :
							((net_dir == vpiOutput) ?
									"Output" :
									((net_dir == vpiInout) ? "Inout" : "")));

			if (net_dir == vpiInput) {
				if (strcmp("clk", net_name) == 0 && net_width == 1) {
					dut->clk = net;
					printf("\e[32mFound input net clk[1]\e[0m\n");
				} else if (strcmp("rst", net_name) == 0 && net_width == 1) {
					dut->rst = net;
					printf("\e[32mFound input net rstn[1]\e[0m\n");
				}
			} else {
				vpi_free_object(net);
			}
		}
	} else {
		vpi_free_object(iter);
	}

	vpi_free_object(top);
}
