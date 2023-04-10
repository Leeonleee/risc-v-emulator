TARGET = vm_riskxvii

CC = gcc

CFLAGS     = -c -Wall -Wvla -Werror -O2 -std=c11 -fno-exceptions -flto
#CFLAGS     = -c -Wall -Wvla -Werror -O0 -std=c11 -fno-exceptions -g

#ASAN_FLAGS = -fsanitize=address
SRC        = vm_riskxvii.c encoding.c initialisation.c instructions.c register_dump.c virtual_routines.c 
OBJ        = $(SRC:.c=.o)

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(ASAN_FLAGS) -o $@ $(OBJ)	

.SUFFIXES: .c .o

.c.o:
	 $(CC) $(CFLAGS) $(ASAN_FLAGS) $<

run:
	./$(TARGET)

test:
	echo what are we testing?!

tests:
	bash run_tests.sh

run_tests:
	bash run_tests.sh

clean:
	rm -f *.o *.obj $(TARGET)
