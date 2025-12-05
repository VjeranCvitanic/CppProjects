#include "../inc/Board.h"

int main()
{
    Board board;
    board.printBoard(board.bitmaps[white_pieces]);

    //for(int i = 0; i < 64; i++)
      //  Board::printBoard(board.rook_move_mask((Square)i));
    
    Board::printBoard(~board.bitmaps[all_pieces] & board.rook_move_mask(e4));

    Board::printBoard(board.get_occupancy(255, board.rook_move_mask(a1)));

    Board::printBoard(board.random_64() & board.random_64() & board.random_64());
}