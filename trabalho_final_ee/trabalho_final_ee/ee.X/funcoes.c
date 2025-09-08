/**
 * @file funcoes.c
 * @brief Implementação das funções de controle e comunicação do sistema.
 *
 * Este arquivo contém as implementações das funções responsáveis por:
 * - Iniciar a medição do sensor ultrassônico.
 * - Calcular a velocidade do som com base na temperatura medida.
 * - Processar os dados de medição e calcular a média dos tempos.
 * - Controlar a movimentação do motor de passos para ajuste da válvula.
 * - Gerenciar o controle PID/PI para a ventoinha e a válvula.
 * - Realizar a comunicação via UART (envio e recepção de dados).
 *
 * Autores: Herbert Tavares, Julio Cesar Carvalhes, Pedro Sampaio, Rafael Brasileiro
 */

#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "main.h"
#include "funcoes.h"

/*
 * Função: inicia_ultrasom
 * Descrição: Inicia a medição do sensor ultrassônico. A cada 100 ms, aciona o trigger, reseta o timer e envia dados via UART.
 */
void inicia_ultrasom(){
    static uint8_t j = 0;// Contador para gerar 100 ms (10 ciclos de 10 ms)
    if(j == 9){EnviaTx();}// Envia dados a cada 100 ms
    j = (j == 9) ? 0 : (j + 1);// Incrementa ou reinicia o contador
    
    Trigger_SetHigh();// Ativa o trigger do sensor ultrassônico
    TMR1_WriteTimer(0);// Zera o timer para precisão na medição
    __delay_us(12);// Mantém o trigger ativo pelo tempo necessário
    Trigger_SetLow();// Desativa o trigger
}

/*
 * Função: velocidade_som
 * Descrição: Retorna a velocidade do som com base na temperatura lida, utilizando a tabela armazenada na EEPROM.
 */
uint16_t velocidade_som() {
#if SIMULACAO
    // Em simulação, utiliza diretamente o valor da tabela para a temperatura atual
    return relacao_som_temperatura[temperatura];
#else
    // Obtém a temperatura via ADC e calcula a velocidade do som correspondente
    temperatura = (uint16_t)(ADC_GetConversion(Temp));
    return relacao_som_temperatura[(uint16_t)(temperatura/10)];
#endif
}

/*
 * Função: media
 * Descrição: Calcula a média dos dois últimos valores de tempo medido pelo sensor ultrassônico.
 */
void media(uint16_t timer) {
    static uint16_t tempo_recebido[2] = {0, 0};// Vetor para armazenar duas medições de tempo
    static uint8_t indice = 0;// Índice atual (0 ou 1)
    tempo_recebido[indice] = timer;// Armazena a nova medição
    indice ^= 1;// Alterna o índice entre 0 e 1 usando XOR
    tempo_medio = (tempo_recebido[0] + tempo_recebido[1]) / 2;// Calcula a média dos dois valores
}

/*
 * Função: posicao_bola_tubo
 * Descrição: Calcula a posição da bola no tubo utilizando a média dos tempos medidos e a velocidade do som, e invoca o controle PID/PI.
 */
void posicao_bola_tubo() {
    media(TMR1_ReadTimer());// Calcula a média dos tempos lidos do timer
    // Calcula a altura da bola utilizando a velocidade do som e o tempo médio
    altura_bola = (uint16_t)(((uint16_t)((velocidade_som() / 2000) * (tempo_medio))));
    Comando_PIeD();// Chama o controlador (PID ou PI) para ajustar válvula/ventoinha de acordo com altura

#if SIMULACAO
    // Em simulação, ajusta a temperatura e atualiza o PWM
    EPWM1_LoadDutyValue(ciclo_util_futuro);
#else
#endif
    
    TMR1_WriteTimer(0);// Reseta o timer para a próxima medição
}

/*
 * Função: DRMotordePassos
 * Descrição: Ajusta a posição do motor de passos para que a válvula alcance a posição desejada.
 */
