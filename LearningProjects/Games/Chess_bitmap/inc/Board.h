#pragma once
#include "Utils.h"
#include <cstdint>
#include <random>

typedef unsigned long U32;
typedef unsigned long long U64;
#define C64(constantU64) constantU64##ULL

#define RANDOM_SEED 1578953456

#define a_file 0x8080808080808080
#define h_file 0x0101010101010101
#define b_file 0x4040404040404040
#define g_file 0x0202020202020202

#define hg_file h_file | g_file
#define ab_file a_file | b_file

#define rank_1 0xff00000000000000
#define rank_2 0x00ff000000000000
#define rank_7 0x000000000000ff00
#define rank_8 0x00000000000000ff

enum Color{
    white = 0,
    black
};

enum Slider{
    rook = 0,
    bishop
};

enum bitmap_enum
{
    all_pieces = 0,
    white_pieces,
    black_pieces,
    pawns,
    rooks,
    bishops,
    knights,
    queens,
    kings
};

enum Square
{
    h8 = 0, g8, f8, e8, d8, c8, b8, a8,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h1, g1, f1, e1, d1, c1, b1, a1
};

class Board
{
public:
    Board();
    Board(char* fenPosition); // TODO

    // move bitmaps
    U64 pawn_attack_mask(Square, Color);
    U64 pawn_advance_mask(Square, Color);
    U64 knight_move_mask(Square square);
    U64 king_move_mask(Square square);
    U64 bishop_move_mask(Square square);
    U64 rook_move_mask(Square square);

    U64 bishop_moves_on_the_fly(Square square, Color color);

//private:
    U64 bitmaps[9];
    U32 random_seed = RANDOM_SEED;

    //U64 magic_numbers_rook[64][4096];

    static void printBoard(U64 board);
    void printAllBoards() const;

    static uint8_t countBits(U64 board);
    static int8_t lsb_position(U64 board);

    static bool getBit(const U64 &bitmap, Square square);
    static void setBit(U64 &bitmap, Square square);
    static void clearBit(U64 &bitmap, Square square);

    U64 get_occupancy(uint8_t key, U64 attack_mask);
    U32 random_32();
    U64 random_64();
    U64 sparse_random_64();
};
