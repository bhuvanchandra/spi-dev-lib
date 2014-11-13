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

#define	START_BIT	0x01

#define SGL_MODE	(1 << 0x7)		
#define DIFF_MODE	(0 << 0x7)
#define CN0		(1 << 0x6)
#define	CN1		(0 << 0x6)
#define MSBF		(1 << 0x5)
#define LSBF		(0 << 0x5)
#define DNT_CARE_BYTE	0x00
#define MSBF_MASK	0x0000FFF
//#define LSBF_MASK	/*TODO*/

/* MODES */
#define ADC_CONFIG_SGL_MODE_MSBF_CN0	(SGL_MODE | CN0 | MSBF)
#define ADC_CONFIG_SGL_MODE_MSBF_CN1	(SGL_MODE | CN1 | MSBF)
#define ADC_CONFIG_DIFF_MODE_MSBF_CN0	(DIFF_MODE | CN0 | MSBF)
#define ADC_CONFIG_DIFF_MODE_MSBF_CN1	(DIFF_MODE | CN1 | MSBF)

#define ADC_CONFIG_SGL_MODE_LSBF_CN0	(SGL_MODE | CN0 | LSBF)
#define ADC_CONFIG_SGL_MODE_LSBF_CN1	(SGL_MODE | CN1 | LSBF)
#define ADC_CONFIG_DIFF_MODE_LSBF_CN0	(DIFF_MODE | CN0 | LSBF)
#define ADC_CONFIG_DIFF_MODE_LSBF_CN1	(DIFF_MODE | CN1 | LSBF)
