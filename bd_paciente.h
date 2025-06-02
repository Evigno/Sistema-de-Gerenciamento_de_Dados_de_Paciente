#ifndef BD_PACIENTE_H 
#define BD_PACIENTE_H

#include <stdio.h> 

// Constantes e seus respectivos limites
#define MAX_NOME 100
#define MAX_CPF 15
#define MAX_DATA 11
#define MAX_PACIENTES 100
#define MAX_LINE_CSV 256

// Estrutura para representar um paciente
typedef struct {
    int id;
    char cpf[MAX_CPF];
    char nome[MAX_NOME];
    int idade;
    char data_cadastro[MAX_DATA];
} Paciente;

// Estrutura TAD para o BD de Pacientes
typedef struct {
    Paciente pacientes[MAX_PACIENTES]; // Vetor estático para armazenar os registros dos pacientes
    int quantidade_atual; // Contador para o número de pacientes atualmente armazenados no vetor
} BDPaciente;

// Protótipos das funções do TAD BDPaciente

// Função que aloca dinamicamente e inicializa uma nova estrutura BDPaciente. Parte crucial do gerenciamento do TAD
// Retorna um ponteiro para a estrutura BDPaciente criada em caso de sucesso, ou NULL se a alocação de memória falhar
BDPaciente* criar_bdpaciente();

// Função que carrega os dados dos pacientes de um arquivo CSV para a estrutura BDPaciente em memória
// Espera-se que o arquivo CSV contenha uma linha de cabeçalho, que será ignorada
int carregar_bd_do_csv(BDPaciente* bd, const char* nome_arquivo);

// Função que permite ao usuário consultar registros de pacientes
void consultar_paciente(const BDPaciente* bd);

// Função que imprime na tela a lista de todos os registros de pacientes armazenados no sistema
void imprimir_lista_pacientes(const BDPaciente* bd);

// Função que libera a memória que foi dinamicamente alocada para a estrutura BDPaciente
void liberar_bdpaciente(BDPaciente* bd);

#endif // Fim da diretiva BD_PACIENTE_H
