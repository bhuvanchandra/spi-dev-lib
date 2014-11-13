/*
 * spi-dev-lib.c
 * This file is part of spi-dev-lib
 *
 * Copyright (C) 2014 - BhuvanChandra.DV
 *
 * spi-dev-lib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * spi-dev-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with spi-dev-lib. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi-dev-lib.h"

//#define DEBUG

/******************************************************************************/
int spiInit(spiData *data){
	int retVal = 0;

	/* open file */
	data->fileDescriptor = open(data->device, O_RDWR);
	if(data->fileDescriptor < 0){
		perror("Can't open device");
		return retVal;
	}

	/* set SPI Mode */
	retVal = ioctl(data->fileDescriptor, SPI_IOC_WR_MODE, &(data->mode));
	if(retVal < 0){
		perror("Can't set spi mode");
		return retVal;
	}

	/* check if SPI Mode was set successfully */
	retVal = ioctl(data->fileDescriptor, SPI_IOC_RD_MODE, &(data->mode));
	if(retVal < 0){
		perror("Can't get spi mode");
		return retVal;
	}

	/* set Bits per word */
	retVal = ioctl(data->fileDescriptor, SPI_IOC_WR_BITS_PER_WORD, &(data->bits));
	if(retVal < 0){
		perror("Can't set bits per word");
		return retVal;
	}

	/* check if Bits per word was set successfully */
	retVal = ioctl(data->fileDescriptor, SPI_IOC_RD_BITS_PER_WORD, &(data->bits));
	if(retVal < 0){
		perror("Can't get bits per word");
		return retVal;
	}

	/* set max speed */
	retVal = ioctl(data->fileDescriptor, SPI_IOC_WR_MAX_SPEED_HZ, &(data->speed));
	if (retVal < 0){
		perror("Can't set max speed hz");
		return retVal;
	}

	/* check if max speed was set successfully */
	retVal = ioctl(data->fileDescriptor, SPI_IOC_RD_MAX_SPEED_HZ, &(data->speed));
	if(retVal < 0){
		perror("Can't get max speed hz");
		return retVal;
	}
#ifdef DEBUG
	/* print the set configurations */
	printf("SPI mode: %d\n", data->mode);
	printf("Bits per word: %d\n", data->bits);
	printf("Max speed: %d Hz (%d kHz)\n", data->speed, data->speed/1000);
#endif /* DEBUG */
	return 0;
}

/******************************************************************************/
int spiTransfer(spiData *data){
	int ret = 0;

	/* Prepare */
	struct spi_ioc_transfer tr =                       
	{
		.tx_buf = (unsigned long)data->tx,
		.rx_buf = (unsigned long)data->rx,
		.len = ARRAY_SIZE(data->tx),
		.delay_usecs = data->delay,
		.speed_hz = data->speed,
		.bits_per_word = data->bits,
	};
	
	/* Send data */
	ret = ioctl(data->fileDescriptor, SPI_IOC_MESSAGE(1), &tr);
	
	return ret;
}
