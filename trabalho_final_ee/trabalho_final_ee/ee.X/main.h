/**
 * @file main.h
 * @brief Define macros, variáveis globais e estruturas utilizadas no sistema.
 *
 * Este arquivo contém as definições de limites, códigos de comando, tabela de velocidade do som em função da temperatura e
 * as variáveis globais empregadas nos algoritmos de controle.
 * 
 * Autores: Herbert Tavares, Julio Cesar Carvalhes, Pedro Sampaio, Rafael Brasileiro
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Número máximo de passos da válvula (limite físico).
 */
#define numero_maximo_de_passos 460

/**
 * @brief Valor máximo do ciclo de trabalho do PWM (10 bits).
 */
#define maximo_pwm 1023

/**
 * @brief Número total de bytes esperados na comunicação UART.
 */
#define quadro_completo_ABCD 5

/**
 * @brief Modo de operação: Controle manual (sem PID).
 */
#define COMANDO_MANUAL 0x00

/**
 * @brief Modo de operação: Controle pela ventoinha (PID ativo).
 */
#define COMANDO_VENTOINHA 0x01

/**
 * @brief Modo de operação: Controle pela válvula (PI ativo).
 */
#define COMANDO_VALVULA 0x02

/**
 * @brief Comando para resetar o sistema.
 */
#define COMANDO_RESET 0x03

/**
 * @brief Macro para a simulação.
 */
#define SIMULACAO false
    
/**
 * @brief Macro para a debug.
 */
#define DEBUG false

/**
 * @brief Tabela armazenada na memória EEPROM que mapeia a temperatura (°C) à velocidade do som.
 *
 * Cada índice representa um valor de temperatura em °C e retorna a velocidade do som correspondente.
 */
__eeprom uint16_t relacao_som_temperatura[51] = {
    33145, 33206, 33266, 33327, 33387, 33447, 33507, 33567, 33627, 33687,
    33746, 33806, 33865, 33925, 33984, 34043, 34102, 34161, 34220, 34278,
    34337, 34396, 34454, 34512, 34570, 34629, 34687, 34745, 34802, 34860,
    34918, 34975, 35033, 35090, 35147, 35205, 35262, 35319, 35375, 35432,
    35489, 35546, 35602, 35659, 35715, 35771, 35827, 35883, 35939, 35995,
    36051
};

/**
 * @brief Union para manipulação de dados de 16 bits, permitindo acesso aos bytes MSB e LSB.
 *
 * Utilizada para transmissão e recepção de valores pela UART.
 */
union {
    uint16_t Dado;    /**< Valor de 16 bits completo. */
    struct {
        uint8_t blsb; /**< Byte menos significativo (LSB). */
        uint8_t bmsb; /**< Byte mais significativo (MSB). */
    };
} Set_altura, Set_valvula, Set_ciclo;

/**
 * @brief Tempo de amostragem ou variável auxiliar de tempo.
 */
int16_t tempo_s;

/**
 * @brief Diferença entre a altura desejada e a altura da bola (erro de controle).
 */
int24_t erro_atual = 0;

/**
 * @brief Variável auxiliar para testes em simulação.
 */
int24_t soma_anterior;

/**
 * @brief Variável auxiliar para armazenar o ciclo de trabalho anterior do PWM.
 */
int24_t erro_anterior = 0;

/**
 * @brief Acumulador do termo integral do controlador.
 */
int24_t integrativo = 0;

/**
 * @brief Componente proporcional do controle PID.
 */
int24_t proporcional = 0;

/**
 * @brief Componente derivativo do controle PID.
 */
int24_t derivativo = 0;

/**
 * @brief Soma total dos termos (P, I e D, se aplicável) do controlador.
 */
int24_t soma_PID = 0;

/**
 * @brief Buffer de recepção para armazenar os bytes recebidos via UART.
 */
int8_t RxBuffer[5];

/**
 * @brief Ganho do termo integral da ventoinha (valor dividido por 1000).
 */
int24_t ki_ventoinha = 2;

/**
 * @brief Ganho do termo derivativo da ventoinha.
 */
int24_t kd_ventoinha = 10000;

/**
 * @brief Ganho do termo proporcional da ventoinha.
 */
int24_t kp_ventoinha = 1300;

/**
 * @brief Ganho do termo integral da válvula.
 */
int24_t ki_valvula = 1000;

/**
 * @brief Ganho do termo proporcional da válvula.
 */
int24_t kp_valvula = 2950;

/**
 * @brief Média dos últimos tempos medidos pelo sensor ultrassônico.
 */
int16_t tempo_medio = 0;

/**
 * @brief Última altura medida da bola.
 */
int24_t altura_bola = 0;

/**
 * @brief Posição atual da válvula.
 */
int16_t posicao_val_atual = 0;

/**
 * @brief Valor de temperatura lido pelo sensor.
 */
uint16_t temperatura;

/**
 * @brief Posição futura da válvula determinada pelo controle.
 */
int16_t posicao_val_futura = 0;

/**
 * @brief Próximo valor do ciclo de trabalho da ventoinha determinado pelo controle.
 */
int16_t ciclo_util_futuro = 0;

/**
 * @brief Número de passos do motor de passos para controlar a válvula.
 */
int8_t npassos = 0;

/**
 * @brief Buffer de 15 bytes para envio de dados pela UART.
 */
int8_t dados_de_envio[15];

/**
 * @brief Armazena o modo de operação do sistema (Manual, Ventoinha, Válvula, Reset).
 */
uint8_t modo;

/**
 * @brief Flag que indica se os dados recebidos são atuais.
 */
bool dado_atual = false;

/**
 * @brief Flag que indica se o modo atual, falso para valvula, verdadeiro para ventoinha
 */
bool set_modo = false;

/**
 * @brief Flag que indica se ocorrera a soma ou a subtração.
 */
bool flag_passos = false;
/**
 * @brief Contador de bytes recebidos via UART.
 */
int8_t countRx = 0;

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
