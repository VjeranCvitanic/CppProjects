#include "../inc/Board.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
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

    halfmove_clock = 0;
    fullmove_number = 0;
    side_to_move = white;
    castling_rights = white_kingside | white_queenside | black_kingside | black_queenside;
    en_passant_square = no_square;
}

Board::Board(char* fenPosition)
{
    if(parseFen(fenPosition) != 0)
    {
        print("Invalid FEN string!\n");
    }
}

int8_t Board::parseFen(char* fenPosition)
{
    char* ptr = fenPosition;
    if(!ptr || strlen(fenPosition) > (64 + 7 + 14 + 1) || strlen(fenPosition) < 15)   // 64 squares + 7 slashes + null terminator + 14 chars for
                                                                                               // side to move, castling, en passant, halfmove clock, fullmove number
        return -1;
    for(int i = 0; i <= 63;)
    {
        int8_t ascii_from_0 = *ptr - '0';
        if(ascii_from_0 >= 1 && ascii_from_0 <= 8) // if digit
            i += ascii_from_0;
        else if (*ptr == '/')
        {}
        else   // if alphabetical
        {
            if(set_char_to_bitmap(*ptr, i) != 0)
                return -1;  // invalid FEN
            i++;
        }

        ptr++;
    }

    parseFenAdditionalInfo(ptr);

    return 0;
}

void Board::parseFenAdditionalInfo(char* ptr)
{
    int8_t cnt = 0;
    ptr++; // skip space after board layout
    castling_rights = 0;

    while(*ptr != '\0')
    {
        if(*ptr == ' ')
        {
            cnt++;
        }
        else if(cnt == 0) // side to move
        {
            side_to_move = (*ptr == 'w') ? white : black;
        }
        else if(cnt == 1) // castling rights
        {
            if(*ptr == 'K')
                castling_rights |= white_kingside;
            else if(*ptr == 'Q')
                castling_rights |= white_queenside;
            else if(*ptr == 'k')
                castling_rights |= black_kingside;
            else if(*ptr == 'q')
                castling_rights |= black_queenside;
        }
        else if(cnt == 2) // en passant square
        {
            en_passant_square = RankFileToSquare(*ptr, *(ptr + 1));
            if(en_passant_square != no_square)
                ptr++; // skip rank character
        }
        else if(cnt == 3) // halfmove clock
        {
            halfmove_clock = 0;
            while (isdigit(*ptr)) {
                halfmove_clock = halfmove_clock * 10 + (*ptr - '0');
                ptr++;
            }
            ptr--;
        }
        else if(cnt == 4) // fullmove number
        {
            fullmove_number = 0;
            while (isdigit(*ptr)) {
                fullmove_number = fullmove_number * 10 + (*ptr - '0');
                ptr++;
            }
            ptr--;
        }
        ptr++;
    }
}

