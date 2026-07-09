# Компилятор
CC = gcc
# Флаги компилятора
CFLAGS = -std=c11 -Wall -Wextra -Werror
# Имя исполняемого файла
TARGET = s21_grep
# Исходные файлы
SOURCES = ../common/s21_reuse.c s21_grep.c 
# Файлы для проверки .clang-format
FORMAT = s21_grep.c s21_grep.h ../common/s21_reuse.c ../common/s21_reuse.h
# Агрументы для проверки утечек памяти
MEM = -f ../../test/grep/patterns.txt ../../test/grep/test_case_1.txt ../../test/grep/test_case_2.txt ../../test/grep/test_case_3.txt ../../test/grep/test_case_4.txt


# Цель по умолчанию (первая в файле)
all: clean s21_grep

# Правило для сборки исполняемого файла
s21_grep: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) 

# Правило для очистки сгенерированных файлов
clean:
	rm -f $(TARGET) *.o*

# Фиктивная цель 'rebuild': очищает и пересобирает проект
rebuild: clean all

# Правило для тестирования функции
test: all
	../../test/grep/test_grep.sh

# Правило для проверки форматирования
clang:
	cp ../../materials/linters/.clang-format .
	clang-format  -n $(FORMAT)

# Правило для проверки утечек памяти
memtest: all
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET) $(MEM)