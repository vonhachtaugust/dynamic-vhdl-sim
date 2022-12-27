/*
 * net.h
 *
 *  Created on: 24 Oct 2022
 *      Author: developer
 */

#ifndef VPI_NET_H_
#define VPI_NET_H_

#include <vpi_user.h>

void put_integer_value_to_net(vpiHandle net, int value);
int get_integer_value_from_net(vpiHandle net);


#endif /* VPI_NET_H_ */
