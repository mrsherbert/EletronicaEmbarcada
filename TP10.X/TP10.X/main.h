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

#define RX_REIN     0x55    //Palavra de reinicio
#define flip_matrix true    // Se for True inverte horizontalmente a matriz

const uint8_t matrix_conf[] = {
    0x09,0x00,  // Decode mode = 0
    0x0A,0x00,  // Intensity 1/32
    0x0B,0x07,  // Scan Limit
    0x0C,0x01,  // Shutdown mode = 1
    0x0F,0x01,  // Display-Test = 1
    0x0F,0x00,  // Display-Test = 0
};  ///< Configuração da matriz de  LEDs

typedef enum{
    spiCS1,                                                                     // Acessa a matriz de LEDs (MAX7219))
    spiCS2                                                                      // Acessa a memória EEPROM (25LC512))
}spiCS_t;                                                                       // CS do dispositivo a ser acessado 

uint8_t posicaoX;
uint8_t posicaoY;

uint8_t direcaoX;
uint8_t direcaoY;
uint8_t guarda_x_i_menos_2;
uint8_t guarda_x_i_menos_1;
uint8_t guarda_y_i_menos_2;
uint8_t guarda_y_i_menos_1;
uint8_t incX = 1;
uint8_t incY;
uint8_t delay = 25;
uint8_t conta_ate_T = 0;
uint8_t T;

bool gera_xs_anteriores = false;                                                // Gera um valor aleatorio de x_i_-_2 e 
                                                                                // x_i_-_2 quando reinicia
bool gera_ys_anteriores = false;                                                  // Gera um valor aleatorio de y_i_-_2 e 
                                                                                // y_i_-_2 quando reinicia
bool primeiro_T = true;
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
void gera_valorX(void);
/* 
 * Funcao que gera valores aleatorios da coordenada x
 */

void matrixPosicao(uint8_t posicaoX, uint8_t posicaoY);
/*
 *
 */

void gera_valorY(void);
/* 
 * Funcao que gera valores aleatorios da coordenada y
 */

void verificaT();

void txSpi(spiCS_t cs, uint8_t *data, size_t dataSize);

void reinicia(void);

void AtualizaXY(void);

bool recalcula = true;
const uint8_t MATRIZ_LED[]={
    0b10000000,     // x = 0 e y = 0
    0b00000000,
    0b00000000,   
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,        

    0b01000000,     // x = 0 e y = 1
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00100000,     // x = 0 e y = 2
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00010000,     // x = 0 e y = 3
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00001000,     // x = 0 e y = 4
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000100,     // x = 0 e y = 5
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000010,     // x = 0 e y = 6
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000001,     // x = 0 e y = 7
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 1 e y = 0
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 1 e y = 1
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 1 e y = 2
    0b00100000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 1 e y = 3
    0b00010000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 1 e y = 4
    0b00001000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 1 e y = 5
    0b00000100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 1 e y = 6
    0b00000010,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 1 e y = 7
    0b00000001,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 2 e y = 0
    0b00000000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 2 e y = 1
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 2 e y = 2
    0b00000000,
    0b00100000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 2 e y = 3
    0b00000000,
    0b00010000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 2 e y = 4
    0b00000000,
    0b00001000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 2 e y = 5
    0b00000000,
    0b00000100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 2 e y = 6
    0b00000000,
    0b00000010,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 2 e y = 7
    0b00000000,
    0b00000001,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 3 e y = 0
    0b00000000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 3 e y = 1
    0b00000000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 3 e y = 2
    0b00000000,
    0b00000000,
    0b00100000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 3 e y = 3
    0b00000000,
    0b00000000,
    0b00010000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 3 e y = 4
    0b00000000,
    0b00000000,
    0b00001000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 3 e y = 5
    0b00000000,
    0b00000000,
    0b00000100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 3 e y = 6
    0b00000000,
    0b00000000,
    0b00000010,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 3 e y = 7
    0b00000000,
    0b00000000,
    0b00000001,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 4 e y = 0
    0b00000000,
    0b00000000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 4 e y = 1
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 4 e y = 2
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 4 e y = 3
    0b00000000,
    0b00000000,
    0b00000000,
    0b00010000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 4 e y = 4
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 4 e y = 5
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000100,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 4 e y = 6
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000010,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 4 e y = 7
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000001,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 5 e y = 0
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 5 e y = 1
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 5 e y = 2
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 5 e y = 3
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00010000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 5 e y = 4
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001000,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 5 e y = 5
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000100,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 5 e y = 6
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000010,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 5 e y = 7
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000001,
    0b00000000,
    0b00000000,
    
    0b00000000,     // x = 6 e y = 0
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b10000000,
    0b00000000,
    
    0b00000000,     // x = 6 e y = 1
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    0b00000000,
    
    0b00000000,     // x = 6 e y = 2
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100000,
    0b00000000,
    
    0b00000000,     // x = 6 e y = 3
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00010000,
    0b00000000,
    
    0b00000000,     // x = 6 e y = 4
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001000,
    0b00000000,
    
    0b00000000,     // x = 6 e y = 5
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000100,
    0b00000000,
    
    0b00000000,     // x = 6 e y = 6
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000010,
    0b00000000,
    
    0b00000000,     // x = 6 e y = 7
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000001,
    0b00000000,
    
    0b00000000,     // x = 7 e y = 0
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b10000000,
    
    0b00000000,     // x = 7 e y = 1
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    
    0b00000000,     // x = 7 e y = 2
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100000,
    
    0b00000000,     // x = 7 e y = 3
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00010000,
    
    0b00000000,     // x = 7 e y = 4
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001000,
    
    0b00000000,     // x = 7 e y = 5
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000100,
    
    0b00000000,     // x = 7 e y = 6
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000010,
    
    0b00000000,     // x = 7 e y = 7
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000001,
    
    0b11111111,     // acende toda a matriz de LED
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    
    0b00000000,     // desligado
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
}; 

/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

