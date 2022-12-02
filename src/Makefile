CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -MMD
EXEC = chess
# PLAYER = player.o human.o computer.o
# LEVELS = level.o level1.o level2.o  level3.o level4.o
# PIECES = piece.o king.o queen.o knight.o bishop.o rook.o pawn.o
OBJECTS = main.o ${PLAYER} ${LEVELS} ${PIECES}
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ../build/${EXEC}

-include ${DEPENDS}

clean:
	rm ${DEPENDS} ${OBJECTS} ${EXEC}
