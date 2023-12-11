.PHONY = all build clean
BUILDER = clang-17
BUILDER_ARGS = -O0 -Wall -Werror -Wextra -std=c17


build: clean test

InterfaceRoutine.o: ./Interface/InterfaceRoutine.c
	$(BUILDER) $(BUILDER_ARGS) -g -c -o .bin/InterfaceRoutine.o ./Interface/InterfaceRoutine.c

Interface.o: ./Interface/Interface.c
	$(BUILDER) $(BUILDER_ARGS) -g -c -o .bin/Interface.o ./Interface/Interface.c

MineField.o: ./MineField/MineField.c
	$(BUILDER) $(BUILDER_ARGS) -g -c -o .bin/MineField.o ./MineField/MineField.c

test.o: test.c
	$(BUILDER) $(BUILDER_ARGS) -g -c -o .bin/test.o test.c

test: InterfaceRoutine.o Interface.o MineField.o test.o
	$(BUILDER) $(BUILDER_ARGS) -g -o .bin/test .bin/*.o

clean:
	rm -rf .bin/*.o
