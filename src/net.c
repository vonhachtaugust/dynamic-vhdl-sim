/*
 * net.c
 *
 *  Created on: 24 Oct 2022
 *      Author: developer
 */
#include <unistd.h>
#include "net.h"

void put_integer_value_to_net(vpiHandle net, int value)
{
	if (!net)
		return;
	s_vpi_value val;
	val.format = vpiIntVal;
	val.value.integer = value;
	vpi_put_value(net, &val, NULL, vpiNoDelay);
}


int get_integer_value_from_net(vpiHandle net)
{
	int ret = 0, i, w = vpi_get(vpiSize, net);
	s_vpi_value val;
	val.format = vpiBinStrVal;
	vpi_get_value(net, &val);
	for (i = 0; i < w; i++) {
		ret <<= 1;
		if (val.value.str[i] == '1')
			ret |= 1;
	}

	return ret;
}
