#include <linux/spi/spidev.h>   //for all spi control stuff
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>          //for ioctl()
#include <errno.h>
#include <fcntl.h>              //for open()
#include "getcontents.h"        //for getContents()
#include "macros.h"


//------------declaration of variables------------//
extern uint8_t mode;                                    //8 Byte unsigned Int
extern uint8_t bits;                                    //8 Bits unsigned Int
extern uint32_t speed;                                  //32 Byte unsigned Int, maxSpeed = 1MHz
extern uint16_t delay;                                  //16 Byte unsigned Int

//******************************************************************************
void getContents(int fd, uint8_t address)
{
//---------Init of all needed data storage------------//
    int ret;
    uint8_t tx[] = {0x01, 0xE0, 0x00};               
    uint8_t rx[ARRAY_SIZE(tx)] = {0};
    unsigned int data = 0;                 
    printf("after initialisation of array rx:");        
   for (ret = 0; ret < ARRAY_SIZE(tx); ret++)          
    {
   	printf("%.2X ", rx[ret]);
    }
    
//----------Prepare Init of CAN Controller------------//
    struct spi_ioc_transfer tr =                        //This structure is mapped directly to the kernel spi_transfer structure.
    {
        .tx_buf = (unsigned long)tx,                    //Holds pointer to userspace buffer with transmit data, or null. No data -> zeroes
        .rx_buf = (unsigned long)rx,                    //Holds pointer to userspace buffer for receive data, or null. No data -> zeroes
        .len = ARRAY_SIZE(tx),                          //Length of tx0 and rx buffers, in bytes.
        .delay_usecs = delay,                           //delay after the last bit transfer before optionally deselecting the device before the next transfer.
        .speed_hz = speed,                              //Temporary override of the device's bitrate.
        .bits_per_word = bits,                          //Temporary override of the device's wordsize.
    };

//-------------------send command---------------------//
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);          
    if (ret < 1)                                        
        printf("can't send SPI message");

//------------output of register contents-------------//
    printf("\nafter read of kernel buffer, rx:");       
    for (ret = 0; ret < ARRAY_SIZE(tx); ret++)          
    {
     	  
        printf("%.2X ", rx[ret]);
        
    }
   
 printf("\nThe analog input value is \n");
 data = (((rx[1]<<8)+rx[2]) & 0x0000FFF);
 printf("rx[1]<<8 %d \n",(rx[1]<<8));
 printf("rx[2] %d \n",rx[2]);
 printf("rx[1]<<8)+rx[2] %d \n", ((rx[1]<<8)+rx[2]));
 printf("(((rx[1]<<8)+rx[2]) & 0x0000FFF) %d \n", (((rx[1]<<8)+rx[2]) & 0x0000FFF));
 printf("The input value is %d:%X \n", data, data);
 printf("The digital value of the analog input is %X \n", data);
    
}
