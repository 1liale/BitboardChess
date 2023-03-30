# Compiler and Linker
CXX = g++

# The Target Binary
TARGET = chess

# DIRECTORIES
TARGET_DIR = ../
NNUE_LIB = nnue

CXXFLAGS := -std=c++14 -Wall -MMD -O3
EXEC = ${TARGET_DIR}/${TARGET}
PLAYER = player.o human.o computer.o
DISPLAY = subject.o textObserver.o
ENGINES = engine.o basicEngine.o seqEngine.o parallelEngine.o
STATE = board.o util.o move.o
NNUE = ${NNUE_LIB}/misc.o ${NNUE_LIB}/nnue.o
OBJECTS = main.o ${PLAYER} ${NNUE} ${ENGINES} ${STATE} ${DISPLAY}
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	@rm ${DEPENDS} ${OBJECTS} ${EXEC}

debug: CXXFLAGS += -DDEBUG -g -fsanitize=address 
debug: ${EXEC}
