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

#define CL      'L'     // Comando exibe lista de agendados
#define CA      'A'     // Comando inicia o agendamento
#define CP      'P'     // Comando ecibe nome do proximo cliente a ser atendido e marca como atendido
#define CR      'R'     // Comando apaga todos os agendados

const char MSG0[]            = "L-Exibe lista, A-Agenda, P-Proximo, R-Apaga lista";
const char MSG1[]            = "Lista de agendamentos";
const char MSG2[]            = "Lista de agendamentos vazia";
const char MSG3[]            = "Proximo: ";
const char MSG4[]            = "Digite o nome:";
const char MSG5[]            = "Nao foi possivel agendar (nome vazio)";
const char MSG6[]            = "Todos os agendamentos foram atendidos";
const char MSG7[]            = "Nao disponemos de mais agendamentos";
const char MSGS[]            = "O agendamento de foi realizado";

union{
    char MSGREC[22];
    struct{
        char NAME[22];
    };
}MSGRX;


// Variaveis
uint8_t ISRCVD;
bool    CMDRECEIV = false;
char    CMDRECORD;
int     NAGEN = 0;
int     NPROX = 0;

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

