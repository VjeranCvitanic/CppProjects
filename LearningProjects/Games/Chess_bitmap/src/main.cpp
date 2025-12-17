#include "../inc/Board.h"
#include <cstdint>
#include <cstdio>

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

    // todo - user move interface
    board.make_move(Board::encode_move(f8, f8, white, P, NoPiece, Q, false, no_castling));

    board.printBoardSymbols();

    board.generate_legal_moves();
    board.printLegalMoves();
}