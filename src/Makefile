# Compiler and Linker
CXX = g++

# The Target Binary
TARGET = chess

# DIRECTORIES
TARGET_DIR = ../

CXXFLAGS := -std=c++14 -Wall -MMD -O3
EXEC = ${TARGET_DIR}/${TARGET}
PLAYER = player.o human.o computer.o
DISPLAY = subject.o textObserver.o
ENGINES = engine.o basicEngine.o seqEngine.o parallelEngine.o
STATE = board.o util.o move.o
OBJECTS = main.o ${PLAYER} ${ENGINES} ${STATE} ${DISPLAY}
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	@rm ${DEPENDS} ${OBJECTS} ${EXEC}

debug: CXXFLAGS += -DDEBUG -g -fsanitize=address 
debug: ${EXEC}
