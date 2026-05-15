# Nome do executável final
TARGET = programa

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Encontra automaticamente todos os arquivos .c nas subpastas
SRCS = main.c $(wildcard */*.c)

# Transforma a lista de .c em uma lista de .o (arquivos objeto)
OBJS = $(SRCS:.c=.o)

# Encontra automaticamente todas as subpastas para incluir os .h
INCLUDES = -I. $(patsubst %/,-I%,$(dir $(wildcard */)))

# Regra principal para gerar o executável
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Regra para compilar os arquivos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpar os arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)