int8_t Board::set_char_to_bitmap(char c, int8_t index)
{
    int8_t row = index / 8;
    int8_t col = index % 8;
    Square square = static_cast<Square>(row * 8 + (7-col));
    switch (tolower(c)) {
        case 'p':
            setBit(bitmaps[pawns], square);
            break;
        case 'n':
            setBit(bitmaps[knights], square);
            break;
        case 'b':
            setBit(bitmaps[bishops], square);
            break;
        case 'r':
            setBit(bitmaps[rooks], square);
            break;
        case 'q':
            setBit(bitmaps[queens], square);
            break;
        case 'k':
            setBit(bitmaps[kings], square);
            break;
        default:
            return -1;  // invalid FEN
    }
    if(isupper(c))
        setBit(bitmaps[white_pieces], square);
    else
        setBit(bitmaps[black_pieces], square);
    return 0;
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

// advance moves
U64 Board::pawn_advance_moves(Square square, Color color)
{
    U64 b = 0;
    setBit(b, square);
    if(color == white)
    {
        if(!getBit(all_pieces_bitmap(), static_cast<Square>(square - 8)))
        {
            b = b >> 8;
            if(getBit(rank_2, square) && !getBit(all_pieces_bitmap(), static_cast<Square>(square - 16)))
                b |= (b >> 8);
        }
    }
    else
    {
        if(!getBit(all_pieces_bitmap(), static_cast<Square>(square + 8)))
        {
            b = b << 8;
            if(getBit(rank_7, square) && !getBit(all_pieces_bitmap(), static_cast<Square>(square + 16)))
                b |= (b << 8);
        }
    }

    clearBit(b, square);

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
    uint8_t num_of_bits = ((piece_type == rook) ? rook_num_occ_bits[square] : bishop_num_occ_bits[square]);

    U64 attacks[4096] = {0};
    U64 occupancies[4096] = {0};

    int n = 1 << num_of_bits;

    for(int i = 0; i < n; i++)
    {
        occupancies[i] = get_occupancy(i, attack_mask);
        attacks[i] = ((piece_type == rook) ? rook_moves_on_the_fly(square, occupancies[i]) : bishop_moves_on_the_fly(square, occupancies[i]));
    }

    for(int i = 0; i <= 5000000; i++)
    {
        U64 magic_candidate = sparse_random_64();

        U64 used_attacks[4096] = {0};
        bool is_valid = true;

        int k = 1 << num_of_bits;

        for(int j = 0; j < k && is_valid; j++)
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
    std::exception e("Magic num fail.");
    print("Failed to find a valid magic number, square: ");
    print(square);
    print(+ ", type:");
    print(piece_type);
    newLine();
    throw(e);
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

        int n = 1 << rook_num_occ_bits[i];

        for(int j = 0; j < n; j++)
        {
            U64 occ = get_occupancy(j, rook_masks[i]);
            int magic_index = (occ * magic_numbers_rook_ptr[i]) >> (64 - rook_num_occ_bits[i]);
            rook_attacks[i][magic_index] = rook_moves_on_the_fly(static_cast<Square>(i), occ);
        }

        n = 1 << bishop_num_occ_bits[i];
        for(int j = 0; j < n; j++)
        {
            U64 occ = get_occupancy(j, bishop_masks[i]);
            int magic_index = (occ * magic_numbers_bishop_ptr[i]) >> (64 - bishop_num_occ_bits[i]);
            bishop_attacks[i][magic_index] = bishop_moves_on_the_fly(static_cast<Square>(i), occ);
        }
    }
}

void Board::initialize()
{
    initialize_num_occ_bits();
#ifndef _PRECOMPUTED_MAGIC_NUMBERS_
    calculate_magic_numbers(rook);
    calculate_magic_numbers(bishop);
#endif
    initialize_attacks();
#ifndef _PRECOMPUTED_MAGIC_NUMBERS_
    if(!verify_all_magics())
    {
        print("Magic number verification failed!\n");
        throw(std::exception("Magic number verification failed!"));
    }
#endif
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
    board *= magic_numbers_bishop_ptr[square];
    uint16_t index = board >> (64 - bishop_num_occ_bits[square]);

    return bishop_attacks[square][index];
} 

U64 Board::get_rook_attack(Square square, U64 board)
{
    board &= rook_masks[square];
    board *= magic_numbers_rook_ptr[square];
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
    print("\n    a b c d e f g h\n");
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
    print("    ---------------\n");
    print("    a b c d e f g h\n");
}

void Board::printAdditionalInfo()
{
    print("Side to move: ");
    print(side_to_move == white ? "White\n" : "Black\n");

    print("Castling rights: ");
    if(castling_rights & 0b1000) print("K");
    if(castling_rights & 0b0100) print("Q");
    if(castling_rights & 0b0010) print("k");
    if(castling_rights & 0b0001) print("q");
    if(castling_rights == 0) print("-");
    newLine();

    print("En passant square: ");
    if(en_passant_square != no_square)
    {
        print(SquareToRankFile(en_passant_square));
        newLine();
    }
    else
    {
        print("-\n");
    }

    print("Halfmove clock: ");
    print(halfmove_clock);
    newLine();

    print("Fullmove number: ");
    print(fullmove_number);
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
    U64 bishop_attacks = get_bishop_attack(square, all_pieces_bitmap());
    if((bitmaps[bishops] & bitmaps[byColor]) & bishop_attacks)
        return true;
    U64 rook_attacks = get_rook_attack(square, all_pieces_bitmap());
    if((bitmaps[rooks] & bitmaps[byColor]) & rook_attacks)
        return true;
    if((bitmaps[queens] & bitmaps[byColor]) & (bishop_attacks | rook_attacks))
        return true;

    return false;
}

U64 Board::all_pieces_bitmap()
{
    return bitmaps[white_pieces] | bitmaps[black_pieces];
}

void Board::generate_pseudo_legal_moves()
{
    pseudoLegalMoves.clear();

    U64 original_side_bitmap = bitmaps[side_to_move];
    int8_t n = countBits(original_side_bitmap);

    for(int8_t i = 0; i < n; i++)
    {
        Square from_square = static_cast<Square>(lsb_position(original_side_bitmap));
        clearBit(original_side_bitmap, from_square);

        U64 move_mask = 0;

        if(getBit(bitmaps[pawns], from_square))
        {
            move_mask = pawn_advance_moves(from_square, side_to_move) | (pawn_attack_mask(from_square, side_to_move) & (bitmaps[otherColor(side_to_move)]));
            
            if(en_passant_square != no_square)
            {
                if(pawn_attack_mask(en_passant_square, otherColor(side_to_move)) & (C64(1) << from_square))
                    move_mask |= (C64(1) << en_passant_square);
            }
            if((side_to_move == white && rank_8 & (C64(1) << from_square)) || (side_to_move == black && rank_1 & (C64(1) << from_square)))
            {
                pseudoLegalMoves.push_back(std::make_tuple(from_square, no_square));
            }
        }
        else if(getBit(bitmaps[knights], from_square))
        {
            move_mask = knight_move_mask(from_square) & ~bitmaps[side_to_move];
        }
        else if(getBit(bitmaps[bishops], from_square))
        {
            move_mask = get_bishop_attack(from_square, all_pieces_bitmap()) & ~bitmaps[side_to_move];
        }
        else if(getBit(bitmaps[rooks], from_square))
        {
            move_mask = get_rook_attack(from_square, all_pieces_bitmap());//<& ~bitmaps[side_to_move];
        }
        else if(getBit(bitmaps[queens], from_square))
        {
            move_mask = get_queen_attack(from_square, all_pieces_bitmap()) & ~bitmaps[side_to_move];
        }
        else if(getBit(bitmaps[kings], from_square))
        {
            move_mask = king_move_mask(from_square) & ~bitmaps[side_to_move];
            if(side_to_move == white)
            {
                if((castling_rights & white_kingside) && 
                   !getBit(all_pieces_bitmap(), f1) && 
                   !getBit(all_pieces_bitmap(), g1))
                {
                    move_mask |= (C64(1) << g1);
                }
                if((castling_rights & white_queenside) && 
                   !getBit(all_pieces_bitmap(), b1) && 
                   !getBit(all_pieces_bitmap(), c1) && 
                   !getBit(all_pieces_bitmap(), d1))
                {
                    move_mask |= (C64(1) << c1);
                }
            }
            else {
                if((castling_rights & black_kingside) && 
                   !getBit(all_pieces_bitmap(), f8) && 
                   !getBit(all_pieces_bitmap(), g8))
                {
                    move_mask |= (C64(1) << g8);
                }
                if((castling_rights & black_queenside) && 
                   !getBit(all_pieces_bitmap(), b8) && 
                   !getBit(all_pieces_bitmap(), c8) && 
                   !getBit(all_pieces_bitmap(), d8))
                {
                    move_mask |= (C64(1) << c8);
                }
            }
        }

        int k = countBits(move_mask);

        for(int8_t j = 0; j < k; j++)
        {
            Square to_square = static_cast<Square>(lsb_position(move_mask));
            clearBit(move_mask, to_square);

            pseudoLegalMoves.push_back(std::make_tuple(from_square, to_square));
        }
    }
}

void Board::printPseudoLegalMoves()
{
    print("Pseudo-legal moves:\n");
    for(const auto& move : pseudoLegalMoves)
    {
        Square from = std::get<0>(move);
        Square to = std::get<1>(move);

        print(SquareToRankFile(from));
        print(" -> ");
        print(SquareToRankFile(to));
        newLine();
    }
}

std::string Board::SquareToRankFile(Square square)
{
    if(Square::no_square == square)
        return "-";
    char file = 'h' - (square % 8);
    char rank = '8' - (square / 8);
    std::string result;
    result += file;
    result += rank;
    return result;
}

Square Board::RankFileToSquare(char file, char rank)
{
    int8_t f = 'h' - file;
    int8_t r = '8' - rank;
    if(f < 0 || f > 7 || r < 0 || r > 7)
        return no_square;
    return static_cast<Square>(r * 8 + f);
}

bool Board::verify_magics(SliderPiece piece, Square square)
{
    U64 mask = (piece == rook) ? rook_masks[square] : bishop_masks[square];
    int bits = (piece == rook) ? rook_num_occ_bits[square] : bishop_num_occ_bits[square];

    int size = 1 << bits;

    for (int i = 0; i < size; i++)
    {
        U64 occ = get_occupancy(i, mask);

        U64 slow = (piece == rook) ? rook_moves_on_the_fly(square, occ) : bishop_moves_on_the_fly(square, occ);

        U64 index =
            (occ * ((piece == rook) ? magic_numbers_rook_ptr[square] : magic_numbers_bishop_ptr[square])) >>
            (64 - bits);

        U64 fast = piece == rook ? rook_attacks[square][index] : bishop_attacks[square][index];

        if (slow != fast)
        {
            print(piece == rook ? "Rook" : "Bishop");
            print("MAGIC FAIL @ ");
            print(SquareToRankFile(square));
            newLine();

            print("occ:");
            newLine();
            printBoardBits(occ);

            print("slow:");
            newLine();
            printBoardBits(slow);

            print("fast:");
            newLine();
            printBoardBits(fast);

            return false;
        }
    }
    return true;
}

bool Board::verify_all_magics()
{
    for(int i = 0; i < 64; i++)
    {
        if(!verify_magics(rook, static_cast<Square>(i)))
            return false;
        if(!verify_magics(bishop, static_cast<Square>(i)))
            return false;
    }
    return true;
}

