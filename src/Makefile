CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -MMD
EXEC = ../build/chess
PLAYER = player.o human.o computer.o
# LEVELS = level.o level1.o level2.o  level3.o level4.o
# PIECES = piece.o king.o queen.o knight.o bishop.o rook.o pawn.o
OBJECTS = main.o ${PLAYER} ${LEVELS} ${PIECES} board.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${DEPENDS} ${OBJECTS} ${EXEC}
