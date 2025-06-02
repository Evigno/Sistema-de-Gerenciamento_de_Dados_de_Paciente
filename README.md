# HealthSys - Sistema de Gerenciamento de Dados de Pacientes (Parte I)

## Descrição
A execução deste trabalho consiste na implementação - com sucesso, de preferência - da primeira parte de um sistema simplificado de gerenciamento de dados de pacientes para uma clínica, desenvolvido na linguagem C.
Nesta primeita parte, focamos nas funcionalidades de consulta e listagem de registros dos pacientes - que eram as exigidas até então -, os quais os dados são persistidos em um arquivo texto CSV e carregados em um vetor estático em meória para a execução do sistema.

**Disciplina:** Estrutura de Dados

**Professor:** Thiago M. Paixão

**Alunos:** Maria Eduarda Vitorino Eler, Lucas Silverio Gums

## Como Compilar e Executar
### Passos para Compilação
1.  Clone ou baixe este repositório para o seu ambiente Linux.
2.  Navegue até o diretório raiz do projeto via terminal.
3.  Execute o comando para compilar o projeto:
    ```bash
    make
    ```
    Isso gerará um executável chamado `healthsys`.

### Passos para Execução
1.  Certifique-se de que o arquivo de dados `bd_paciente.csv` está presente no mesmo diretório que o executável `healthsys`. Um arquivo de exemplo é fornecido no repositório.
2.  Execute o sistema com o seguinte comando:
    ```bash
    ./healthsys
    ```
3.  O sistema apresentará um menu com as opções disponíveis para esta Parte I:
    * `1` - Consultar paciente
    * `5` - Imprimir lista de pacientes
    * `Q` - Sair do sistema

## Estrutura do Repositório
* `main.c`: Contém a função `main`, o loop principal do programa, a interface de menu para o usuário e a coordenação das chamadas às funcionalidades do sistema.
* `bd_paciente.h`: Arquivo de cabeçalho para o Tipo Abstrato de Dados `BDPaciente`. Define a estrutura `Paciente` (representando os dados de um paciente) e a estrutura `BDPaciente` (gerenciador do banco de dados de pacientes), bem como os protótipos das funções que manipulam ambos os dados.
* `bd_paciente.c`: Arquivo de implementação do TAD `BDPaciente`. Contém a lógica para carregar pacientes do arquivo CSV, consultar pacientes por nome ou CPF e listar todos os pacientes cadastrados.
* `Makefile`: Arquivo de configuração para o utilitário `make`, que automatiza o processo de compilação do projeto no ambiente Linux.
* `bd_paciente.csv`: Arquivo texto no formato CSV que armazena os dados dos pacientes. O sistema carrega os dados deste arquivo ao ser iniciado.

## Principais TADs Utilizados
### 1. `struct Paciente`
Representa as informações de um único paciente.
Campos:
* `int id`: Número de identificador único do paciente.
* `char cpf[15]`: CPF do paciente (no formato XXX.XXX.XXX-XX).
* `char nome[100]`: Nome completo do paciente, com limite de caracteres igual a 100 - contando o caracter nulo.
* `int idade`: Idade do paciente em anos.
* `char data_cadastro[11]`: Data de cadastro do paciente no sistema (no formato AAAA-MM-DD).

### 2. `TAD BDPaciente`
Abstração para o armazenamento e manipulação dos dados dos pacientes.
* **Propósito**: Encapsular a lógica de acesso e gerenciamento da coleção de registros de pacientes, provendo uma interface clara para o restante do sistema.
* **Estrutura Interna**:
    * `Paciente pacientes[MAX_PACIENTES]`: Um vetor estático que armazena os registros dos pacientes. `MAX_PACIENTES` é uma constante pré-definida em `bd_paciente.h`.
    * `int quantidade_atual`: Contador que armazena o número de pacientes atualmente carregados no vetor.
* **Principais Funções**:
    * `BDPaciente* criar_bdpaciente()`: Aloca dinamicamente e inicializa a estrutura `BDPaciente`.
    * `int carregar_bd_do_csv(BDPaciente* bd, const char* nome_arquivo)`: Carrega os dados dos pacientes do arquivo `bd_paciente.csv` para o vetor em memória.
    * `void consultar_paciente(const BDPaciente* bd)`: Permite buscar pacientes por prefixo do nome - comparando strings - ou CPF e exibe os resultados.
    * `void imprimir_lista_pacientes(const BDPaciente* bd)`: Exibe todos os registros de pacientes armazenados, com paginação para facilitar a visualização.
    * `void liberar_bdpaciente(BDPaciente* bd)`: Libera a memória alocada para a estrutura `BDPaciente`.

## Principais Decisões de Implementação
* **Armazenamento de Dados (Parte I)**: Foi utilizado um vetor estático para armazenar os registros dos pacientes em memória. A capacidade máxima é definida pela constante `MAX_PACIENTES` em `bd_paciente.h`.

* **Alocação Dinâmica**: A estrutura `BDPaciente`, que gerencia o vetor estático e outras informações do "banco de dados", é alocada dinamicamente.

* **Leitura do Arquivo CSV**: A leitura do arquivo `bd_paciente.csv` é feita linha a linha utilizando `fgets`. O parsing de cada linha para extrair os campos (ID, CPF, Nome, Idade, Data de Cadastro) é realizado com `sscanf`. A primeira linha do CSV (cabeçalho) é ignorada.

* **Consulta de Pacientes**: A busca por nome ou CPF é implementada para funcionar com base em prefixos. Por exemplo, ao buscar por "Mari", pacientes com nomes como "Maria" ou "Mariana" seriam retornados. A função `strncmp` é utilizada para essa comparação de prefixos.

* **Listagem de Pacientes com Paginação**: Para melhorar a usabilidade ao listar um grande número de pacientes, a funcionalidade de "Imprimir lista de pacientes" implementa um sistema de paginação simples, exibindo um número definido de registros por vez.

* **Interface do Usuário**: O menu principal oferece as opções '1' para consulta, '5' para listagem e 'Q' para sair. A funcionalidade de consulta possui um submenu para escolha do critério (Nome ou CPF).

* **Modularização**: O código foi estruturado de forma modular, separando a lógica de interface com o usuário (`main.c`) da lógica de manipulação de dados (TAD `BDPaciente` em `bd_paciente.c` e `bd_paciente.h`), facilitando a manipulação e compreensão do mesmo (além de deixar o sistema organizado).

* **Tratamento de Erros e Validação**: Foram implementadas verificações básicas, como o sucesso na abertura de arquivos e na alocação de memória. A entrada do usuário no menu e nas opções de consulta também passa por validações simples. Funções auxiliares como `limpar_buffer_entrada()` são usadas para robustecer a leitura de dados do teclado.

## Cabou :DD