void DRMotordePassos(){
    // Ajusta a posição atual em direção à posição futura
    if(posicao_val_atual > posicao_val_futura){
        posicao_val_atual--;
        flag_passos = false;// Move o motor para diminuir a posição
        muda_passos();
    }else if(posicao_val_atual < posicao_val_futura){
        posicao_val_atual++;
        flag_passos = true;// Move o motor para aumentar a posição
        muda_passos();
    }
    Led_LAT = !CMP1_GetOutputStatus(); // Atualiza o LED conforme o status do sensor de fim de curso
}

/*
 * Função: muda_passos
 * Descrição: Realiza a mudança dos passos.
 */
void muda_passos(){
    switch(npassos){
        case 0:
            SM1_SetHigh();// Ativa SM1
            SM2_SetLow();// Desativa SM2
            SM3_SetLow();// Desativa SM3
            SM4_SetHigh();// Ativa SM4
            break;
        case 1:
            SM1_SetHigh();// Ativa SM1
            SM2_SetHigh();// Ativa SM2
            SM3_SetLow();// Desativa SM3
            SM4_SetLow();// Desativa SM4
            break;
        case 2:
            SM1_SetLow();// Desativa SM1
            SM2_SetHigh();// Ativa SM2
            SM3_SetHigh();// Ativa SM3
            SM4_SetLow();// Desativa SM4
            break;
        case 3:
            SM1_SetLow();// Desativa SM1
            SM2_SetLow();// Desativa SM2
            SM3_SetHigh();// Ativa SM3
            SM4_SetHigh();// Ativa SM4
            break;
    }
    if(flag_passos){
        npassos = (npassos == 0) ? 3 : (npassos - 1);// Decrementa ou reinicia o contador
    }else{
        npassos = (npassos == 3) ? 0 : (npassos + 1);}// Incrementa ou reinicia o contador
}

/*
 * Função: Dados_recebidos
 * Descrição: Lê os dados recebidos via UART e os armazena no buffer, reiniciando o timer de timeout.
 */
void Dados_recebidos()
{
    dado_atual = true;// Indica que os dados recebidos são atuais
    uint8_t rxData = EUSART_Read();// Lê o dado recebido
    if(countRx < quadro_completo_ABCD){// Enquanto não atingir o número esperado de bytes
        RxBuffer[countRx] = rxData;// Armazena o byte no buffer
        countRx++;// Incrementa o contador de bytes
        TMR4_WriteTimer(0);// Reseta o timer de timeout
    }else{
        countRx++;// Apenas incrementa se ultrapassar o tamanho esperado
        TMR4_WriteTimer(0);// Reseta o timer de timeout
    }
}

/*
 * Função: ProcessaDados
 * Descrição: Processa os dados armazenados no buffer após o timeout, atualizando variáveis globais e invocando a seleção do modo.
 */
void ProcessaDados()
{
    if(!dado_atual){return;}// Se os dados não forem atuais, sai da função
    dado_atual = false;// Marca os dados como processados
    modo = RxBuffer[0];// Atualiza o modo de operação a partir do buffer
    if(countRx == quadro_completo_ABCD){
        switch(modo){
            case COMANDO_MANUAL:
                // Configura os setpoints para controle manual
                Set_valvula.bmsb = RxBuffer[1];
                Set_valvula.blsb = RxBuffer[2];
                Set_ciclo.bmsb   = RxBuffer[3];
                Set_ciclo.blsb   = RxBuffer[4];
                break;
            case COMANDO_VENTOINHA:
                // Configura os setpoints para controle via ventoinha
                Set_altura.bmsb  = RxBuffer[1];
                Set_altura.blsb  = RxBuffer[2];
                Set_valvula.bmsb = RxBuffer[3];
                Set_valvula.blsb = RxBuffer[4];
                break;
            case COMANDO_VALVULA:
                // Configura os setpoints para controle via válvula
                Set_altura.bmsb = RxBuffer[1];
                Set_altura.blsb = RxBuffer[2];
                Set_ciclo.bmsb  = RxBuffer[3];
                Set_ciclo.blsb  = RxBuffer[4];
                break;
            default:
                break;
        }
        selecao_do_modo();// Chama a função que ajusta os parâmetros de controle conforme o modo
    }
    if(modo == COMANDO_RESET){
        RESET();// Se o comando for reset, reinicia o microcontrolador
    }
    countRx = 0;// Zera o contador do buffer
}

