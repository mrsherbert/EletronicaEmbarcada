/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


#define RXINI '$'      ///< Inicio do quadro de comunicação.
#define RXEND 0x0D          // Fim do quadro de comunicação.
#define RX_PWM 0x81           // Comando PWM
#define RX_ERROR 0x84         // Comando erro
#define RX_SWITCH 0x80        // Comando para ativar/desativar
#define RX_GAINl 0x82         // Comando ganho da luz
#define RX_GAINt 0x83         // Comando ganho da luz
#define SIZECMD 3           // Comprimento da mensagem PWM

// RECEPÇÃO DE DADOS
union{
    uint16_t Rtx;
    struct{
        uint8_t vLx;
        uint8_t vHx;
    };
}RXdados;


// TRANSMISSÃO DE DADOS
union{
    uint16_t Tx1;
    struct{
        uint8_t vL1;
        uint8_t vH1;
    };
}TX1dados;

union{
    uint16_t Tx2;
    struct{
        uint8_t vL2;
        uint8_t vH2;
    };
}TX2dados;

uint8_t     ByteRx[SIZECMD];    // Bytes recebidos
uint8_t     readrx;
uint8_t     countRx = 0;        // Contador de bits é ponteiro
uint16_t    vPWM;               // Valor de PWM
uint16_t    vGAINl = 1;      // Valor do ganho da luz
uint16_t    vGAINt = 1;      // Valor do ganho da temperatura
int         vAUX = 0;           // Auxiliar 1
bool        swit = true;

/**
 * Realiza o tratamento dos dados para transmissão aos perifericos
 */
void TRUSART();

/**
 * Seleciona os comandos recebidos para realização de funções
 */
void selecaoRX();

/**
 * Desloca os bits que são recebidos para compreensão dos embarcados
 */
void desloca();


#endif	/* XC_HEADER_TEMPLATE_H */
