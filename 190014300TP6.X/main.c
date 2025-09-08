/*
;------------------------------------------------------------------------------;
;                                TestPoint 6                                   ;
;                    Aluno: Herbert Tavares de Oliveira Silva                  ;
;         Matricula: 190014300, UnB-FGA Eletrônica Embarcada 2024.2            ;
;------------------------------------------------------------------------------;
 */

#include "mcc_generated_files/mcc.h"

uint16_t PWMv = 0;
uint16_t ADCv = 0;
uint16_t ADCvp = 0;
uint16_t vmax = 1;
float vmed = 1;

void PWMs(void){
    // Atualizar o estado PWM
    if(PWMv < ADCvp){                // Se pulso está defasado
        PWMv = PWMv + 2;            // Incrementa o pulso
    }else if(PWMv > ADCvp){          // Se o pulso está adiantado
        PWMv = PWMv - 2;}           // Decrementa o pulso
    
    // Limitadores do estado PWM
    if(ADCvp == PWMv + 2 ||ADCvp == PWMv - 2){
    if(ADCvp < 2){PWMv = ADCvp;}     // Limite inferior PWM
    if(ADCvp > 1022){PWMv = 1023;} // Limite superior PWM
    }
    
    // Se a diferença entre ADC e PWM for apenas 1 unidade
    if(ADCvp == PWMv + 1 || ADCvp == PWMv - 1){PWMv = ADCvp;}

}
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
     
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    TMR1_SetInterruptHandler(PWMs);         // A cada interrupção vai para PWMs
    while (1)
    {
        ADCv = ADC_GetConversion(1);
        // Atualiza valor de correção
        if(ADCv > vmax){
            vmax = ADCv;
            // Coeficiente 
            vmed = (float) 1023/vmax;}
        ADCvp = (uint16_t) (vmed * ADCv);
        EPWM2_LoadDutyValue(PWMv); // Carrega atualização do pulso
    }
}