CC = gcc
CFLAGS = -Wall -g
INCLUDE_DIR = ./include
SRC_DIR = ./src

# Kaynak dosyaları
SRC_FILES = $(SRC_DIR)/memory_manager.c $(SRC_DIR)/utils.c $(SRC_DIR)/main.c
OBJ_FILES = $(SRC_DIR)/memory_manager.o $(SRC_DIR)/utils.o $(SRC_DIR)/main.o

# Hedef dosya
TARGET = memory_manager_app

$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)
