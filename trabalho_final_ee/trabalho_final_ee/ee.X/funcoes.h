/**
 * @file funcoes.h
 * @brief Declaração das funções de controle, aquisição de dados e movimentação do motor.
 *
 * Este arquivo contém os protótipos de funções responsáveis pelo controle do sistema, incluindo
 * aquisição de dados do sensor ultrassônico, processamento da comunicação UART, e controle dos atuadores.
 *
 * Autores: Herbert Tavares, Julio Cesar Carvalhes, Pedro Sampaio, Rafael Brasileiro
 */

#ifndef FUNCOES_H
#define FUNCOES_H

#include <xc.h> // Inclui os arquivos do processador, cada arquivo já possui proteção contra inclusão múltipla.

/**
 * @brief Retorna a velocidade do som de acordo com a temperatura.
 *
 * Esta função utiliza a tabela armazenada na EEPROM para determinar a velocidade do som
 * com base na temperatura lida.
 *
 * @return Velocidade do som correspondente à temperatura.
 */
uint16_t velocidade_som();

/**
 * @brief Realiza a mudança dos passos.
 */
void muda_passos();

/**
 * @brief Atualiza os passos do motor de acordo com a posição desejada.
 */
void DRMotordePassos();

/**
 * @brief Calcula a posição da bola no tubo.
 */
void posicao_bola_tubo(void);

/**
 * @brief Calcula a média do sistema com base no tempo medido.
 *
 * @param timer Valor lido do timer.
 */
void media(uint16_t timer);

/**
 * @brief Realiza a medição da altura.
 */
void medir_altura(void);

/**
 * @brief Adquire a posição da bola no tubo.
 */
void Posicao_Bola_Tubo(void);

/**
 * @brief Carrega o buffer de transmissão (Tx) com as informações desejadas.
 */
void EnviaTx(void);

/**
 * @brief Seleciona o modo de operação do sistema e configura os parâmetros correspondentes.
 */
void selecao_do_modo(void);

/**
 * @brief Controla a ventoinha ou valvula para ajustar a altura da bola usando um controlador PID.
 */
void Controlador(void);

/**
 * @brief Lê os dados recebidos pela UART e armazena-os no buffer para processamento.
 */
void Dados_recebidos(void);

/**
 * @brief Executa o controle PI ou PID, dependendo do modo de operação selecionado.
 */
void Comando_PIeD(void);

/**
 * @brief Inicia a medição da altura da bola utilizando um sensor ultrassônico.
 */
void inicia_ultrasom(void);

/**
 * @brief Processa os dados recebidos após um intervalo de tempo.
 */
void ProcessaDados(void);

/**
 * @brief Converte um valor de um intervalo para outro.
 *
 * Esta função mapeia o valor de 'x' do intervalo [in_min, in_max] para o intervalo [out_min, out_max].
 *
 * @param x Valor atual a ser mapeado.
 * @param in_min Limite inferior do intervalo de entrada.
 * @param in_max Limite superior do intervalo de entrada.
 * @param out_min Limite inferior do intervalo de saída.
 * @param out_max Limite superior do intervalo de saída.
 * @return Valor mapeado correspondente.
 */
int24_t map_value(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

#endif /* FUNCOES_H */
