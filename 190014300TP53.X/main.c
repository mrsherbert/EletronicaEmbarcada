/*
;------------------------------------------------------------------------------;
;                                TestPoint 5.3                                 ;
;                    Aluno: Herbert Tavares de Oliveira Silva                  ;
;         Matricula: 190014300, UnB-FGA Eletrônica Embarcada 2024.2            ;
;------------------------------------------------------------------------------;
 */

#include "mcc_generated_files/mcc.h"
#define vmin 300
#define vmax 900
uint16_t v;

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    while (1)
    {
     v = ADC_GetConversion(1);
     if(v < vmin){IO_RA3_SetHigh();}
     else if(v > vmax){IO_RA3_SetLow();}
    }
}
/**
 End of File
*/