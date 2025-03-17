
# 定义编译器、编译选项、目标文件和源文件
CC = gcc
CFLAGS = -Wall -Wextra
TARGET = mkfs.cblfs      # 最终可执行文件名
SRC = mkfs.cblfs.c       # 源文件
OBJ = $(SRC:.c=.o)       # 自动生成目标文件名（将 .c 替换为 .o）

# 默认目标：编译并运行
all: build run

# 仅编译
build: $(TARGET)

# 仅运行（需确保已编译）
run:
	@echo "Executing..."
	@./$(TARGET)

# 生成可执行文件：依赖 .o 文件
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Compiled,deleting object file..."
	@rm -f $(OBJ)  # 删除所有 .o 文件

# 生成 .o 文件：依赖 .c 文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJ) $(TARGET)

# 声明伪目标
.PHONY: all build run clean