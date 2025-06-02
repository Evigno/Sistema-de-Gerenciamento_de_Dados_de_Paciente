#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bd_paciente.h"

#define RECORDS_PER_PAGE 5

// Funções Auxiliares (Helpers)

// Limpa o buffer de entrada do teclado

void limpar_buffer_entrada() {
    int c;
    // Lê e descarta caracteres do buffer de entrada até encontrar uma nova linha ou o fim do arquivo.
    while ((c = getchar()) != '\n' && c != EOF);
}

// Imprime o cabeçalho formatado p a listagem de pacientes
static void imprimir_cabecalho_paciente() {
    // %-Xs: string, ocupando X caracteres.
    printf("%-5s %-15s %-30s %-5s %-15s\n", "ID", "CPF", "Nome", "Idade", "Data_Cadastro");
    printf("-------------------------------------------------------------------------------\n"); // Linha separadora.
}
//Imprime os dados de um único paciente, com a formatação certa

static void imprimir_dados_paciente(const Paciente* p) {
    printf("%-5d %-15s %-30s %-5d %-15s\n",
           p->id, p->cpf, p->nome, p->idade, p->data_cadastro);
}

// Implementação das Funções do TAD BDPaciente

// Aloca dinamicamente memória p uma estrutura BDPaciente e inicializa ela
BDPaciente* criar_bdpaciente() {
    BDPaciente* bd = (BDPaciente*)malloc(sizeof(BDPaciente));
    if (bd != NULL) {
        bd->quantidade_atual = 0;
    }
    return bd; // Retorna o ponteiro ou NULL se malloc falhou
}

// Libera a memória alocada p uma estrutura BDPaciente

void liberar_bdpaciente(BDPaciente* bd) {
    // Libera a memória apontada por 'bd'
    free(bd);
}

// Carrega dados de pacientes de um arquivo CSV p a estrutura BDPaciente

int carregar_bd_do_csv(BDPaciente* bd, const char* nome_arquivo) {
    if (bd == NULL) {
        fprintf(stderr, "Erro: Estrutura BDPaciente não inicializada (NULL).\n");
        return -1;
    }

    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo CSV"); // PEH error exibe a mensagem de erro do sistema
        return -1;
    }
// #CadêMeusDiasDeGlória
    char linha[MAX_LINE_CSV]; // Buffer p armazenar cada linha lida do arquivo

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fprintf(stderr, "Erro: Arquivo CSV vazio ou erro ao ler o cabeçalho.\n");
        fclose(arquivo);
        return -1;
    }

    bd->quantidade_atual = 0; // Reinicia a contagem de pacientes

    // Loop p ler cada linha do arquivo até o máx de pacientes ou o fim do arquivo
    // Continua enquanto <houver espaço no vetor> e enquanto <fgets conseguir ler uma linha>
    while (bd->quantidade_atual < MAX_PACIENTES && fgets(linha, sizeof(linha), arquivo) != NULL) {
        // strcspn retorna o índice do 1º caractere de 'reject' encontrado em 'linha'
        // Quando '\0' for atribuído a essa posição, a string termina
        linha[strcspn(linha, "\n")] = 0;
        linha[strcspn(linha, "\r")] = 0;

        // Obtém um ponteiro p a próxima posição livre no vetor de pacientes
        Paciente* p = &bd->pacientes[bd->quantidade_atual];
        
        // Tenta parsear a linha lida usando sscanf, de acordo com o formato CSV esperado

        // Os tamanhos (14, 99, 10) são um a menos q o tamanho do buffer no struct p garantir espaço p o '\0'
        int itens_lidos = sscanf(linha, "%d,%14[^,],%99[^,],%d,%10s",
                                 &p->id, p->cpf, p->nome, &p->idade, p->data_cadastro);

        if (itens_lidos == 5) {
            bd->quantidade_atual++; // Incrementa o contador de pacientes se a linha foi parseada com sucesso
        } else {
            fprintf(stderr, "Aviso: Linha mal formatada ou dados incompletos no CSV: '%s' (itens lidos: %d)\n", linha, itens_lidos);
            // Opcionalmente, pode-se decidir se o carregamento deve parar ou apenas pular esta linha
        }
    }

    // Verifica se o loop terminou pq o vetor de pacientes atingiu a capacidade máxima, mas ainda há mais linhas no arquivo CSV
    if (bd->quantidade_atual == MAX_PACIENTES && fgets(linha, sizeof(linha), arquivo) != NULL) {
        fprintf(stderr, "Aviso: O arquivo CSV contém mais registros que o maximo permitido (%d). Alguns registros podem nao ter sido carregados.\n", MAX_PACIENTES);
        fclose(arquivo);
        return -2;
    }

    fclose(arquivo);
    return 0;
}

