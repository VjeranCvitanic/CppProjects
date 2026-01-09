#include "../inc/Board.h"
#include "../inc/Bot.h"
#include <cstddef>

// clang++ .\src\Board.cpp .\src\utils.cpp .\src\main.cpp -Xlinker /STACK:0x1000000 -o .\out\chess.exe

int main()
{
    // Set UTF-8 output encoding (Windows)
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    Board board(fen[0]);

    Bot whiteBot(white);
    Bot blackBot(black);

    Bot* wptr = nullptr;//&whiteBot;
    Bot* bptr = &blackBot;

    board.setBots(wptr, bptr);

    board.GameLoop();

    // opening book todo
    // optimize botleneck todo
    // alpha beta todo
    // eval function todo
}
