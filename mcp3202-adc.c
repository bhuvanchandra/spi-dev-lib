/*
 * mcp3202-adc.c
 * This file is part of spi_adc_demo
 *
 * Copyright (C) 2014 - BhuvanChandra.DV
 *
 * spi_adc_demo is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * spi_adc_demo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with spi_adc_demo. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include "spi-dev-lib.h"
#include "mcp3202-adc.h"

/******************************************************************************/
int readAdc(spiData *data){
	unsigned int reData = 0;           

	data->tx[0] = START_BIT;
	if(data->privData[0] < 0 || data->privData[0] > 1){
		printf("MCP3202 had two channels, CHN0, CHN1\n");
		return -1;
	}
	switch(data->privData[0]){
		case 0:
			data->tx[1] = ADC_CONFIG_SGL_MODE_MSBF_CN0;
		break;

		case 1:
			data->tx[1] = ADC_CONFIG_SGL_MODE_MSBF_CN1;
		break;
		
		default:
			data->tx[1] = ADC_CONFIG_SGL_MODE_MSBF_CN0;
		break;
	}
	data->tx[2] = DNT_CARE_BYTE;

	spiTransfer(data);

	printf("The analog input value is \n");
	reData = (((data->rx[1] << 8) + data->rx[2]) & BIT_MASK);
	
	printf("Value at MCP3202 CH%d is: %d D : %X H \n", data->privData[0], reData, reData);
	
	return 0;
}

/******************************************************************************/
int main(int argc, char **argv){
	int retVal = 0;
	spiData *data = malloc(sizeof(spiData));

	if(argc < 3){
		printf("Useage: %s <adc_channel> <spi_dev_name>\ne.g: mcp3202 0 /dev/spidev1.0\n", argv[0]);
		exit(-1);
	}
	
	data->mode = 0;                     
	data->bits = 8;                   
	data->speed = 1000000;           
	data->delay = 0;
	sprintf(data->device, "%s", argv[2]);

	data->privData[0] = atoi(argv[1]);	/* ADC Channel No. */

	if(spiInit(data)){
		perror("SPI Init failed");
		exit(-1);
	}
	
	retVal = readAdc(data);
	if(retVal < 0){
		perror("Failed to read ADC");
	}

	free(data);
	close(data->fileDescriptor);

	return retVal;               
}
