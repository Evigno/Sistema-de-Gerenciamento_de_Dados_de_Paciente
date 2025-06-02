# Compilador e flags
CC = gcc
CFLAGS = -Wall -g -std=c11 

# Arquivos fonte e arquivos objeto
SRC = main.c bd_paciente.c
OBJ = $(SRC:.c=.o)

# Nome do executável
EXEC = healthsys

all: $(EXEC)

# Regra para linkar (vincular) o executável a partir dos arquivos objeto
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Regras para compilar arquivos .c em arquivos objeto (.o)
main.o: main.c bd_paciente.h
	$(CC) $(CFLAGS) -c main.c -o main.o

bd_paciente.o: bd_paciente.c bd_paciente.h
	$(CC) $(CFLAGS) -c bd_paciente.c -o bd_paciente.o

# Alvo para limpar os arquivos gerados pela compilação (arquivos objeto e o executável)
clean:
	rm -f $(OBJ) $(EXEC)

# Alvo para executar o programa (opcional)
run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run