/*
 * Função: selecao_do_modo
 * Descrição: Seleciona o modo de operação do sistema e configura os parâmetros conforme os dados recebidos.
 */
void selecao_do_modo(){
    switch(modo){
        case COMANDO_MANUAL:
            if(Set_ciclo.Dado > 1023){Set_ciclo.Dado = 1023;}// Limita o valor do PWM
            ciclo_util_futuro = Set_ciclo.Dado;// Atualiza o setpoint do PWM
            EPWM1_LoadDutyValue(ciclo_util_futuro);// Aplica PWM direto
            if(Set_valvula.Dado > 460){Set_valvula.Dado = 460;}// Limita o valor da posição da válvula
            posicao_val_futura = Set_valvula.Dado;
            break;
        case COMANDO_VENTOINHA:
            if(Set_valvula.Dado > 460){Set_valvula.Dado = 460;}// Limita Valvula
            posicao_val_futura = Set_valvula.Dado;// Implementa valvula constante
            
            // Zera os valores do PID anteriores
            erro_atual = 0;
            soma_PID = 0;
            integrativo = 0;
            proporcional = 0;
            derivativo = 0;
            break;
        case COMANDO_VALVULA:
            if(Set_ciclo.Dado > 1023){Set_ciclo.Dado = 1023;}// Limita PWM
            ciclo_util_futuro = Set_ciclo.Dado;// Implementa PWM constante
            EPWM1_LoadDutyValue(ciclo_util_futuro);// Aplica PWM direto
            
            // Zera os valores do PID anteriores
            erro_atual = 0;
            soma_PID = 0;
            integrativo = 0;
            proporcional = 0;
            derivativo = 0;
            break;
        default:
            break;
    }
}

/*
 * Função: EnviaTx
 * Descrição: Envia os dados de status e controle do sistema via UART, formatando-os conforme o modo de operação e se o sistema está em simulação ou operação real.
 */
