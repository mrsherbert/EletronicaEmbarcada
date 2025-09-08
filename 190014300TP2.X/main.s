;------------------------------------------------------------------------------;
;                                 TestPoint 2                                  ;
;                    Aluno: Herbert Tavares de Oliveira Silva                  ;
;         Matricula: 190014300, UnB-FGA Eletrônica Embarcada 2024.2            ;
;------------------------------------------------------------------------------;
    
#include <xc.inc>

CONFIG  FOSC = INTRC_NOCLKOUT; 
GLOBAL AUXIC, AUXC, AUXDP
PSECT udata_shr
AUXIC:
    DS	2 ; AUXILIAR DO RESULTADO
AUXC:
    DS	1 ; AUXILIAR DE CONTAGEM
AUXDP:
    DS	2 ; AUXILIAR DE DESLOCAMENTO
    
    psect resetVec,class=CODE,delta=2
resetVec:
    PAGESEL	inicio
    GOTO	inicio

psect code,class=CODE,delta=2
inicio:
    BANKSEL ANSEL
    CLRF    ANSEL
    CLRF    ANSELH
    BANKSEL TRISA
    CLRF    TRISA
    CLRF    TRISC
    BCF	    OPTION_REG, 7
    BANKSEL PORTA
    CLRF    PORTA
    CLRF    PORTC
    CLRF    STATUS
LOOP:
    CLRWDT
    CLRF    AUXIC
    CLRF    AUXIC+1
    CLRF    AUXDP+1
    MOVF    PORTB,W  
    MOVWF   AUXC
    BTFSC   STATUS,2	;Pula se não for zero
    GOTO    MZERO
    MOVF    PORTD,W
    MOVWF   AUXDP
    BTFSC   STATUS,2	;Pula se não for zero
    GOTO    MZERO
    BTFSC   PORTE,0	;Pula se for zero
    GOTO    DISPLACE	;Deslocamentos sucessivos
;------------------------------ MÉTODO DAS SOMAS ------------------------------;
SUM:
    ADDWF   AUXIC,F
    BTFSS   STATUS,0	; Verifica Carry
    GOTO    $+2
    INCF    AUXIC+1
    BCF	    STATUS,0
    DECFSZ  AUXC,1
    GOTO    SUM
    GOTO    EXIT
;------------------------ UM DOS VALORES É IGUAL A ZERO -----------------------;
MZERO:	    ; É necessário zerar os valores das portas porque senão o resultado 
    CLRF    PORTA	;anterior fica na memória como o verdadeiro resultado
    CLRF    PORTC
    GOTO    WRITER
;-------------------------- MÉTODO DOS DESLOCAMENTOS --------------------------;    
DISPLACE:
    RRF	    AUXC
    BTFSS   STATUS,0	; Verifica Carry
    GOTO    $+9
    BCF	    STATUS,0
    MOVF    AUXDP,W
    ADDWF   AUXIC,F
    BTFSS   STATUS,0	; Verifica Carry
    GOTO    $+2
    INCF    AUXIC+1
    MOVF    AUXDP+1,W
    ADDWF   AUXIC+1,F
    BCF	    STATUS,0
    RLF	    AUXDP
    RLF	    AUXDP+1
    MOVF    AUXC,W
    BTFSC   STATUS,2	;Pula se não for zero
    GOTO    $+2
    GOTO    DISPLACE
    GOTO    EXIT
;------------------------------------ SAÍDA -----------------------------------;
EXIT:
    MOVF    AUXIC,W
    MOVWF   PORTA
    MOVF    AUXIC+1,W
    MOVWF   PORTC
    GOTO    WRITER
;----------------------------- ESCRITA NA MEMÓRIA -----------------------------;
WRITER:
    MOVF    FSR,W
    BTFSS   STATUS,2	; Se flagZ=1 pula
    GOTO    $+3
    MOVLW   0x20	; Aplica valor inicial em FSR
    MOVWF   FSR
    MOVLW   0x70
    XORWF   FSR,W	; W = FSR xor 0x70, só dara zero quando atingir o maximo
    BTFSC   STATUS,2	; Verifica flag zero, se z=0 pula
    GOTO    $-5
    MOVF    FSR,W	; W = FSR = valor atual
    MOVWF   AUXC	; AUX = FSR banco x0
    ADDLW   0x80
    MOVWF   AUXIC	; AUXIC = FSR banco x1
    BCF	    STATUS,7	; banco 0x, banco atual 00
    MOVF    PORTB,W
    MOVWF   INDF	; Implementa a(FSR-20h) na memória
    BSF	    STATUS,7	; banco 1x, banco atual 10
    MOVF    PORTC,W
    MOVWF   INDF	; Implementa cH(FSR-20h) na memória
    MOVF    AUXIC,W
    MOVWF   FSR		; FSR = AUXIC = FSR banco x1, banco atual 11
    MOVF    PORTA,W
    MOVWF   INDF	; Implementa cL(FSR-20h) na memória
    BCF	    STATUS,7	; banco 0x, banco atual 01
    MOVF    PORTD,W
    MOVWF   INDF	; Implementa b(FSR-20h) na memória
    MOVF    AUXC,W
    MOVWF   FSR
    INCF    FSR		; Incrementa o valor anterior de FSR
    GOTO    LOOP
    END resetVec