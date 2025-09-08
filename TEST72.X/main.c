/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1827
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

/*
                         Main application
 */
uint16_t Vpwm;                      
uint16_t pulse = 0;
uint16_t Vcap;
uint16_t Vrpm;
float    Const = 0.000016;

void setPWM(void){
    if(Vpwm < 3){ pulse = Vpwm;}    // Se v atinge um limite inferior
                                    // ent�o pulse recebe v
    
    if(pulse < Vpwm){               // Verifica se o sinal atual de PWM
                                    // � igual ao lido pelo ADC
        
        pulse = pulse + 1;          // Incrementa o pulso
        
    }else if(pulse > Vpwm){         // Verifica se o sinal atual de PWM 
                                    // � igual ao lido pelo ADC
        
        pulse = pulse - 1;          // Decrementa o pulso
        
    }else{                          // Verifica se pulse tem o mesmo valor de v
        
        pulse = Vpwm ;              // ent�o permanece com o valor
    }
    
    if(Vpwm > 1022){ pulse = 1023;} // Se o v atinge o limite superior ent�o 
                                    // pulse recebe o valor maximo
}
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
    
    //TMR1_SetInterruptHandler(setPWM);             // Implementa que a cada 
                                                    // interrup��o o TMR1 
                                                    // atualiza o PWM
    
    while (1)
    {
        Vpwm = (1.5) * ADC_GetConversion(1); // Pega valor de ADC p/ PWM
        
        setPWM();                                   // Atualiza a velocidade do motor
        
        EPWM2_LoadDutyValue(pulse);                 // Carrega atualiza��o do pulso
        
        if(ECCP1_IsCapturedDataReady()){            // Verifica se teve captura
            
            TMR1_WriteTimer(0);                     // Zera o valor de TMR1
            
            Vcap = ECCP1_CaptureRead();             // L� o valor de captura
            
            Vrpm = 60 /((Const) * Vcap); // Rota��es por segundos a cada
                                                    // 60 segundos = rota��es por
                                                    // minuto
            
            PIR1bits.CCP1IF = 0;                    // Desmarca a interrup��o  
                                                    // assim n�o entra dnv
        }
    }
}
/**
 End of File
*/