/***************************************************************************//**
 * @file debug_printf.c
 * @brief Prints hello world through the ITM module on SWO-pin.
 * @version 5.2.2
 *******************************************************************************
 * # License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include <stdio.h>
#include "em_device.h"

#define ITM_Port32(n) (*((volatile unsigned int *)(0xE0000000+4*n)))

/* Need to implement the two Retarget IO functions with the read/write functions we want to use. */
int RETARGET_WriteChar(char c){
	return ITM_SendChar (c);
}

int RETARGET_ReadChar(void){
	return 0;
}

void setupSWOForPrint(void)
{
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

/**************************************************************************//**
 * @brief  Main function
 *         The main function demonstrates two ITM trace features;
 *         First printf is utilized to print strings to trace channel 0.
 *         Then a while loop demonstrates how to send data to another
 *         ITM channel, specifically channel 1.
 *****************************************************************************/
int main(void)
{
	int i;

	/* Configures the SWO to output both printf-information, PC-samples and interrupt trace. */
	setupSWOForPrint();

	printf("Hello world!\n");
	ITM_SendChar('\n');

	/* This while loop increments i and sends the variable to ITM trace channel 1, notice that printf used ITM channel 0. */
	while(1){
		/* First we need to wait until the ITM buffer is ready to accept new data. */
		while (ITM_Port32(0) == 0);
		/* Use the define at the top of this file to write to the correct ITM-port address. */
		ITM_Port32(1) = i++;

		printf("Hello world!\n");
	}
}
