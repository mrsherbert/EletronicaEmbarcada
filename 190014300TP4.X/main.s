;------------------------------------------------------------------------------;
;                                TestPoint 4.1                                 ;
;                    Aluno: Herbert Tavares de Oliveira Silva                  ;
;         Matricula: 190014300, UnB-FGA Eletrônica Embarcada 2024.2            ;
;------------------------------------------------------------------------------;
#include <xc.inc>

    CONFIG  FOSC = INTRC_NOCLKOUT
    CONFIG  WDTE = ON
    CONFIG  MCLRE = ON
    CONFIG  LVP = OFF

    #define ledR PORTA,0
    #define ledG PORTA,1
    #define ledB PORTA,2
    
    GLOBAL led, count
    PSECT udata_shr
led:		; LED
    DS	1
count:
    DS	2

    psect resetVec,class=CODE,delta=2
resetVec:
    PAGESEL	inicio
    GOTO	inicio

inicio:
    BANKSEL ANSEL	; Seleciona o banco de ANSEL
    CLRF    ANSEL	; Configura as portas para digitais
    CLRF    ANSELH
    BANKSEL TRISA	; Seleciona o banco de TRISA
    CLRF    TRISA	; Saídas == '0', entradas == '1'
    MOVLW   01101111B
    MOVWF   OPTION_REG	; Fosc/4 , Prescaler 128/
    BANKSEL PORTA
    CLRF    PORTA	; Limpar as saídas
    BANKSEL OSCCON
    MOVLW   01010001B	; Configura oscilador para 2Mhz	
    MOVWF   OSCCON
    BANKSEL WDTCON
    MOVLW   00000101B	; Prescaler de 128
    MOVWF   WDTCON
    BANKSEL TMR0
    CLRF    TMR0
    BANKSEL INTCON
    CLRF    INTCON	; NÃO É PRECISO NENHUMA INTERRUPÇÃO
    CLRF    STATUS
    MOVLW   00110010B	; Contador 
    MOVWF   count+1
    MOVLW   01000110B	; Contador
    MOVWF   count
    CLRF    led		; Inicia o registrador do led
LOOP:
    CLRWDT
    BTFSC   INTCON,2	; Verifica o flag TIMER0
    DECFSZ  count,F	; Decrementa contador, se zerado pula a prox instrução
    GOTO    LOOP
    MOVLW   11111111B	; Contador 
    MOVWF   count
    DECFSZ  count+1,F	; Decrementa contador, se zerado pula a prox instrução
    GOTO    LOOP
    GOTO    MSLEEP
MSLEEP:
    BCF	    PORTA,0	; Zera led vermelho
    BCF	    PORTA,1	; Zera led verde
    CLRWDT
    SLEEP		; Dorme por 1 segundo
    INCF    led
    BTFSC   led,0	; Verifica o último bit do led
    GOTO    VERDE
VERMELHO:
    BSF	    PORTA,0	; Liga led vermelho
    CLRF    led		; Limpa o registrador
    GOTO    TR0END
VERDE:
    BSF	    PORTA,1
TR0END:
    MOVLW   10010001B
    MOVWF   TMR0
    MOVLW   00110010B	; Contador 
    MOVWF   count+1
    MOVLW   01000110B	; Contador
    MOVWF   count
    BCF	    INTCON,2	; Apaga flag do timer
    GOTO    LOOP