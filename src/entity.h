/*
 * dut.h
 *
 *  Created on: 17 Oct 2022
 *      Author: developer
 */

#ifndef VPI_ENTITY_H_
#define VPI_ENTITY_H_

#include <vpi_user.h>

typedef struct {
	const char* name;
	vpiHandle clk;
	vpiHandle rst;
} entity_t;

void scan_dut(entity_t* dut);


#endif /* VPI_ENTITY_H_ */
