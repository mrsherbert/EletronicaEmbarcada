;------------------------------------------------------------------------------;
;                                 TestPoint 3                                  ;
;                    Aluno: Herbert Tavares de Oliveira Silva                  ;
;         Matricula: 190014300, UnB-FGA Eletrônica Embarcada 2024.2            ;
;------------------------------------------------------------------------------;
    
#include <xc.inc>

CONFIG  FOSC = INTRC_NOCLKOUT
GLOBAL n, d, q, r, AUX1, AUX2, ASCIIq, ASCIIr, COUNT
PSECT udata_shr
n:		; Numerador
    DS	1
d:		; Divisor
    DS	1
q:		; Quociente
    DS	1
r:		; Resto
    DS	1
AUX1:		; Guarda o valor de resto durante as operações
    DS	1
AUX2:		; Guarda o valor de quociente durante as operações
    DS	1
ASCIIq:		; ASCII do quociente
    DS	2
ASCIIr:		; ASCII do resto
    DS	2
COUNT:		; Variavel de contagem de deslocamento
    DS	1

    psect resetVec,class=CODE,delta=2
resetVec:
    PAGESEL	inicio
    GOTO	inicio
    
psect code,class=CODE,delta=2
inicio:
    BANKSEL ANSEL	; Seleciona o banco de ANSEL
    CLRF    ANSEL	; Configura as portas para digitais
    CLRF    ANSELH
    BANKSEL TRISA	; Seleciona o banco de TRISA
    CLRF    TRISA	; Saídas == '0', entradas == '1'
    CLRF    TRISC
    CLRF    TRISE
    BSF	    TRISE,0	; TRISE0 == '1'
    BCF	    OPTION_REG,7; Ativa os pullups PORTB
    BANKSEL PORTA
    CLRF    PORTA	; Limpar as saídas
    CLRF    PORTC
LOOP:
    CLRWDT
    CLRF    STATUS
    CLRF    PCLATH
    MOVF    PORTD,W
    MOVWF   d
    BTFSC   STATUS,2
    GOTO    ZEROF	; Se der zero termina
    MOVF    PORTB,W
    MOVWF   n		; n = W = PORTB
    BTFSC   PORTE,0	; Verifica o RE0
    GOTO    DISPLACE
;--------------------------- Método das subtrações ----------------------------;
SUB:
    MOVWF   AUX1	; No inicio AUX1 = n, No final é o resto
    CLRF    AUX2	; O valor do quociente
    MOVF    d,W		; W = d
    SUBWF   AUX1,W	; W = AUX1 - d, consecutivamente até zerar
    BTFSS   STATUS,0	; Verifica o carry
    GOTO    FINAL
    MOVWF   AUX1	; AUX1 = AUX1 - d
    INCF    AUX2	; AUX2 = AUX2 + 1
    GOTO    $-6
;-------------------------- Método dos deslocamentos --------------------------;
DISPLACE:
    MOVWF   AUX2	; No inicio AUX2 = n, No final é igual ao quociente
    MOVLW   1000B
    MOVWF   COUNT
    CLRF    AUX1	; No inicio AUX1 = 0, No final é igual ao resto
    RLF	    AUX2
    RLF	    AUX1
    MOVF    d,W		; W = d
    SUBWF   AUX1,W	; W = AUX1 - d
    BTFSS   STATUS,0	; Verifica se AUX1 < d
    GOTO    $+4		; Vai a linha 88
    BSF	    AUX2,0
    MOVWF   AUX1
    GOTO    $+2		; Vai a linha 89
    BCF	    AUX2,0
    BCF	    STATUS,0
    DECFSZ  COUNT,F	; Decrementa contador, se zerado pula a prox instrução
    GOTO    $-12	; Volta a linha 79
    GOTO    FINAL
;--------------------------- TERMINO DAS OPERAÇÕES ----------------------------;
ZEROF:
    CLRF    AUX2
    MOVF    PORTB,W
    MOVWF   AUX1
    BSF	    PORTE,1	; RE1 == 1
    GOTO    $+2		; Vai para linha 102
FINAL:
    BCF	    PORTE,1	; RE1 == 0
    MOVF    AUX1,W	; Resto
    MOVWF   r
    MOVWF   PORTC
    MOVF    AUX2,W	; Quociente
    MOVWF   q
    MOVWF   PORTA
    MOVLW   high (table)
    MOVWF   PCLATH
    MOVF    r,W
    ANDLW   00001111B
    CALL    table
    MOVWF   ASCIIr
    SWAPF   r,W
    ANDLW   00001111B
    CALL    table
    MOVWF   ASCIIr+1
    MOVF    q,W
    ANDLW   00001111B
    CALL    table
    MOVWF   ASCIIq
    SWAPF   q,W
    ANDLW   00001111B
    CALL    table
    MOVWF   ASCIIq+1
    GOTO    LOOP
ORG 0x400
table:
    ADDWF   PCL,F
    RETLW   0x30	; '0'
    RETLW   0x31	; '1'
    RETLW   0x32	; '2'
    RETLW   0x33	; '3'
    RETLW   0x34	; '4'
    RETLW   0x35	; '5'
    RETLW   0x36	; '6'
    RETLW   0x37	; '7'
    RETLW   0x38	; '8'
    RETLW   0x39	; '9'
    RETLW   0x41	; 'A'
    RETLW   0x42	; 'B'
    RETLW   0x43	; 'C'
    RETLW   0x44	; 'D'
    RETLW   0x45	; 'E'
    RETLW   0x46	; 'F'

END resetVec