CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -MMD -O3
EXEC = ../build/chess
PLAYER = player.o human.o computer.o
DISPLAY = subject.o textObserver.o
# LEVELS = level.o level1.o level2.o  level3.o level4.o
STATE = board.o util.o move.o
OBJECTS = main.o ${PLAYER} ${LEVELS} ${STATE} ${DISPLAY}
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${DEPENDS} ${OBJECTS} ${EXEC}
