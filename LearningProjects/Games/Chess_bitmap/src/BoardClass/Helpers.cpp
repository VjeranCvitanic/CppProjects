#include "../../inc/Board.h"
#include <cstdint>

Piece Board::get_piece_on_square(Square square)
{
    for(int8_t p = P; p <= K; p++)
    {
        if(getBit(bitmaps[p], square))
        {
            return static_cast<Piece>(p);
        }
    }

    return NoPiece;
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

char Board::pieceToChar(Piece piece, Color color)
{
    char c;
    switch(piece)
    {
        case P: c = 'p'; break;
        case N: c = 'n'; break;
        case B: c = 'b'; break;
        case R: c = 'r'; break;
        case Q: c = 'q'; break;
        case K: c = 'k'; break;
        default: c = '.'; break;
    }
    if(color == white)
        c = toupper(c);
    return c;
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

U64 Board::all_pieces_bitmap()
{
    return bitmaps[white_pieces] | bitmaps[black_pieces];
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

bool Board::is_move_en_passant(Square from, Square to)
{
    if(get_piece_on_square(from) != P)
        return false;
    int8_t diff = abs(static_cast<int8_t>(from) - static_cast<int8_t>(to));
    if(((diff == 7) || (diff == 9)) && get_piece_on_square(to) == NoPiece)
        return true;
    return false;
}

Square Board::calculate_en_passant_square(Move move)
{
    Color side = decode_side_to_move_from_move(move);
    Square from = decode_square_from_move(move, true);

    if(side == white)
        return static_cast<Square>(from - 8);
    return static_cast<Square>(from + 8);
}

void Board::update_en_passant_square(Move move)
{
    if(is_move_double_pawn_push(move))
        en_passant_square = calculate_en_passant_square(move);
    else
        en_passant_square = no_square;
}

void Board::update_castling_rights(Move move)
{
    Piece moved = decode_moved_piece_from_move(move);
    Color side = decode_side_to_move_from_move(move);
    if(moved == K)
    {
        castling_rights &= (side == white) ? ~(white_kingside | white_queenside) : ~(black_kingside | black_queenside);
    }
    else if(moved == R)
    {
        Square from = decode_square_from_move(move, true);
        U64 b = 0;
        setBit(b, from);
        if(b & h_file)
            castling_rights &= (side == white) ? ~white_kingside : ~black_kingside;
        else if(b & a_file)
            castling_rights &= (side == white) ? ~white_queenside : ~black_queenside;
    }
}

void Board::update_move_count(Move move)
{
    if(decode_side_to_move_from_move(move) == white)
        return; // update only after both players make moves
    fullmove_number++;
    if(decode_captured_piece_from_move(move) != NoPiece || decode_moved_piece_from_move(move) == P)
        halfmove_clock = 0;
    else
        halfmove_clock++;
}

bool Board::is_move_double_pawn_push(Move move)
{
    if(decode_moved_piece_from_move(move) != P)
        return false;
    Square from = decode_square_from_move(move, true);
    Square to = decode_square_from_move(move, false);
    if(abs(to - from) == 16)
        return true;
    return false;
}

bool Board::is_move_promotion(Move move)
{
    Piece moved = decode_moved_piece_from_move(move);
    Square to = decode_square_from_move(move, false);

    if(moved == P && ((to <= a8) || (to >= h1)))
    {
        return true;
    }

    return false;
}

CastlingRights Board::get_castling(Square from, Square to)
{
    if(get_piece_on_square(from) != K)
        return no_castling;
    if(from == e1)
    {
        if(to == c1)
            return white_queenside;
        else if(to == g1)
            return white_kingside;
    }
    else if(from == e8)
    {
        if(to == c8)
            return black_queenside;
        else if(to == g8)
            return black_kingside;
    }

    return no_castling;
}

Move Board::blackout_promotion_info(Move move)
{
    constexpr Move PROMO_MASK = (0b111ULL << 19);

    move &= ~PROMO_MASK;
    move |= (static_cast<Move>(AnyPiece & 0b111) << 19);

    return move;
}

// test
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

