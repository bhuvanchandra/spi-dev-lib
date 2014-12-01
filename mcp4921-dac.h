
#ifndef MCP4921_DAC_H
#define	MCP4921_DAC_H

#define GAIN	1
#define VREF	3.3f
#define RES	12
#define CNST	4096	/* 2^N (N = 12) */
#define DIN(x)	(unsigned int) (x * CNST)/(VREF * GAIN)

#define A_B(x)	(x << 7)
#define BUF(x)	(x << 6)
#define GA(x)	(x << 5)
#define SHDN(x)	(x << 4)

#define CONFIG_BITS (A_B(0) | BUF(1) | GA(1) | SHDN(1))

/******************************************************************************/
int setDac(spiData *data);

#endif /* MCP4921_DAC_H */
