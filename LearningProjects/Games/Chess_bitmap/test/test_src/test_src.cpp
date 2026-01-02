#include <cstdint>
#include <iostream>
#include <typeinfo>
#include "../../inc/Board.h"
#include "../../inc/Utils.h"

// clang++ .\src\Board.cpp .\src\utils.cpp .\test\test_src\test_src.cpp -o .\test\out\test.exe

template<typename T>
bool equal(T arg1, T arg2)
{
    bool v = arg1 == arg2;
    if(v == false)
    {
        const char* t = typeid(T).name();
        print(t), print(" ");
        print("Unequal: arg1: "), print(arg1), print(", arg2: "), print(arg2);
        newLine();
    }
    return v;
}

bool equal(Board arg1, Board arg2)
{
    bool v = arg1 == arg2;
    return v;
}

void print(Board data)
{
    return;
}

bool test_encode_decode_move(Square from, Square to, Color side, Piece moved, Piece captured, Piece promoted, bool enpassant, CastlingRights cr, bool dpp, uint8_t prev_CR)
{
    Move EncMove = Board::encode_move(from, to, side, moved, captured, promoted, enpassant, cr, dpp, prev_CR);

    Piece Deccaptured = Board::decode_captured_piece_from_move(EncMove);
    Piece Decmoved = Board::decode_moved_piece_from_move(EncMove);
    Square Decfrom = Board::decode_square_from_move(EncMove, true);
    Square Decto = Board::decode_square_from_move(EncMove, false);
    Color Decside = Board::decode_side_to_move_from_move(EncMove);
    Piece Decpromoted = Board::decode_promoted_piece_from_move(EncMove);
    bool Decis_en_passant = Board::decode_is_en_passant_move(EncMove);
    CastlingRights Deccastling = Board::decode_castling_move(EncMove);
    bool Decdpp = Board::decode_is_double_pawn_push(EncMove);
    uint8_t DecprevCr = Board::decode_prev_castling_rights(EncMove);

    return equal(to, Decto) && equal(from, Decfrom) && equal(side, Decside)
           && equal(moved, Decmoved) && equal(captured, Deccaptured)
           && equal(promoted, Decpromoted) && equal(enpassant,Decis_en_passant)
           && equal(cr, Deccastling) && equal(dpp, Decdpp) && equal(prev_CR, DecprevCr);
}

double test_enc_dec_moves()
{
    int fail_cases = 0;
    int succ_cases = 0;
    for(uint8_t i = 0; i < 64; i++)
    {
        Square from = static_cast<Square>(i);
        for(uint8_t i = 0; i < 64; i++)
        {
            Square to = static_cast<Square>(i);
            for(uint8_t i = 0; i < 2; i++)
            {
                Color side = static_cast<Color>(i);
                for(uint8_t i = P; i < K; i++)
                {
                    Piece moved = static_cast<Piece>(i);
                    for(uint8_t i = P; i < K; i++)
                    {
                        Piece captured = static_cast<Piece>(i);
                        for(uint8_t i = P; i < K; i++)
                        {
                            Piece promoted = static_cast<Piece>(i);
                            for(uint8_t i = 0; i < 2; i++)
                            {
                                bool enpassant = static_cast<bool>(i);
                                for(uint8_t i = 0; i <= 3; i++)
                                {
                                    CastlingRights cr = static_cast<CastlingRights>(pow(2, i));
                                    for(uint8_t i = 0; i < 2; i++)
                                    {
                                        bool dp_push = static_cast<bool>(i);
                                        for(uint8_t i = 0; i < 16; i++)
                                        {
                                            uint8_t prevcr = ((i % 4 == 0) ? white_kingside : 0) |
                                                            ((i % 4 == 1) ? black_kingside : 0) |
                                                            ((i % 4 == 2) ? white_queenside : 0) |
                                                            ((i % 4 == 3) ? black_queenside : 0);
                                            test_encode_decode_move(from, to, side, moved, captured, promoted, enpassant, cr, dp_push, prevcr) ? succ_cases++ : fail_cases++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return succ_cases * 1.0 / (succ_cases + fail_cases);
}

double revert_test()
{
    int succ = 0, fail = 0;

    for(auto& f : fen)
    {
        print("FEN: ");
        print(f);
        newLine();
        Board b1(f);
        Board b2(f);

        b1.initialize();
        b2.initialize();

        b1.generate_legal_moves();
        b2.generate_legal_moves();

        for(auto& m : b2.legalMoves)
        {
            b1.CommitMove(m);
            if(equal(m, b1.RevertMove()))
                succ++;
            else
                fail++;

            if(equal(b1, b2))
                succ++;
            else
                fail++;
        }
    }

    return (fail + succ == 0) ? 0 : succ * 1.0 / (succ + fail);
}

int main()
{
    //print("test_enc_dec_moves success_rate: ");
    //print(test_enc_dec_moves());

    newLine();

    print("revert_test success_rate: ");

    print(revert_test());
}