#include "../inc/Board.h"
#include <cstdio>

int main()
{
    // Set UTF-8 output encoding (Windows)
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    Board board;

    board.initialize();

    print(board.is_square_attacked(d3, black));

}