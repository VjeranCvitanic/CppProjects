#include "../../inc/Board.h"
#include <cstdint>
#include <exception>

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

Board::Board(const char* fenPosition)
{
    if(parseFen(fenPosition) != 0)
    {
        print("Invalid FEN string!\n");
    }
}

Board::Board(std::string fenPosition)
{
    if(parseFen(fenPosition.c_str()) != 0)
    {
        print("Invalid FEN string!\n");
    }
}

bool Board::operator==(Board& other)
{
    bool f = true;
    for(int8_t i = 0; i < 9; i++)
    {
        if(bitmaps[i] != other.bitmaps[i])
        {
            f = false;
            print(i), print(" ");
            printBoardBits(bitmaps[i]);
            printBoardBits(other.bitmaps[i]);
            getchar();
        }
    }

    newLine();

    return f;
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

void Board::GameLoop()
{
    Move move;
    printAdditionalInfo();
    initialize();
    generate_legal_moves();

    while(!isGameEnd())
    {   
        printBoardSymbols();
        printAdditionalInfo();
        int8_t ret_val;
        do
        {
            ret_val = UserMoveInterface(move);
        }
        while(ret_val != RETURN_SUCCESS && ret_val != RETURN_REVERT);

        if(ret_val == RETURN_SUCCESS)
        {
            CommitMove(move);
            UpdateState(move);
        }
        else if(ret_val == RETURN_REVERT)
        {
            side_to_move = otherColor(side_to_move);
            update_move_count(move, false);
        }
        
        generate_legal_moves();
    }

    printBoardSymbols();

    EndGame();
}

void Board::UpdateState(Move move)
{
    side_to_move = otherColor(side_to_move);

    update_en_passant_square(move);    
    update_castling_rights(move);
    update_move_count(move, true);
}

void Board::EndGame()
{
    if(is_square_attacked(static_cast<Square>(lsb_position(bitmaps[K] & bitmaps[side_to_move])), otherColor(side_to_move)))
        print("Winner: "), print(otherColor(side_to_move));
    else
        print("Draw");
    newLine();
}

BoardState Board::save_board_state()
{
    BoardState state;
    // Implementation to save the current board state into 'state'
    memcpy(state.bitmaps, bitmaps, sizeof(bitmaps));
    state.castling_rights = castling_rights;
    state.en_passant_square = en_passant_square;
    state.side_to_move = side_to_move;
    return state;
}
void Board::restore_board_state(BoardState&& state)
{
    memcpy(bitmaps, state.bitmaps, sizeof(bitmaps));
    castling_rights = state.castling_rights;
    en_passant_square = state.en_passant_square;
    side_to_move = state.side_to_move;
}

void Board::make_move(Move move)
{
    Piece captured = decode_captured_piece_from_move(move);
    Piece moved = decode_moved_piece_from_move(move);
    Square from = decode_square_from_move(move, true);
    Square to = decode_square_from_move(move, false);
    Color side = decode_side_to_move_from_move(move);
    Piece promoted = decode_promoted_piece_from_move(move);
    bool is_en_passant = decode_is_en_passant_move(move);
    CastlingRights castling = decode_castling_move(move);

    if(captured != NoPiece)
    {
        if(is_en_passant)
        {
            if(side == white)
            {
                clearBit(bitmaps[pawns], static_cast<Square>(to + 8));
                clearBit(bitmaps[black_pieces], static_cast<Square>(to + 8));
            }
            else
            {
                clearBit(bitmaps[pawns], static_cast<Square>(to - 8));
                clearBit(bitmaps[white_pieces], static_cast<Square>(to - 8));
            }
        }
        else
        {
            clearBit(bitmaps[captured], to);
            clearBit(bitmaps[side == white ? black_pieces : white_pieces], to);
        }
    }
    
    clearBit(bitmaps[side], from);
    clearBit(bitmaps[moved], from);

    if(is_move_promotion(move))
    {
        if(promoted != NoPiece && promoted != P)
            setBit(bitmaps[promoted], to);
        else
        {
            print("Promoted piece is invalid!");
            throw(std::exception("Promoted piece is invalid!"));
        }
    }
    else
        setBit(bitmaps[moved], to);
    setBit(bitmaps[side], to);

    if(castling != no_castling)
    {
        if(castling & white_kingside)
        {
            clearBit(bitmaps[rooks], h1);
            clearBit(bitmaps[white_pieces], h1);
            setBit(bitmaps[rooks], f1);
            setBit(bitmaps[white_pieces], f1);
        }
        else if(castling & white_queenside)
        {
            clearBit(bitmaps[rooks], a1);
            clearBit(bitmaps[white_pieces], a1);
            setBit(bitmaps[rooks], d1);
            setBit(bitmaps[white_pieces], d1);
        }
        else if(castling & black_kingside)
        {
            clearBit(bitmaps[rooks], h8);
            clearBit(bitmaps[black_pieces], h8);
            setBit(bitmaps[rooks], f8);
            setBit(bitmaps[black_pieces], f8);
        }
        else if(castling & black_queenside)
        {
            clearBit(bitmaps[rooks], a8);
            clearBit(bitmaps[black_pieces], a8);
            setBit(bitmaps[rooks], d8);
            setBit(bitmaps[black_pieces], d8);
        }
    }
}

void Board::unmake_move(Move move)
{
    Piece captured = decode_captured_piece_from_move(move);
    Piece moved = decode_moved_piece_from_move(move);
    Square from = decode_square_from_move(move, true);
    Square to = decode_square_from_move(move, false);
    Color side = decode_side_to_move_from_move(move);
    Piece promoted = decode_promoted_piece_from_move(move);
    bool is_en_passant = decode_is_en_passant_move(move);
    CastlingRights castling = decode_castling_move(move);
    bool is_double_pawn_push = decode_is_double_pawn_push(move);
    int8_t prev_CR = decode_prev_castling_rights(move);

    setBit(bitmaps[moved], from);
    setBit(bitmaps[side], from);

    if(promoted != NoPiece)
        clearBit(bitmaps[promoted], to);
    else
        clearBit(bitmaps[moved], to);
    clearBit(bitmaps[side], to);
    if(captured != NoPiece)
    {
        if(is_en_passant)
        {
            if(side == white)
            {
                setBit(bitmaps[captured], static_cast<Square>(to + 8));
                setBit(bitmaps[otherColor(side)], static_cast<Square>(to + 8));
            }
            else {
                setBit(bitmaps[captured], static_cast<Square>(to - 8));
                setBit(bitmaps[otherColor(side)], static_cast<Square>(to - 8));            
            }
        }
        else {
            setBit(bitmaps[captured], to);
            setBit(bitmaps[otherColor(side)], to);
        }
    }

    castling_rights = prev_CR;
    if(is_double_pawn_push)
    {
        en_passant_square = (side == white) ? static_cast<Square>(to + 8) : static_cast<Square>(to - 8);
    }

    if(castling != no_castling)
    {
        if(castling & white_kingside)
        {
            clearBit(bitmaps[rooks], f1);
            clearBit(bitmaps[white_pieces], f1);
            setBit(bitmaps[rooks], h1);
            setBit(bitmaps[white_pieces], h1);
        }
        else if(castling & white_queenside)
        {
            clearBit(bitmaps[rooks], d1);
            clearBit(bitmaps[white_pieces], d1);
            setBit(bitmaps[rooks], a1);
            setBit(bitmaps[white_pieces], a1);
        }
        else if(castling & black_kingside)
        {
            clearBit(bitmaps[rooks], f8);
            clearBit(bitmaps[black_pieces], f8);
            setBit(bitmaps[rooks], h8);
            setBit(bitmaps[black_pieces], h8);
        }
        else if(castling & black_queenside)
        {
            clearBit(bitmaps[rooks], d8);
            clearBit(bitmaps[black_pieces], d8);
            setBit(bitmaps[rooks], a8);
            setBit(bitmaps[black_pieces], a8);
        }
    }
}

bool Board::isGameEnd()
{
    if(legalMoves.empty())
        return true;

    // todo draw

    return false;
}

bool Board::isDraw()
{
    // todo https://www.chess.com/terms/draw-chess
    return false;
}

void Board::CommitMove(Move move)
{
    make_move(move);
    ListOfMoves.push_back(move);
}

Move Board::RevertMove()
{
    if(ListOfMoves.empty())
    {
        print("No previous moves to revert!");
        return 0;
    }

    Move m = ListOfMoves.back();

    ListOfMoves.pop_back();
    unmake_move(m);
    return m;
}

void Board::PlayOutGame()
{
    Board sim;
    sim.printBoardSymbols();
    for(auto& m : ListOfMoves)
    {
        sim.make_move(m);
        sim.printBoardSymbols();
    }
}