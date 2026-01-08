#include "../../inc/Board.h"
#include <cstdint>

Square Board::decode_square_from_move(Move move, bool is_from)
{
    return static_cast<Square>(is_from ? (move & 0x3f) : ((move >> 6) & 0x3f));
}
Color Board::decode_side_to_move_from_move(Move move)
{
    return static_cast<Color>((move >> 12) & 0x1);
}
Piece Board::decode_moved_piece_from_move(Move move)
{
    return static_cast<Piece>((move >> 13) & 0x7);
}
Piece Board::decode_captured_piece_from_move(Move move)
{
    return static_cast<Piece>((move >> 16) & 0x7);
}
Piece Board::decode_promoted_piece_from_move(Move move)
{
    return static_cast<Piece>((move >> 19) & 0x7);
}
bool Board::decode_is_en_passant_move(Move move)
{
    return (move >> 22) & 0x1;
}
CastlingRights Board::decode_castling_move(Move move)
{
    return static_cast<CastlingRights>((move >> 23) & 0b1111);
}
bool Board::decode_is_double_pawn_push(Move move)
{
    return (move >> 27) & 0x1;
}
int8_t Board::decode_prev_castling_rights(Move move)
{

    return static_cast<CastlingRights>((move >> 28) & 0b1111);
}

void Board::encode_promotion_piece(Move& move, Piece promo)
{
    constexpr Move PROMO_MASK = (0b111ULL << 19);

    move &= ~PROMO_MASK;
    move |= (static_cast<Move>(promo) & 0b111) << 19;
}


Move Board::encode_move(Square from, Square to, Color side,
                        Piece moved_piece,
                        Piece captured_piece,
                        Piece promotion_piece,
                        bool is_en_passant,
                        CastlingRights castling,
                        bool is_double_pawn_push,
                        int8_t prevCR)
{
    Move move = 0;
    move |= (static_cast<Move>(from) & 0b111111);
    move |= (static_cast<Move>(to) & 0b111111) << 6;
    move |= (static_cast<Move>(side) & 0b1) << 12;
    move |= (static_cast<Move>(moved_piece) & 0b111) << 13;
    move |= (static_cast<Move>(captured_piece) & 0b111) << 16;
    encode_promotion_piece(move, promotion_piece);
    if(is_en_passant)
    {
        move |= C64(1) << 22;
    }
    else {
        move &= ~(C64(1) << 22);
    }

    move |= (static_cast<Move>(castling) & 0b1111) << 23;

    if(is_double_pawn_push)
        move |= C64(1) << 27;
    else
        move &= ~(C64(1) << 27);

    move |= (static_cast<Move>(prevCR) & 0b1111) << 28;

    return move;
}

Move Board::encode_move(Square from, Square to, Piece promoted)
{
    return encode_move(from, to, side_to_move, get_piece_on_square(from), get_piece_on_square(to),
                        promoted, is_move_en_passant(from, to), get_castling(from, to), is_move_double_pawn_push(from, to), castling_rights);
}