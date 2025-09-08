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
#include "main.h"
#include "math.h"

/*
                         Main application
 */
// Funcao aproveitada do Exercicio32
void initMatrix(){
    uint8_t data[4];                                                            // Buffer para tx spi
    uint8_t k=0;                                                                // Ponteiro do arranjo da configuração das matrizes
    
    for(uint8_t i=0;i<6;i++){                                                   // Envia os 8 valores de configuração
        for(uint8_t j=0;j<4;j=j+2){
            data[j]= matrix_conf[k];                                            // Define o endereço dos registradores de configuração
            data[j+1]= matrix_conf[k+1];                                        // Define o valor dos registradores de configuração
        }
        k=k+2;                                                                  // Inc ponteiro da configuração     
        txSpi(spiCS1, data, 4);                                                 // Tx configuração para as 2 matrizes
        if(i==4){                                                               // Display-Test
            __delay_ms(800);
        }
    }
}

void AtualizaXY(void){
    //DESLIGAR O LED
    // Atualiza posicao em X
    if(posicaoX == 0 || posicaoX == 15){
        incX = -incX;
        //LIGAR O LED
    }
    posicaoX = posicaoX + incX;
    
    // Atualiza posicao em Y
    static guarda_posicaoY;
    guarda_posicaoY = guarda_posicaoY + incY;
    
    
    if(guarda_posicaoY <= 0){
        incY = -incY;
        guarda_posicaoY = 0;
    }
    if(guarda_posicaoY >= 7){
        incY = -incY;
        guarda_posicaoY = 7;
    }
    posicaoY = (uint8_t)(round(guarda_posicaoY));   
    
    // Faz envio para a matriz de led
    matrixPosicao(posicaoX,posicaoY);
}

void gera_valorX(void){
    guarda_x_i_menos_2 = rand();
    // Lógica para fazer com que guarda_x_i_menos_2 esteja no range especificado
    if(guarda_x_i_menos_2 > 7){
        guarda_x_i_menos_2 = 7;
    }else if(guarda_x_i_menos_2 < 0){
        guarda_x_i_menos_2 = 0;
    }else{
        guarda_x_i_menos_2 = guarda_x_i_menos_2;
    }
    
    guarda_x_i_menos_1 = rand();
    // Lógica para fazer com que guarda_x_i_menos_2 esteja no range especificado
    if(guarda_x_i_menos_1 > 7){
        guarda_x_i_menos_1 = 7;
    }else if(guarda_x_i_menos_1 < 0){
        guarda_x_i_menos_1 = 0;
    }else{
        guarda_x_i_menos_1 = guarda_x_i_menos_1;
    }
}

void gera_valorY(void){
    posicaoY = rand();
    // Lógica para fazer com que posicao.Y esteja no range especificado
    if(posicaoY > 7){
        posicaoY = 7;
    }else if(posicaoY < 0){
        posicaoY = 0;
    }else{
        posicaoY = posicaoY;
    }
    
    guarda_y_i_menos_2 = rand();
    // Lógica para fazer com que guarda_y_i_menos_2 esteja no range especificado
    if(guarda_y_i_menos_2 > 7){
        guarda_y_i_menos_2 = 7;
    }else if(guarda_y_i_menos_2 < 0){
        guarda_y_i_menos_2 = 0;
    }else{
        guarda_y_i_menos_2 = guarda_y_i_menos_2;
    }
    
    guarda_y_i_menos_1 = rand();
    // Lógica para fazer com que guarda_y_i_menos_1 esteja no range especificado
    if(guarda_y_i_menos_1 > 7){
        guarda_y_i_menos_1 = 7;
    }else if(guarda_y_i_menos_1 < 0){
        guarda_y_i_menos_1 = 0;
    }else{
        guarda_y_i_menos_1 = guarda_y_i_menos_1;
    }
}

void reinicia(void){
    T = (uint8_t)(rand() & 0x0F);
    float a;
    //garante que 'a' tah no range especificado
    a = -1.6 + (rand()&0x1F)*0.1;
    incY = a;
    // Desabilita pra nao entrar de novo nessa funcao
    gera_xs_anteriores = true;
    gera_ys_anteriores = true;
}

void txSpi(spiCS_t cs, uint8_t *data, size_t dataSize){
    switch(cs){                                                                 // Ativa CS do dispositivo
        case spiCS1:
            CS1_SetLow();                                                       // Ativa CS1
            break;
        case spiCS2:
            CS2_SetLow();                                                       // Ativa CS2
            break;
    }
    SPI1_ExchangeBlock(data,dataSize);                                          // Tx
    switch(cs){
        case spiCS1:
            CS1_SetHigh();                                                      // Desativa CS1
            break;
        case spiCS2:
            CS2_SetHigh();                                                      // Desativa CS2
            break;
    }
    //__delay_us(1);
}

void matrixPosicao(uint8_t posicaoX, uint8_t posicaoY){
    uint8_t data[4];
    data[1] = 0;
    data[3] = 0;                         // Buffer para tx spi 
    for(uint8_t i = 0; i < 8; i++){
        data[0] = i;
        data[2] = i;
    }  
    if(posicaoX < 8){
        data[0] = posicaoX;
        data[1] = 1 << posicaoY;
    }else{
        data[2] =  posicaoX - 7;
        data[3] = 1 << posicaoY;
    }
        
        
  /*  
#if flip_matrix                                                                 // No Lab Remoto a imagem aparece invertida na horizontal
    for(uint8_t i=8;i>0;i--){                                                   // Endereça digitos 7..0
#else
    for(uint8_t i=1;i<9;i++){                                                   // Endereça digitos 0..7
#endif
    */                                                                // Valor do digito i da Matriz Direita
    txSpi(spiCS1, data, 4);                                                 // Tx valores dos dígitos dig para as matrizes
}

void verificaT(){
    static conta_ate_T = 0;  
    if(recalcula){
        reinicia();
        recalcula = false;    
    }
   if(conta_ate_T == T){
        AtualizaXY();
        conta_ate_T = 0;           
    }
    conta_ate_T++;    
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    // Conta durante ate multiplo de 15 de 25 ms   
    TMR1_SetInterruptHandler(verificaT);
    CS1_SetHigh();
    CS2_SetHigh();
    
    SPI1_Open(SPI1_DEFAULT);        // Configura MSSP1
    
    initMatrix();                   // Configura matrizes
    __delay_ms(1000);
        
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
   
    while (1)
    {   
        if(EUSART_is_rx_ready()){                                               // Se chega um byte 
            uint8_t rxChar = EUSART_Read();                                     // Se guarda em rxChar
            if(rxChar == 0x55){                                                 // Se for o inicio do quadro    
                recalcula = true ;                                              // zera contador
            }
        }
    }
}    
/**
 End of File
*/