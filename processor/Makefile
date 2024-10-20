CC = g++

CFLAGS = -c -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline					\
	-Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default				\
	-Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy            \
	-Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
	-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing   \
	-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

INCLUDES = ../assembler/asm.hpp ./processor.hpp ../errors.hpp ../stack/stack.hpp

SOURCES = ./main.cpp ./processor.cpp ../assembler/asm.cpp ../errors.cpp ../stack/stack.cpp

OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = proc

all: $(SOURCES) $(INCLUDES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o: $(INCLUDES) $(SOURCES)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf *.bin
	rm -rf proc
	rm -rf ../debug/dump.txt