// Permite ao usuário consultar pacientes por nome ou CPF (busca por prefixo)
void consultar_paciente(const BDPaciente* bd) {
    if (bd == NULL) {
        fprintf(stderr, "Erro: Banco de dados de pacientes não inicializado para consulta.\n");
        return;
    }

    int opcao_consulta;
    char termo_busca[MAX_NOME]; // Buffer p o termo de busca (nome ou CPF)

    // Loop do submenu de consulta
    while (1) {
        printf("\n[Sistema] Escolha o modo de consulta:\n");
        printf("1 - Por nome\n");
        printf("2 - Por CPF\n");
        printf("3 - Retornar ao menu principal\n");
        printf("Opção: ");

        // Lê a opção do usuário.
        if (scanf("%d", &opcao_consulta) != 1) { // Verifica se a leitura foi um número inteiro
            printf("Entrada inválida. Por favor, digite um número.\n");
            limpar_buffer_entrada();
            continue;
        }
        limpar_buffer_entrada();

        if (opcao_consulta == 3) {
            break;
        }

        if (opcao_consulta < 1 || opcao_consulta > 2) {
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }

        printf("[Sistema] Digite o %s para busca (prefixo): ", (opcao_consulta == 1) ? "nome" : "CPF");
        
        // Lê o termo de busca usando fgets p evitar overflow.
        if (fgets(termo_busca, sizeof(termo_busca), stdin) == NULL) {
            printf("Erro ao ler termo de busca.\n");
            continue; // Volta ao início do loop do submenu.
        }
        termo_busca[strcspn(termo_busca, "\n")] = 0;

        int encontrados = 0;
        imprimir_cabecalho_paciente();

        for (int i = 0; i < bd->quantidade_atual; i++) {
            const Paciente* p = &bd->pacientes[i]; 
            int corresponde = 0; 

            if (opcao_consulta == 1) {
                if (strncmp(p->nome, termo_busca, strlen(termo_busca)) == 0) {
                    corresponde = 1;
                }
            } else {
                if (strncmp(p->cpf, termo_busca, strlen(termo_busca)) == 0) {
                    corresponde = 1;
                }
            }

            if (corresponde) {
                imprimir_dados_paciente(p);
                encontrados++;
            }
        }

        if (encontrados == 0) {
            printf("Nenhum paciente encontrado com o %s informado.\n", (opcao_consulta == 1) ? "nome" : "CPF");
        }
        printf("-------------------------------------------------------------------------------\n");
    }
}

// Imprime a lista de todos os pacientes, com paginação.
void imprimir_lista_pacientes(const BDPaciente* bd) {
    if (bd == NULL) {
        fprintf(stderr, "Erro: Banco de dados de pacientes nao inicializado para impressão.\n");
        return;
    }

    printf("\n[Sistema] Imprimindo lista de pacientes...\n");
    if (bd->quantidade_atual == 0) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }

    imprimir_cabecalho_paciente(); 

    for (int i = 0; i < bd->quantidade_atual; i++) {
        imprimir_dados_paciente(&bd->pacientes[i]);
        
        if ((i + 1) % RECORDS_PER_PAGE == 0 && (i + 1) < bd->quantidade_atual) {
            printf("Pressione Enter para continuar ou Q e Enter para sair da listagem...");
            
            int ch_pag = getchar(); 

            
            if (toupper(ch_pag) == 'Q') {
                limpar_buffer_entrada();
                printf("-------------------------------------------------------------------------------\n");
                return;
            }
        
            if (ch_pag != '\n' && ch_pag != EOF) {
                 limpar_buffer_entrada();
            }

        
            if ((i + 1) < bd->quantidade_atual) { 
                 imprimir_cabecalho_paciente();
            }
        }
    }
    printf("-------------------------------------------------------------------------------\n"); // Linha separadora final.
    printf("Fim da lista de pacientes.\n");
}
