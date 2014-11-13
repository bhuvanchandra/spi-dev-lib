#include <fcntl.h>                  //for open()
#include <linux/spi/spidev.h>       //for all spi control stuff
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>                 //for exit()
#include <sys/ioctl.h>              //for ioctl()
#include <unistd.h>                 //for close()
#include <getopt.h>
#include "getcontents.h"            //for getContents()
#include "macros.h"



//---------variables for struct spi_ioc_transfer-------//
uint8_t mode=0;                     //8 Byte unsigned Int, only supports mode 1 and 3
uint8_t bits = 8;                   //8 Bits = 1Byte
uint32_t speed = 1000000;           //32 Byte unsigned Int, maxSpeed = 1MHz
uint16_t delay=0;                   //16 Byte unsigned Int
static const char *device = "/dev/spidev3.0";

static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
	     "  -s --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word \n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 's':
			speed = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'b':
			bits = atoi(optarg);
			break;
		case 'l':
			mode |= SPI_LOOP;
			break;
		case 'H':
			mode |= SPI_CPHA;
			break;
		case 'O':
			mode |= SPI_CPOL;
			break;
		case 'L':
			mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			mode |= SPI_CS_HIGH;
			break;
		case '3':
			mode |= SPI_3WIRE;
			break;
		case 'N':
			mode |= SPI_NO_CS;
			break;
		case 'R':
			mode |= SPI_READY;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}



int main(int argc, char **argv)
{
 	
	int retVal = 0;
    	int fd;
    	uint8_t address = 0xFF;
        
        parse_opts(argc, argv);
	

//----------------open file-------------------------//
    fd = open(device, O_RDWR);                             //open file spidev3.0
    if (fd < 0)                                                 //if file can't be opened
        printf("can't open device");


//--------------set SPI Mode------------------------//
    retVal = ioctl(fd, SPI_IOC_WR_MODE, &mode);                 //set mode via I/O-Control, ioctrl(fileDescriptor, Action, *value)
    if (retVal == -1)                                           //if file can't be opened
        printf("can't set spi mode");
//--------check if SPI Mode was set successfully------//
    retVal = ioctl(fd, SPI_IOC_RD_MODE, &mode);                 //get mode via I/O-Control, ioctrl(fileDescriptor, Action, *value)
    if (retVal == -1)                                           //if file can't be read
        printf("can't get spi mode");


//----------------set Bits per word-------------------//
    retVal = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);        //set bits per word via I/O-Control, ioctrl(fileDescriptor, Action, *value)
    if (retVal == -1)                                           //if file can't be opened
        printf("can't set bits per word");
//------check if Bits per word was set successfully----//
    retVal = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);        //get bits per word via I/O-Control, ioctrl(fileDescriptor, Action, *value)
    if (retVal == -1)                                           //if file can't be read
        printf("can't get bits per word");


//--------------------set max speed---------------------//
    retVal = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);        //set max speed via I/O-Control, ioctrl(fileDescriptor, Action, *value)
    if (retVal == -1)                                           //if file can't be opened
        printf("can't set max speed hz");
//--------check if max speed was set successfully-------//
    retVal = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);        //get max speed via I/O-Control, ioctrl(fileDescriptor, Action, *value)
    if (retVal == -1)                                           //if file can't be read
        printf("can't get max speed hz");


//-------------print the set configurations----------------//
    printf("SPI mode: %d\n", mode);                             //print SPI mode
    printf("bits per word: %d\n", bits);                        //print bits per word
    printf("max speed: %d Hz (%d kHz)\n", speed, speed/1000);   //print max speed

    getContents(fd, address);                                   //function call to read a register

    close(fd);                                                  //close file spidev

    return retVal;               
	
}

 	


