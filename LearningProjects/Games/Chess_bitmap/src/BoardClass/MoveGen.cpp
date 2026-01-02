#include "../../inc/Board.h"

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

void Board::encode_pseudo_legal_moves()
{
    encodedPseudoLegalMoves.clear();

    for(const auto& move_tuple : pseudoLegalMoves)
    {
        Square from_square = std::get<0>(move_tuple);
        Square to_square = std::get<1>(move_tuple);

        encodedPseudoLegalMoves.push_back(encode_pseudo_legal_move(from_square, to_square));
    }
}

void Board::generate_legal_moves()
{
    generate_pseudo_legal_moves();
    encode_pseudo_legal_moves();

    legalMoves.clear();

    for(std::vector<Move>::iterator it = encodedPseudoLegalMoves.begin(); it != encodedPseudoLegalMoves.end(); it++)
    {
        BoardState saved_state = save_board_state();

        make_move(*it);

        int lsb = lsb_position(bitmaps[kings] & bitmaps[side_to_move]);

        Square king_square = static_cast<Square>(lsb);
        if(!is_square_attacked(king_square, otherColor(side_to_move)))
        {
            CastlingRights castling = decode_castling_move(*it);
            if(castling != no_castling)
            {
                Square curr_square = decode_square_from_move(*it, false);
                Square through_square = (curr_square == g1) ? f1 :
                                       (curr_square == c1) ? d1 :
                                       (curr_square == g8) ? f8 : d8;
                if(is_square_attacked(through_square, otherColor(side_to_move)) || is_square_attacked(curr_square, otherColor(side_to_move)))
                {
                    restore_board_state(std::move(saved_state));
                    continue;
                }
                if((castling & castling_rights) == 0)
                {
                    restore_board_state(std::move(saved_state));
                    continue;
                }
            }
            legalMoves.push_back(*it);
        }

        restore_board_state(std::move(saved_state));
    }
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
            if((side_to_move == white && (rank_8 & (C64(1) << from_square))) || (side_to_move == black && (rank_1 & (C64(1) << from_square))))
            {
                pseudoLegalMoves.push_back(std::make_tuple(from_square, from_square)); //promotion
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
            move_mask = get_rook_attack(from_square, all_pieces_bitmap()) & ~bitmaps[side_to_move];
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

Move Board::encode_pseudo_legal_move(Square from_square, Square to_square)
{
    CastlingRights castle = no_castling;
    if(getBit(bitmaps[kings], from_square))
    {
        if(side_to_move == white)
        {
            if(from_square == e1 && to_square == g1)
                castle = white_kingside;
            else if(from_square == e1 && to_square == c1)
                castle = white_queenside;
        }
        else
        {
            if(from_square == e8 && to_square == g8)
                castle = black_kingside;
            else if(from_square == e8 && to_square == c8)
                castle = black_queenside;
        }
    }

    Piece moved_piece = get_piece_on_square(from_square);
    Piece captured_piece = from_square == to_square ? NoPiece : get_piece_on_square(to_square);

    return encode_move(from_square, to_square,
                                        side_to_move,
                                        moved_piece,
                                        captured_piece,
                                        AnyPiece, // just need something when running simulation for whether move is legal
                                        ((to_square == en_passant_square) && (moved_piece == P)),
                                        castle,
                                        is_move_double_pawn_push(from_square, to_square),
                                        castling_rights);
}

bool Board::is_move_legal(Move move)
{
    Move bm = blackout_promotion_info(move);

    for(Move& m : legalMoves)
    {
        if(bm == m)
            return true;
    }

    return false;
}