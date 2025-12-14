#include "../inc/Board.h"
#include <cstdint>
#include <cstdio>

int main()
{
    // Set UTF-8 output encoding (Windows)
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    Board board;//fen[0]);

    board.printAdditionalInfo();

    board.initialize();

    board.printBoardSymbols();

    board.generate_pseudo_legal_moves();

    board.printPseudoLegalMoves();
}