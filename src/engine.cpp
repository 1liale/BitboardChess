#include "engine.h"

using namespace std;

Engine::Engine(Board *b, int depth): chessBoard{b}, depth{depth} {}
Engine::~Engine() {}