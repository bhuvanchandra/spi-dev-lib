/*	CONFIGURATION BITS FOR THE MCP3202
|---------------------------------------------|
|		| Config Bits | Channel | GND |
|		|  SGL/| ODD/ |Selection|     |   
| Single Ended	|  DIFF| SIGN |		|     |	
|    Mode       |-------------|---------|-----|	
|    		|   1  | 0    | +    -- |  -  |
|		|   1  | 1    | --   +  |  -  |
|---------------|------|------|---------|-----| 
|    Pseudo-    |   0  | 0    | IN+  IN-|     | 
| Differential	|   0  | 1    | IN-  IN+|     |
|---------------|-------------|---------|-----|*/

#ifndef MCP3202_ADC_H
#define	MCP3202_ADC_H

#define	START_BIT	0x01
#define DNT_CARE_BYTE	0x0

#define SGL_MODE	(0x1 << 7)		
#define DIFF_MODE	(0x0 << 7)
#define CN0		(0x1 << 6)
#define	CN1		(0x0 << 6)
#define MSBF		(0x1 << 5)
#define LSBF		(0x0 << 5)
#define BIT_MASK	0xfff

/* MODES */
#define ADC_CONFIG_SGL_MODE_MSBF_CN0	(SGL_MODE | CN0 | MSBF)
#define ADC_CONFIG_SGL_MODE_MSBF_CN1	(SGL_MODE | CN1 | MSBF)
#define ADC_CONFIG_DIFF_MODE_MSBF_CN0	(DIFF_MODE | CN0 | MSBF)
#define ADC_CONFIG_DIFF_MODE_MSBF_CN1	(DIFF_MODE | CN1 | MSBF)

#define ADC_CONFIG_SGL_MODE_LSBF_CN0	(SGL_MODE | CN0 | LSBF)
#define ADC_CONFIG_SGL_MODE_LSBF_CN1	(SGL_MODE | CN1 | LSBF)
#define ADC_CONFIG_DIFF_MODE_LSBF_CN0	(DIFF_MODE | CN0 | LSBF)
#define ADC_CONFIG_DIFF_MODE_LSBF_CN1	(DIFF_MODE | CN1 | LSBF)

#endif /* MCP3202_ADC_H */
