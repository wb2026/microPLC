#include "stdint.h"
#include "stdlib.h"


volatile uint32_t VectorTable[82] __attribute__((section(".RAMVectorTable")));



void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
//void SVC_Handler(void);
void DebugMon_Handler(void);

void SystemInit(void);



void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void SystemInit(void)
{
	
	
}


int main(void)
{
	uint8_t i=0;  

	//  for(i = 0; i < 48; i++)
	for(i = 0; i < 82; i++)	
	{
		VectorTable[i] = *(volatile uint32_t*)((uint32_t)0x08002800 + (i<<2));
	}

	(*((volatile uint32_t *)0xE000ED08UL)) = (uint32_t)VectorTable;	
	
	
    for( ; ; )
    {
        /* Should not reach here. */
    }	
}








