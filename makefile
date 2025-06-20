# Compilador
CC = gcc

# Nome do executável
TARGET = jogo

# Arquivos fonte
SRC = main.c

# Diretório de saída para os objetos
OBJ_DIR = obj

# Arquivos objeto
OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/character.o $(OBJ_DIR)/joystick.o $(OBJ_DIR)/bullet.o $(OBJ_DIR)/gun.o $(OBJ_DIR)/element.o $(OBJ_DIR)/npc.o $(OBJ_DIR)/collision.o $(OBJ_DIR)/map.o $(OBJ_DIR)/boss.o

# Flags do compilador
CFLAGS = -Wall -Werror -o  $(shell pkg-config --cflags allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5) 

# Flags do linker
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5) -lm

# Regra padrão
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Regra para compilar os arquivos objeto
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
