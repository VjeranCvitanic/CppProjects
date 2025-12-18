#include "../../inc/Board.h"

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

Move Board::encode_move(Square from, Square to, Color side,
                        Piece moved_piece,
                        Piece captured_piece,
                        Piece promotion_piece,
                        bool is_en_passant,
                        CastlingRights castling)
{
    Move move = 0;
    move |= (static_cast<Move>(from) & 0b111111);
    move |= (static_cast<Move>(to) & 0b111111) << 6;
    move |= (static_cast<Move>(side) & 0b1) << 12;
    move |= (static_cast<Move>(moved_piece) & 0b111) << 13;
    move |= (static_cast<Move>(captured_piece) & 0b111) << 16;
    move |= (static_cast<Move>(promotion_piece) & 0b111) << 19;
    if(is_en_passant)
    {
        move |= C64(1) << 22;
    }
    else {
        move &= ~(C64(1) << 22);
    }

    move |= (static_cast<Move>(castling) & 0b1111) << 23;

    return move;
}

Move Board::encode_move(Square from, Square to, Piece promoted)
{
    return encode_move(from, to, side_to_move, get_piece_on_square(from), get_piece_on_square(to),
                        promoted, is_move_en_passant(from, to), get_castling(from, to));
}