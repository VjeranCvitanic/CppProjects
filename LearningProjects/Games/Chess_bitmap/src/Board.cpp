#include "../inc/Board.h"
#include <cstdint>
#include <cstdio>
#include <ostream>

Board::Board()
{
    bitmaps[white_pieces] = 0xffff000000000000;
    bitmaps[black_pieces] = 0x000000000000ffff;
    bitmaps[pawns] = 0x00ff00000000ff00;
    bitmaps[knights] = 0x4200000000000042;
    bitmaps[bishops] = 0x2400000000000024;
    bitmaps[rooks] = 0x8100000000000081;
    bitmaps[queens] = 0x1000000000000010;
    bitmaps[kings] = 0x0800000000000008;
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

/*U64 Board::pawn_advance_mask(Square square, Color color)
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
}*/

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
    while(t & ~hg_file & ~rank_7)
    {
        moves |= (t >> 9);
        t >>= 9;
    }

    t = b;
    while(t & ~hg_file & ~rank_2)
    {
        moves |= (t << 7);
        t <<= 7;
    }

    t = b;
    while(t & ~ab_file & ~rank_2)
    {
        moves |= (t << 9);
        t <<= 9;
    }

    t = b;
    while(t & ~ab_file & ~rank_7)
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
    while(t & ~hg_file)
    {
        moves |= (t >> 1);
        t >>= 1;
    }

    t = b;
    while(t & ~ab_file)
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

U64 Board::bishop_moves_on_the_fly(Square square, U64 board)
{
    U64 moves = 0;

    U64 t = 0, temp = 0;
    setBit(temp, square);

    t = temp;
    while(t & ~h_file)
    {
        t >>= 9;
        moves |= t;
        if(t & board)
            break;
    }

    t = temp;
    while(t & ~h_file)
    {
        t <<= 7;
        moves |= t;
        if(t & board)
            break;
    }

    t = temp;
    while(t & ~a_file)
    {
        t <<= 9;
        moves |= t;
        if(t & board)
            break;
    }

    t = temp;
    while(t & ~a_file)
    {
        t >>= 7;
        moves |= t;
        if(t & board)
            break;
    }

    return moves;
}
U64 Board::rook_moves_on_the_fly(Square square, U64 board)
{
    U64 moves = 0;

    U64 t = 0, temp = 0;
    setBit(temp, square);

    t = temp; 
    while(t & ~h_file)
    {
        t >>= 1;
        moves |= t;
        if(t & board)
            break;
    }

    t = temp;
    while(t & ~a_file)
    {
        t <<= 1;
        moves |= t;
        if(t & board)
            break;
    }

    t = temp;
    while(t & ~rank_1)
    {
        t <<= 8;
        moves |= t;
        if(t & board)
            break;
    }

    t = temp;
    while(t & ~rank_8)
    {
        t >>= 8;
        moves |= t;
        if(t & board)
            break;
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

U64 Board::get_occupancy(uint16_t key, U64 attack_mask)
{
    U64 occ = 0;

    Square square;
    uint8_t num_of_bits = countBits(attack_mask);
    for(uint8_t cnt = 0; cnt < num_of_bits; cnt++)
    {
        square = static_cast<Square>(lsb_position(attack_mask));
        clearBit(attack_mask, square);

        if(key & (C64(1) << cnt))
            occ |= (C64(1) << square);
    }

    return occ;
}

U32 Board::random_32()
{
    U32 num = random_seed;

    num ^= num << 13;
    num ^= num >> 17;
    num ^= num << 5;

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

Color Board::otherColor(Color color)
{
    return color == white ? black : white;
}

void Board::calculate_magic_numbers(SliderPiece piece_type)
{
    U64* magic_ptr = (piece_type == rook) ? magic_numbers_rook : magic_numbers_bishop;
    for(int i = 0; i < 64; i++)
    {
        magic_ptr[i] = calculate_magic_number(piece_type, static_cast<Square>(i));   
    }
}

U64 Board::calculate_magic_number(SliderPiece piece_type, Square square)
{
    U64 attack_mask = ((piece_type == rook) ? rook_move_mask(square) : bishop_move_mask(square));
    uint8_t num_of_bits = countBits(attack_mask);

    U64 attacks[4096] = {0};
    U64 occupancies[4096] = {0};

    for(int i = 0; i < (1 << num_of_bits); i++)
    {
        occupancies[i] = get_occupancy(i, attack_mask);
        attacks[i] = ((piece_type == rook) ? rook_moves_on_the_fly(square, occupancies[i]) : bishop_moves_on_the_fly(square, occupancies[i]));
    }

    for(int i = 0; i <= 500000000; i++)
    {
        U64 magic_candidate = sparse_random_64();

        U64 used_attacks[4096] = {0};
        bool is_valid = true;

        for(int j = 0; (j < (1 << num_of_bits)) && is_valid; j++)
        {
            int index = (int)((occupancies[j] * magic_candidate) >> (64 - num_of_bits));

            if(used_attacks[index] == 0)
                used_attacks[index] = attacks[j];
            else if(used_attacks[index] != attacks[j])
                is_valid = false;
        }

        if(is_valid)
            return magic_candidate;
    }

    print("Failed to find a valid magic number, square: ");
    print(square);
    print(+ ", type:");
    print(piece_type);
    newLine();
    return 0;
}

void Board::init_leapers_attacks()
{
    for(int i = 0; i < 64; i++)
    {
        pawn_attacks[white][i] = pawn_attack_mask(static_cast<Square>(i),white);
        pawn_attacks[black][i] = pawn_attack_mask(static_cast<Square>(i),black);

        king_attacks[i] = king_move_mask(static_cast<Square>(i));

        knight_attacks[i] = knight_move_mask(static_cast<Square>(i));
    }
}

void Board::init_sliders_attacks()
{
    for(int i = 0; i < 64; i++)
    {
        bishop_masks[i] = bishop_move_mask(static_cast<Square>(i));
        rook_masks[i] = rook_move_mask(static_cast<Square>(i));

        for(int j = 0; j < (1 << rook_num_occ_bits[i]); j++)
        {
            U64 occ = get_occupancy(j, rook_masks[i]);
            int magic_index = (occ * magic_numbers_rook_precomputed[i]) >> (64 - rook_num_occ_bits[i]);
            rook_attacks[i][magic_index] = rook_moves_on_the_fly(static_cast<Square>(i), occ);
        }

        for(int j = 0; j < (1 << bishop_num_occ_bits[i]); j++)
        {
            U64 occ = get_occupancy(j, bishop_masks[i]);
            int magic_index = (occ * magic_numbers_bishop_precomputed[i]) >> (64 - bishop_num_occ_bits[i]);
            bishop_attacks[i][magic_index] = bishop_moves_on_the_fly(static_cast<Square>(i), occ);
        }
    }
}

void Board::initialize()
{
    initialize_num_occ_bits();
    initialize_attacks();
}

void Board::initialize_attacks()
{
    init_leapers_attacks();
    init_sliders_attacks();
}

void Board::initialize_num_occ_bits()
{
    for(int i = 0; i < 64; i++)
    {
        bishop_num_occ_bits[i] = countBits(bishop_move_mask(static_cast<Square>(i)));
        rook_num_occ_bits[i] = countBits(rook_move_mask(static_cast<Square>(i)));
    }
}

U64 Board::get_bishop_attack(Square square, U64 board)
{
    board &= bishop_masks[square];
    board *= magic_numbers_bishop_precomputed[square];
    uint16_t index = board >> (64 - bishop_num_occ_bits[square]);

    return bishop_attacks[square][index];
} 

U64 Board::get_rook_attack(Square square, U64 board)
{
    board &= rook_masks[square];
    board *= magic_numbers_rook_precomputed[square];
    uint16_t index = board >> (64 - rook_num_occ_bits[square]);

    return rook_attacks[square][index];
}

U64 Board::get_queen_attack(Square square, U64 board)
{
    return get_bishop_attack(square, board) | get_rook_attack(square, board);
}

U64 Board::get_king_attack(Square square)
{
    return king_attacks[square];
}
U64 Board::get_knight_attack(Square square)
{
    return knight_attacks[square];
}
U64 Board::get_pawn_attack(Square square, Color color)
{
    return pawn_attacks[color][square];
}

void Board::printBoardBits(U64 board)
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
        printBoardBits(bitmaps[i]);
        newLine();
        newLine();
    }
}

void Board::printBoardSymbols()
{
    print("    a b c d e f g h\n");
    print("    ---------------\n");
    
    for(int8_t row = 0; row <= 7; row++)
    {
        print(8 - row);
        print("|  ");
        for(int8_t col = 7; col >= 0; col--)
        {
            int8_t piece = -1;

            for(int8_t c = white_pieces; c <= black_pieces; c++)
            {
                Square sq = static_cast<Square>(row*8 + col);
                if(getBit(bitmaps[c], sq))
                {
                    for(int8_t p = 0; p < 6; p++)
                    {
                        if(getBit(bitmaps[p+pawns], sq))
                        {
                            piece =  -1 * p + (11 - c * 6);
                            break;
                        }
                    }
                }
            }

            if(piece == -1)
                print(". ");
            else
            {
                print(unicode_symbols[piece]);
                print(" ");
            }
        }
        newLine();
    }
    newLine();
}

bool Board::is_square_attacked(Square square, Color byColor)
{
    if(bitmaps[pawns] & bitmaps[byColor] & get_pawn_attack(square, otherColor(byColor)))
        return true;
    if((bitmaps[knights] & bitmaps[byColor]) & get_knight_attack(square))
        return true;
    if((bitmaps[kings] & bitmaps[byColor]) & get_king_attack(square))
        return true;
    U64 bishop_attacks = get_bishop_attack(square, bitmaps[white_pieces] | bitmaps[black_pieces]);
    if((bitmaps[bishops] & bitmaps[byColor]) & bishop_attacks)
        return true;
    U64 rook_attacks = get_rook_attack(square, bitmaps[white_pieces] | bitmaps[black_pieces]);
    if((bitmaps[rooks] & bitmaps[byColor]) & rook_attacks)
        return true;
    if((bitmaps[queens] & bitmaps[byColor]) & (bishop_attacks | rook_attacks))
        return true;

    return false;
}