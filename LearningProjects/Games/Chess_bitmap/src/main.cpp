#include "../inc/Board.h"

// clang++ .\src\Board.cpp .\src\utils.cpp .\src\main.cpp -Xlinker /STACK:0x1000000 -o .\out\chess.exe

int main()
{
    // Set UTF-8 output encoding (Windows)
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    Board board(fen[0]);

    board.printAdditionalInfo();

    board.initialize();

    board.printBoardSymbols();

    board.generate_legal_moves();

    board.printLegalMoves();

    board.printBoardSymbols();

    board.UserMoveInterface("g1f2");
// todo - make sure when white plays that white pieces can move only
    board.printBoardSymbols();

    //board.generate_legal_moves();
    //board.printLegalMoves();
}