void EnviaTx(){
#if DEBUG
    // Modo DEBUG: exibe informações detalhadas
    EUSART_Write((uint16_t)Set_altura.bmsb);
    EUSART_Write((uint16_t)Set_altura.blsb);
    EUSART_Write(((uint16_t)(altura_bola & 0xFF00)) >> 8);
    EUSART_Write(altura_bola & 0x00FF);
    EUSART_Write((erro_atual & 0xFF0000) >> 16);
    EUSART_Write((erro_atual & 0x00FF00) >> 8);
    EUSART_Write(erro_atual & 0x0000FF);
    EUSART_Write((integrativo & 0xFF0000) >> 16);
    EUSART_Write((integrativo & 0x00FF00) >> 8);
    EUSART_Write(integrativo & 0x0000FF);
    EUSART_Write((proporcional & 0xFF0000) >> 16);
    EUSART_Write((proporcional & 0x00FF00) >> 8);
    EUSART_Write(proporcional & 0x0000FF);
    EUSART_Write((derivativo & 0xFF0000) >> 16);
    EUSART_Write((derivativo & 0x00FF00) >> 8);
    EUSART_Write(derivativo & 0x0000FF);
    EUSART_Write((soma_PID & 0xFF0000) >> 16);
    EUSART_Write((soma_PID & 0x00FF00) >> 8);
    EUSART_Write(soma_PID & 0x0000FF);
    EUSART_Write((posicao_val_futura & 0xFF00) >> 8);
    EUSART_Write(posicao_val_futura & 0x00FF);
    EUSART_Write((posicao_val_atual & 0xFF00) >> 8);
    EUSART_Write(posicao_val_atual & 0x00FF);
    EUSART_Write((ciclo_util_futuro & 0xFF00) >> 8);
    EUSART_Write(ciclo_util_futuro & 0x00FF);
    EUSART_Write('\r');
    EUSART_Write('\n');
#elif SIMULACAO
    // Modo SIMULACAO: envia dados simplificados
    EUSART_Write(modo);
    EUSART_Write((uint16_t)Set_altura.bmsb);
    EUSART_Write((uint16_t)Set_altura.blsb);
    EUSART_Write((uint16_t)Set_valvula.bmsb);
    EUSART_Write((uint16_t)Set_valvula.blsb);
    EUSART_Write(((uint16_t)(ciclo_util_futuro & 0xFF00)) >> 8);
    EUSART_Write(ciclo_util_futuro & 0x00FF);
#else
    // Modo NORMAL: envia dados completos do sistema
    EUSART_Write(modo);
    EUSART_Write((uint16_t)Set_altura.bmsb);
    EUSART_Write((uint16_t)Set_altura.blsb);
    EUSART_Write(((uint16_t)(altura_bola & 0xFF00)) >> 8);
    EUSART_Write(altura_bola & 0x00FF);
    EUSART_Write(((uint16_t)(tempo_medio & 0xFF00)) >> 8);
    EUSART_Write(tempo_medio & 0x00FF);
    EUSART_Write(((uint16_t)(temperatura & 0xFF00)) >> 8);
    EUSART_Write(temperatura & 0x00FF);
    EUSART_Write((uint16_t)Set_valvula.bmsb);
    EUSART_Write((uint16_t)Set_valvula.blsb);
    EUSART_Write(((uint16_t)(posicao_val_atual & 0xFF00)) >> 8);
    EUSART_Write(posicao_val_atual & 0x00FF);
    EUSART_Write(((uint16_t)(ciclo_util_futuro & 0xFF00)) >> 8);
    EUSART_Write(ciclo_util_futuro & 0x00FF);
    EUSART_Write('\r');
    EUSART_Write('\n');
#endif
}


/*
 * Função: Comando_PIeD
 * Descrição: Seleciona e executa o controle PI ou PID, chamando a função correspondente conforme o modo de operação.
 */
void Comando_PIeD(){
    switch(modo){
        case COMANDO_VENTOINHA:
            set_modo = true;    // Modo ventoinha
            Controlador();      // Executa o controle PID para a ventoinha
            break;
        case COMANDO_VALVULA:
            set_modo = false;   // Modo valvula
            Controlador();      // Executa o controle PI para a válvula
            break;
        default:
            break;
    }
}

/*
 * Função: Controlador
 * Descrição: Implementa o controle PID para a ventoinha, calculando os termos proporcional, integral e derivativo,
 *            limitando a saída e mapeando o resultado para o PWM.
 */
void Controlador(){
    erro_atual = (int24_t)(((int24_t)Set_altura.Dado) - ((int24_t)(altura_bola)));
    integrativo += (int24_t)(((ki_ventoinha * erro_atual) / 1000));
    proporcional = ((((int24_t)kp_ventoinha / 1000)) * erro_atual);
    derivativo = (int24_t)(kd_ventoinha * (erro_atual - erro_anterior)) / 1000;
    soma_PID = proporcional + integrativo + derivativo;
    
    // Limita a saída para evitar valores excessivos
    if(soma_PID < 0) soma_PID = 0;
    if(soma_PID > 42000) soma_PID = 42000;
    
    if(set_modo){
        // Mapeia o valor do PID para o intervalo do PWM
        ciclo_util_futuro = (int16_t)map_value(soma_PID, 0, 42000, 600, 1023);
        EPWM1_LoadDutyValue(ciclo_util_futuro);
    }else{
        // Mapeia a saída para a posição da válvula (inversamente proporcional)
        posicao_val_futura = (int16_t)map_value(soma_PID, 0, 42000, 460, 0);
    }
    erro_anterior = erro_atual;
}

/*
 * Função: map_value
 * Descrição: Converte o valor 'x' do intervalo [in_min, in_max] para o intervalo [out_min, out_max].
 */
int24_t map_value(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
    return ((x - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
}
