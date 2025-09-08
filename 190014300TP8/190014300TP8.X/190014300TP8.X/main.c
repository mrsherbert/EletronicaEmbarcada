/*
;------------------------------------------------------------------------------;
;                               TestPoint 8                                    ;
;                    Aluno: Herbert Tavares de Oliveira Silva                  ;
;         Matricula: 190014300, UnB-FGA Eletrônica Embarcada 2024.2            ;
;------------------------------------------------------------------------------;
 */

#include "mcc_generated_files/mcc.h"
#include "main.h"

/*
                         Main application
 */

/*
 * << Deslocamento para a esquerda
 * >> Deslocamento para a direita
 */

void TRUSART(){
    if(swit){
    // Os dados transmitidos do canal de temperatura
    TX1dados.Tx1 = (uint16_t)((ADC_GetConversion(channel_AN0)* 1000 /1024) * vGAINt); // 0 - 100 ° C com ponto fixo
    
    
    // Conversão para os valores em hexadecimal reais
    TX1dados.vL1 = TX1dados.vL1 << 1;                       // Desloca parte baixa para esquerda
    TX1dados.Tx1 = (TX1dados.Tx1 >> 1);                     // Desloca tudo em conjunto
    TX1dados.vH1 = TX1dados.vH1 & 0x7F;                     // aplica mascara para zerar ultimo bit


    // Os dados transmitidos do canal de luminosidade
    TX2dados.Tx2 = (uint16_t)((ADC_GetConversion(channel_AN1)* 1000 /1024) * vGAINl); // 0 - 100% iluminação com ponto fixo

    // Conversão para os valores em hexadecimal reais
    TX2dados.vL2 = TX2dados.vL2 << 1;                       // Desloca parte baixa para esquerda
    TX2dados.Tx2 = (TX2dados.Tx2 >> 1);                     // Desloca tudo em conjunto
    TX2dados.vH2 = TX2dados.vH2 & 0x7F;                     // aplica mascara para zerar ultimo bit
    
        
        // Envio dos dados
    EUSART_Write(0x80);                                     // Envia parte alta
    EUSART_Write(TX1dados.vH1);                             // Envia parte alta
    EUSART_Write(TX1dados.vL1);                             // Envia parte baixa
    EUSART_Write(TX2dados.vH2);                             // Envia parte alta
    EUSART_Write(TX2dados.vL2);                             // Envia parte baixa
        //PIR1bits.RCIF = false;
    }
}

void selecaoRX(){
    switch(ByteRx[0]){
        // Tabelas de seleção do comando
        
        // Habilita - Desabilita o envio
        case RX_SWITCH:      
            if(RXdados.vHx== 0x00){
            if(RXdados.vLx == 0x00){        swit = false;}  // Habilita
            else if(RXdados.vLx == 0x01){   swit = true;}   // Desabilita
            }
            countRx = 0;
            break;
            
        // Tabela de atrualização PWM
        case RX_PWM:
            if(countRx == SIZECMD){                         // Se a mensagem tem o número correto de bytes
                if(RXdados.vHx<0x08){                       // Se não atingir o valor máximo
                    vPWM = RXdados.Rtx;                     // Atribui a variavel do PWM
                    EPWM2_LoadDutyValue(vPWM);              // Atualiza PWM
                }}
            countRx = 0;
            break;
            
        // Comando do ganho de luz
        case RX_GAINl:
            if(countRx == SIZECMD){                         // Se a mensagem tem o número correto de bytes
                if(RXdados.vHx<0x80){                       // Se o bit transmitido ao ganho for maior de 16383
                    vGAINl = RXdados.Rtx;                   // Inclui o valor ao ganho
                    vGAINl = vGAINl/1000;
                }}
            countRx = 0;
            break;
            
        // Comando do ganho de temperatura    
        case RX_GAINt:
            if(countRx == SIZECMD){                         // Se a mensagem tem o número correto de bytes
                if(RXdados.vHx<0x80){                       // Se o bit transmitido ao ganho for maior de 16383
                    vGAINt = RXdados.Rtx;                   // Atribui o valor ao ganho
                    vGAINt = vGAINt/1000;
                }}
            countRx = 0;
            break;
            
        default:
            countRx = 0;
            __delay_ms(200);
            break;
    }
}

void desloca(){
    RXdados.vHx = ByteRx[1];                                // Bit MSB
    RXdados.vLx = ByteRx[2];                                // Bit LSB
    if(RXdados.vLx>0x7F){return;}                           // Returna caso não atinge o necessario
    if(RXdados.vHx>0x7F){return;}                           // Returna caso não atinge o necessario
    RXdados.vLx = RXdados.vLx << 1;                         // Desloca parte baixa para esquerda
    RXdados.Rtx = (RXdados.Rtx >> 1);                       // Desloca tudo em conjunto para direita
    RXdados.vHx = RXdados.vHx & 0x7F;                       // aplica mascara para zerar ultimo bit
    selecaoRX();
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

    
    TMR1_SetInterruptHandler(TRUSART);
    
    while (1)
    {
        if(EUSART_is_rx_ready()){ 
            readrx = EUSART_Read();
            if(countRx < SIZECMD){
                ByteRx[countRx] = readrx;
                countRx++;
                if(countRx == SIZECMD){
                    desloca();
                }
            }else{
                __delay_ms(200);
                countRx = 0;
            }
        }
    }
}
/**
 End of File
*/