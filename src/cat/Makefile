# Компилятор
CC = gcc
# Флаги компилятора
CFLAGS = -std=c11 -Wall -Wextra -Werror
# Имя исполняемого файла
TARGET = s21_cat
# Исходные файлы
SOURCES = ../common/s21_reuse.c s21_cat.c 
# Файлы для проверки .clang-format
FORMAT = s21_cat.c s21_cat.h ../common/s21_reuse.c ../common/s21_reuse.h
# Агрументы для проверки утечек памяти
MEM = -nvt ../../test/cat/test_case_1.txt

# Цель по умолчанию (первая в файле)
all: clean s21_cat

# Правило для сборки исполняемого файла
s21_cat: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) 

# Правило для очистки сгенерированных файлов
clean:
	rm -f $(TARGET)

# Фиктивная цель 'rebuild': очищает и пересобирает проект
rebuild: clean all

# Правило для тестирования функции
test: all
	../../test/cat/test_cat.sh

# Правило для проверки форматирования
clang:
	cp ../../materials/linters/.clang-format .
	clang-format  -n $(FORMAT)

# Правило для проверки утечек памяти
memtest: all
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET) $(MEM)