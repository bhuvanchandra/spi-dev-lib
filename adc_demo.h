/*
 * path_macros.h
 *
 *  Created on: 06-Nov-2013
 *      Author:
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __ADCDEMO_H__
#define __ADCDEMO_H__

// Potemtiometer/analog sources (Vmax should be 3.3v) via X14 pin 1 on evaluation board, via XXX pin XX on Iris board
// Potemtiometer/analog sources (Vmax should be 3.3v) via X14 pin 2 on evaluation board, via XXX pin XX on Iris board

#ifdef __ARM_EABI__    //Path when executing on target
	#define T20_ADC0		"/sys/bus/platform/devices/wm97xx-battery/power_supply/colibri_t20-analog_inputs/voltage_now"
	#define T20_ADC1		"/sys/bus/platform/devices/wm97xx-battery/power_supply/colibri_t20-analog_inputs/temp"

#else                  //Path when debugging on host
	#define T20_ADC0       	"/tmp/temp"
	#define T20_ADC1	"/tmp/voltage_now"
#endif

//Processor
//BogoMIPS
//Features
//CPU implementer
//CPU architecture
//CPU variant
//CPU part
//CPU revision
//Hardware
//Revision
//Serial

char *proc_info(char *detail)
{

	FILE *f = fopen ("/proc/cpuinfo", "r");

	char *a = malloc (256);
	char *b = malloc (128);
	char *c = malloc (128);

	while (!feof (f))
	{
		fgets (a, 512, f);

		if (strlen (a) == 1) continue;

		sscanf (a, "%[^\t:] : %[^\t\n]", b, c);

		if(strstr(b, detail))
		{
			return c;
		}
	}

	fclose (f);
	free(a);
	free(b);
	free(c);
	return "Unknown module";
}

unsigned long aux_adc_read(const char *pathRelayValue)
{
	FILE *fdValue;
	unsigned long value;

	fdValue = fopen(pathRelayValue, "r");
	if(fdValue < 0)
	{
		perror("Open failed");
	}
	fscanf(fdValue, "%ld", &value);
	fclose(fdValue);
	return value;
}

#endif /* ADCDEMO_H_ */
