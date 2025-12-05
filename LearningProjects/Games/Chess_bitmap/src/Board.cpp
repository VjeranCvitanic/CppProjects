#include "../inc/Board.h"
#include <cstdint>

Board::Board()
{
    bitmaps[bitmap_enum{all_pieces}] = 0xffff00000000ffff;
    bitmaps[bitmap_enum{white_pieces}] = 0xffff000000000000;
    bitmaps[bitmap_enum{black_pieces}] = bitmaps[bitmap_enum{all_pieces}] / bitmaps[bitmap_enum{white_pieces}];
    bitmaps[bitmap_enum{pawns}] = 0x00ff00000000ff00;
    bitmaps[bitmap_enum{rooks}] = 0x8100000000000081;
    bitmaps[bitmap_enum{knights}] = 0x4200000000000042;
    bitmaps[bitmap_enum{bishops}] = 0x2400000000000024;
    bitmaps[bitmap_enum{queens}] = 0x1000000000000010;
    bitmaps[bitmap_enum{kings}] = 0x0800000000000008;
}

void Board::printBoard(U64 board)
{
    print("    a b c d e f g h\n");
    print("    ---------------\n");

    for(int8_t row = 0; row <= 7; row++)
    {
        print(8 - row);
        print("|  ");
        for(int8_t col = 7; col >= 0; col--)
        {
            print((board >> (row*8 + col)) & C64(1));
            print(" ");
        }
        newLine();
    }
}

void Board::printAllBoards() const
{
    for(int i = 0; i < sizeof(bitmaps) / sizeof(U64); i++)
    {
        print(i);
        newLine();
        printBoard(bitmaps[i]);
        newLine();
        newLine();
    }
}

bool Board::getBit(const U64 &bitmap, Square square)
{
    return bitmap & (C64(1) << square);
}
void Board::setBit(U64 &bitmap, Square square)
{
    bitmap |= (C64(1) << square);
}
void Board::clearBit(U64 &bitmap, Square square)
{
    bitmap &= ~(C64(1) << square);
}

// Attacks
U64 Board::pawn_attack_mask(Square square, Color color)
{
    U64 b = 0;
    setBit(b, square);
    if(color == white)
    {
        b = ((b >> 7) & ~h_file) | ((b >> 9) & ~a_file);
    }
    else
    {
        b = ((b << 7) & ~a_file) | ((b << 9) & ~h_file);
    }

    return b;
}

U64 Board::pawn_advance_mask(Square square, Color color)
{
    U64 b = 0;
    setBit(b, square);
    if(color == white)
    {
        b = b >> 8;
        if(getBit(rank_2, square))
            b |= (b >> 8);
    }
    else
    {
        b = (b << 8);
        if(getBit(rank_7, square))
            b |= (b << 8);
    }

    return b;
}

U64 Board::knight_move_mask(Square square)
{
    U64 b = 0, moves = 0;
    setBit(b, square);

    moves |= (b << 17) & ~h_file;
    moves |= (b << 15) & ~a_file;
    moves |= (b << 10) & ~(hg_file);
    moves |= (b << 6) & ~(ab_file);

    moves |= (b >> 17) & ~a_file;
    moves |= (b >> 15) & ~h_file;
    moves |= (b >> 10) & ~(ab_file);
    moves |= (b >> 6) & ~(hg_file);

    return moves;
}

U64 Board::king_move_mask(Square square)
{
    U64 b = 0, moves = 0;
    setBit(b, square);

    moves |= (b << 8);
    moves |= (b << 1) & ~h_file;
    moves |= (b << 7) & ~a_file;
    moves |= (b << 9) & ~h_file;

    moves |= (b >> 8);
    moves |= (b >> 1) & ~a_file;
    moves |= (b >> 7) & ~h_file;
    moves |= (b >> 9) & ~a_file;

    return moves;
}

U64 Board::bishop_move_mask(Square square)
{
    U64 b = 0, moves = 0;
    setBit(b, square);

    U64 t = b;
    while(t & ~h_file)
    {
        moves |= (t >> 9);
        t >>= 9;
    }

    t = b;
    while(t & ~h_file)
    {
        moves |= (t << 7);
        t <<= 7;
    }

    t = b;
    while(t & ~a_file)
    {
        moves |= (t << 9);
        t <<= 9;
    }

    t = b;
    while(t & ~a_file)
    {
        moves |= (t >> 7);
        t >>= 7;
    }

    return moves;
}

U64 Board::rook_move_mask(Square square)
{
    U64 b = 0, moves = 0;
    setBit(b, square);

    U64 t = b;
    while(t & ~g_file)
    {
        moves |= (t >> 1);
        t >>= 1;
    }

    t = b;
    while(t & ~b_file)
    {
        moves |= (t << 1);
        t <<= 1;
    }

    t = b;
    while(t & ~rank_2)
    {
        moves |= (t << 8);
        t <<= 8;
    }

    t = b;
    while(t & ~rank_7)
    {
        moves |= (t >> 8);
        t >>= 8;
    }

    return moves;
}

uint8_t Board::countBits(U64 board)
{
    uint8_t cnt = 0;

    while(board)
    {
        cnt++;
        board &= board-1;
    }

    return cnt;
}

int8_t Board::lsb_position(U64 board)
{
    return board == 0 ? -1 : countBits((board & -board) - 1);
}

U64 Board::get_occupancy(uint8_t key, U64 attack_mask)
{
    U64 occ = 0;

    int8_t square;
    for(int cnt = 0; cnt < countBits(attack_mask); cnt++)
    {
        clearBit(attack_mask, static_cast<Square>(square));

        if(key & (C64(1) << cnt))
            occ |= (C64(1) << square);
    }

    return occ;
}

U32 Board::random_32()
{
    U32 num = random_seed;

    num ^= num << 16;
    num ^= num >> 26;
    num ^= num << 6;

    random_seed = num;

    return num;
}

U64 Board::random_64()
{
    U64 num1, num2, num3, num4;

    num1 = (U64)random_32() & 0xffff;
    num2 = (U64)random_32() & 0xffff;
    num3 = (U64)random_32() & 0xffff;
    num4 = (U64)random_32() & 0xffff;

    return num1 | (num2 << 16) | (num3 << 32) | (num4 << 48);
}

U64 Board::sparse_random_64()
{
    return random_64() & random_64() & random_64();
}

