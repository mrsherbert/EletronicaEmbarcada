/**
 * @file main.c
 * @brief Função principal do sistema.
 *
 * Este arquivo contém a função principal que:
 * - Inicializa o sistema e os periféricos.
 * - Configura as interrupções para aquisição de dados, controle do motor de passos e comunicação UART.
 * - Realiza a calibração da válvula.
 * - Entra em um loop infinito para monitoramento dos sinais e processamento dos dados.
 *
 * Autores: [Adicionar os autores conforme necessário]
 * Data: [Inserir data, se necessário]
 */

#include "mcc_generated_files/mcc.h"
#include "funcoes.h"
#include "main.h"

/*
 * Função: main
 * Descrição: Função principal que inicializa o sistema, configura as interrupções e entra em um loop infinito para processar
 *            a recepção de dados via UART e atualizar o estado do LED conforme o status do sensor.
 */
void main(void) {
    SYSTEM_Initialize();                   // Inicializa os módulos e periféricos do sistema
    
    INTERRUPT_GlobalInterruptEnable();     // Habilita interrupções globais
    
    INTERRUPT_PeripheralInterruptEnable();   // Habilita interrupções periféricas
    
    // Configura a interrupção para processar dados após 40 ms sem recepção
    TMR4_SetInterruptHandler(ProcessaDados);
    
    // Configura a interrupção para iniciar a medição do sensor ultrassônico
    TMR0_SetInterruptHandler(inicia_ultrasom);
    
    // Configura a interrupção do GATE para calcular a posição da bola no tubo
    TMR1_SetGateInterruptHandler(posicao_bola_tubo);
    
    // Configura a interrupção para o motor de passos
    TMR6_SetInterruptHandler(DRMotordePassos);

#if SIMULACAO
    posicao_val_atual = 0;  // Em simulação, zera a posição da válvula
#else
    // Calibração da válvula: espera até que o sensor de fim de curso indique o limite
    while(!CMP1_GetOutputStatus()){
        flag_passos = false;  // Move o motor para diminuir a posição
        muda_passos();
        __delay_ms(4);
    }
    posicao_val_atual = 0;  // Define a posição atual como zero após a calibração
    
    Led_LAT = !CMP1_GetOutputStatus(); // Atualiza o LED conforme o status do sensor de fim de curso
#endif
    
    while (1)
    {
        if(EUSART_is_rx_ready()){
            Dados_recebidos();  // Processa os dados recebidos via UART quando disponíveis
        }
    }
}
