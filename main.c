#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bd_paciente.h"

// Função que limpa o buffer de entrada do teclado
static void limpar_buffer_entrada_main() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função que exibe o menu principal do sistema para o usuário
void exibir_menu() {
    printf("\nHealthSys - Gerenciamento de Pacientes (Parte I)\n");
    printf("1 - Consultar paciente\n");
    // Opções 2, 3, 4 (Atualizar, Remover, Inserir) são para próximas partes do trabalho
    printf("5 - Imprimir lista de pacientes\n");
    printf("Escolha uma opção: ");
}

// Função principal do programa
int main() {
    // Tenta criar (alocar e inicializar) a estrutura do banco de dados de pacientes, verificando se a mesma foi bem sucedida
    BDPaciente* bd = criar_bdpaciente();
    if (bd == NULL) {
        fprintf(stderr, "Erro crítico: Falha ao alocar memória para o banco de dados.\n");
        return EXIT_FAILURE; // Termina o programa com código de erro se falhar
    }

    // Tenta carregar os dados dos pacientes do arquivo "bd_paciente.csv" ao iniciar o sistema[cite: 13]
    if (carregar_bd_do_csv(bd, "bd_paciente.csv") != 0) {
        // Se carregar_bd_do_csv retornar um valor diferente de 0, exibe a mensagem abaixo
        fprintf(stderr, "Aviso: Não foi possível carregar todos os dados de 'bd_paciente.csv'.\n");
    }

    char opcao_str[10];
    char opcao_char;

    // Loop principal do menu do sistema
    while (1) {
        exibir_menu(); // Mostra as opções do menu
        if (fgets(opcao_str, sizeof(opcao_str), stdin) == NULL) {
            // Se fgets retornar NULL, ocorreu um erro de leitura ou fim de arquivo (EOF)
            printf("Erro ao ler opção ou fim de entrada.\n");
            break;
        }

        if (strlen(opcao_str) > 0 && opcao_str[strlen(opcao_str) - 1] == '\n') {
            opcao_str[strlen(opcao_str) - 1] = '\0';
        } else {
            // Se a string não termina com '\n' e seu tamanho é o máximo do buffer menos 1, significa que a entrada do usuário foi maior que o buffer
            // Nesse caso, é preciso limpar o restante do buffer de entrada
            if (strlen(opcao_str) == sizeof(opcao_str) - 1) {
                 limpar_buffer_entrada_main();
            }
        }

        // Verifica se o usuário digitou exatamente um caractere para a opção (em que ponto chegamos...)
        if (strlen(opcao_str) == 1) {
            opcao_char = toupper(opcao_str[0]); // Converte a opção para maiúscula para tratar 'q' e 'Q' da mesma forma, uma vez que o C é uma linguagem case sensitive

            switch (opcao_char) {
                case '1':
                    consultar_paciente(bd); // Chama a função do TAD para consultar pacientes
                    break;
                case '5':
                    imprimir_lista_pacientes(bd); // Chama a função do TAD para imprimir a lista
                    break;
                case 'Q':
                    printf("Desligando...\n");
                    liberar_bdpaciente(bd); // Libera a memória
                    return EXIT_SUCCESS;
                default: // Caso o jumento do usuário digite uma opção inválida
                    printf("Opção inválida. Tente novamente.\n");
                    break;
            }
        } else {
             // Se o asno digitou mais de um caractere ou nenhum
             printf("Opção inválida. Por favor, digite apenas '1', '5' ou 'Q' :)\n");
        }
    }

    // Libera a memória do banco de dados caso o loop principal seja interrompido por um 'break' (o que só tem chance de ocorrer uma vez nesse programa - por enquanto)
    liberar_bdpaciente(bd);
    return EXIT_SUCCESS;
}