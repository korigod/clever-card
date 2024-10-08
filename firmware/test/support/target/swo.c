#include <stdio.h>
#include "em_chip.h"
#include "em_device.h"

#define ITM_Port32(n) (*((volatile unsigned int *)(0xE0000000 + 4 * n)))

/* Need to implement the two Retarget IO functions with the read/write functions we want to use. */
int RETARGET_WriteChar(char c) {
	return ITM_SendChar (c);
}

int RETARGET_ReadChar(void) {
	return 0;
}

void setupSWOForPrint(void) {
	CHIP_Init();

	/* Enable GPIO clock. */
	CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

	/* Enable Serial wire output pin */
	GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;

	/* Set location 1 */
	GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) |GPIO_ROUTE_SWLOCATION_LOC1;
	/* Enable output on pin PC15 */
	GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
	GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;

	/* Enable debug clock AUXHFRCO */
	CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

	/* Wait until clock is ready */
	while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY));

	/* Enable trace in core debug */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	ITM->LAR  = 0xC5ACCE55;
	ITM->TER  = 0x0;
	ITM->TCR  = 0x0;
	TPI->SPPR = 2;
	TPI->ACPR = 0xf;
	ITM->TPR  = 0x0;
	DWT->CTRL = 0x400003FE;
	ITM->TCR  = 0x0001000D;
	TPI->FFCR = 0x00000100;
	ITM->TER  = 0x1;
}
