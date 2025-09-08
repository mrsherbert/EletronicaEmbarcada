;------------------------------------------------------------------------------;
;                                 TestPoint 1                                  ;
;                    Aluno: Herbert Tavares de Oliveira Silva                  ;
;         Matricula: 190014300, UnB-FGA Eletrônica Embarcada 2024.2            ;
;------------------------------------------------------------------------------;
    
#include <xc.inc>

    psect resetVec,class=CODE,delta=2
resetVec:
    PAGESEL	inicio
    GOTO	inicio

psect code,class=CODE,delta=2
inicio:
;--- Configurando todas as portas para digitais ---;
    BANKSEL ANSEL
    CLRF    ANSEL	;---Apenas PORTA---;
    CLRF    ANSELH
;--- Configurando todas as entradas é saídas ---;
;--- Os bits da saída 0 e da entrada 1 ---;
    BANKSEL TRISA
    MOVLW   00100000B
    MOVWF   TRISA
    MOVLW   00001111B
    MOVWF   TRISB
    MOVWF   TRISC
;--- Seleciona o banco 0 ---;
    BANKSEL PORTA
    CLRF    PORTA	; Limpa a saída do sistema
LOOP:
    CLRWDT
    MOVF    PORTC,W
    BTFSS   PORTA,5	; Testa se RA5 = 1, caso sim pula a proxima linha e soma
    GOTO    SUB
    ADDWF   PORTB,W	; W = PORTB + PORTC
    ANDLW   00011111B
    MOVWF   PORTA	; PORTA = W
    GOTO    LOOP
SUB:
    SUBWF   PORTB,W	; W = PORTB - PORTC
    ANDLW   00011111B
    MOVWF   PORTA	; PORTA = W
    GOTO    LOOP
    END resetVec