/*
;------------------------------------------------------------------------------;
;                               TestPoint 9                                    ;
;                    Aluno: Herbert Tavares de Oliveira Silva                  ;
;         Matricula: 190014300, UnB-FGA Eletrônica Embarcada 2024.2            ;
;------------------------------------------------------------------------------;
 */

#include "mcc_generated_files/mcc.h"
#include "main.h"

/*
                         Main application
 */

void MSGTXT(const char *MSG){
    while (*MSG != '\0') {      // Enquanto não atinge o fim
        EUSART_Write(*MSG);     // Envia cada byte
        MSG++;                  // Incrementa ponteiro
    }
    EUSART_Write(0x0D);         // Caracter de fim da mensagem
}

void MSGFUNC(){
    switch(CMDRECORD){
        case CL:    // Comando de exibir lista
            // Caso a lista esteja sem agendamentos ou por algum motivo
            if(NAGEN == 0 || DATAEE_ReadByte(0) == '$'){
                
                // Mensagem de lista de agendamento vazia
                MSGTXT(MSG2);
                NAGEN = 0;
            }else{
                MSGTXT(MSG1);
               
                // For com todos os agendamentos disponiveis
                for(int j=0;j<NAGEN;j++){
                    int AUX1 = 22 * j;                    // Inicio de uma linha de agendamento 
                    bool MSGATIV = true;                  // Booleano de controle da impressão da linha
                    
                    while(MSGATIV){
                        ISRCVD = (uint8_t) DATAEE_ReadByte((uint8_t)AUX1);   // Lê valor armazenado
                        if(ISRCVD == 0x0D){
                            EUSART_Write(0x0D);           // Fim de mensagem
                            // Chegou ao byte que identifica o fim
                            MSGATIV = false;              // Sai do while
                        }else if(ISRCVD == '$'){
                            
                            //  REESTABELECE INICIO DE COMUNICAÇÃO NA MAIN
                            CMDRECEIV = false;
                            break;
                        }else{
                            
                            // Atualiza valor de memória e imprime atual
                            EUSART_Write(ISRCVD);
                            AUX1++;
                        }
                    }
                }
            }
            
            //  REESTABELECE INICIO DE COMUNICAÇÃO NA MAIN
            CMDRECEIV = false;
            break;
            
        case CA:    // Comando de iniciar agendamento
            if(NAGEN<10){
                int AUX1 = 22 * NAGEN;                  // Auxiliar de armazenamento na memoria
                int posi = 0;                           // Posição
                bool CONT = true;                       // Boleano de controle
                DATAEE_WriteByte((uint8_t)AUX1,'\0');   // Primeiro bit normalmente vazio p/ o X
                AUX1++;
                
                // Mensagem pedindo nome
                MSGTXT(MSG4);
                
                while(CONT){
                    // Enquanto estiver chegando dados
                    if(EUSART_is_rx_ready()){
                        
                        ISRCVD = EUSART_Read();
                        if(ISRCVD == 0x0D || posi == 21){
                            
                            // Guarda contexto
                            int Bposi = posi;

                            // Zerar posições de memoria inuteis
                            while(posi < 21){
                                MSGRX.NAME[posi] = 0x00;
                                posi++;
                            }
                            // Garantem o ultimo byte como 0x0D
                            MSGRX.NAME[posi-1] = 0x0D;

                            // Restaura contexto
                            posi = Bposi;
                            
                            // Garantem o ultimo byte como 0x0D
                            MSGRX.NAME[posi] = 0x0D;
                            
                            // Se não ouve nenhum agendamento so chegou 0x0D
                            if(posi == 0){

                                // Implementa bit impossivel
                                MSGRX.NAME[posi] = '$';

                                // Envia mensagem de Mensagem de erro
                                MSGTXT(MSG5);

                                // Sai do while
                                CONT = false;

                            // Se chegou o nome por final
                            }else{

                                // Incrementa o número de agendamentos e manda msg
                                NAGEN++;
                                MSGTXT(MSGS);

                                // Sai do while
                                CONT = false;
                            }
                        }else{
                            // Atribui o valor chegado a posição na memoria
                            MSGRX.NAME[posi] = ISRCVD;
                            posi++;
                        }
                    }
                }
                // Atribuição de valores
                for(int j=0;j<22;j++){
                    uint8_t letra = MSGRX.NAME[j];
                    DATAEE_WriteByte((uint8_t)AUX1,letra);
                    AUX1++;
                }
                                
            }else{
                
                // Lista de agendamento cheia
                MSGTXT(MSG7);
            }
            
            //  REESTABELECE INICIO DE COMUNICAÇÃO NA MAIN
            CMDRECEIV = false;
            break;
            
        case CP:    // Comando para exibir e marcar o proximo
            if(NPROX<NAGEN){
                MSGTXT(MSG3);               // Mensagem para exibir o proximo
                int AUX1 = 22 * NPROX;      // Auxiliar de inicio do proximo agendamento
                for(int j=0; j<22;j++){
                    ISRCVD = (uint8_t) DATAEE_ReadByte((uint8_t)AUX1);
                    if(ISRCVD == 0x0D){     // Caracter de fim de linha
                        EUSART_Write(0x0D); // Imprime o fim
                        j = 23;             // Sai do for
                        
                    }else if(ISRCVD == '$'){ // identificador de linha apagada
                        
                        //  REESTABELECE INICIO DE COMUNICAÇÃO NA MAIN
                        CMDRECEIV = false;
                        break;
                        
                    }else{                  // Não chegou ao fim
                        EUSART_Write(ISRCVD);   // Imprime
                        AUX1++;             // Atualiza proxima posição
                    }
                }
                
                // Atualiza valores e coloca X no nome
                AUX1 = 22 * NPROX;
                DATAEE_WriteByte((uint8_t)AUX1,'X');
                NPROX++;
                
            }else if(NPROX == 0){
                MSGTXT(MSG2);   // Lista vazia
            }else{
                MSGTXT(MSG6);   // Todos atendidos
            }
            
            //  REESTABELECE INICIO DE COMUNICAÇÃO NA MAIN
            CMDRECEIV = false;
            break;
            
        case CR:    // Comando para apagar a lista
            // Implementa byte que identifica todos os valores apagados
            for(int i = 0; i <NAGEN; i++){
                DATAEE_WriteByte((uint8_t)i*22,'$');
            }
            NAGEN = 0;
            NPROX = 0;
            
            //  REESTABELECE INICIO DE COMUNICAÇÃO NA MAIN
            CMDRECEIV = false;
            break;
    }
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

    MSGTXT(MSG0);
    
    while (1)
{
        if (EUSART_is_rx_ready()) {
            ISRCVD = EUSART_Read(); // Armazenar valor enviado
            if (CMDRECEIV) { // Se já chegou um CMD valido
                if (ISRCVD == 0x0D) { // Se está corretamente enviado
                    // Reinicia o bool de controle e vai a função
                    CMDRECEIV = false;
                    MSGFUNC();
                } else { // Se não foi enviado corretamente
                    // Reinicia o boll e imprime CR
                    CMDRECEIV = false;
                    EUSART_Write(0x0D);
                }
            } else {
                if (ISRCVD == CA || ISRCVD == CL || ISRCVD == CP || ISRCVD == CR) {// Comandos validos
                    // Inicia verificação e guarda comando
                    CMDRECEIV = true;
                    CMDRECORD = ISRCVD;
                }else{
                    MSGTXT(MSG0);
                }
            }
        }
    }
}
/**
 End of File
*/