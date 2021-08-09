COMPILE_FLAGS = -Wall -Wextra -std=c11 -pedantic -g
OBJDIR = ./bin/make
OBJECTS = graphTest.o graph.o utility.o
OBJS := $(addprefix $(OBJDIR)/,graphTest.o graph.o utility.o)

graphTest: ${OBJECTS}
	gcc ${COMPILE_FLAGS} ${OBJS} -o ./bin/graphTest
graphTest.o: ./test/graphTest.c ./include/graph.h
	gcc ${COMPILE_FLAGS} -c ./test/graphTest.c -o ${OBJDIR}/graphTest.o
graph.o : ./src/graph.c ./include/graph.h
	gcc ${COMPILE_FLAGS} -c ./src/graph.c -o ${OBJDIR}/graph.o
utility.o : ./src/utility.c ./include/utility.h
	gcc ${COMPILE_FLAGS} -c ./src/utility.c -o ${OBJDIR}/utility.o

.PHONY: clean
clean:
	rm ./bin/make/*.*