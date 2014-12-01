/*
 * mcp4921_dac.c
 * This file is part of spi-demo
 *
 * Copyright (C) 2014 - BhuvanChandra.DV
 *
 * spi-demo is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * spi-demo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with spi-demo. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include "spi-dev-lib.h"
#include "mcp4921-dac.h"

/******************************************************************************/
int setDac(spiData *data){
	int retVal = 0;
	
	data->privData[0] = (data->privData[0] & 0xFFF);
	data->tx[0] = (CONFIG_BITS | ((data->privData[0] & 0xF00) >> 8));
	data->tx[1] = (data->privData[0] & 0x0FF);
	retVal = spiTransfer(data);
	
	return retVal;
}

/******************************************************************************/
int main(int argc, char **argv){
	int retVal = 0;
	float Vout = 0.0f;
	spiData *data = malloc(sizeof(spiData));

	if(argc < 3){
		printf("Useage: %s <spi_dev_name>\ne.g: mcp3202 /dev/spidev1.0\n", argv[0]);
		exit(-1);
	}
	
	data->mode = 0;                     
	data->bits = 8;                   
	data->speed = 1000000;           
	data->delay = 0;
	sprintf(data->device, "%s", argv[1]);

	if(spiInit(data)){
		perror("SPI Init failed");
		exit(-1);
	}
	
	printf("Enter the Voltage (0V-3.3V): ");
	scanf("%f", &Vout);
	if(Vout > VREF || Vout < 0){
		printf("Voltage range: (0V-3.3V)\n");
		return -1;
	}
	else{
		data->privData[0] = DIN(Vout);
		setDac(data);		
	}

	free(data);
	close(data->fileDescriptor);

	return retVal;               
}
