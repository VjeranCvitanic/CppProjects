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

bool test_encode_decode_move(Square from, Square to, Color side, Piece moved, Piece captured, Piece promoted, bool enpassant, CastlingRights cr)
{
    Move EncMove = Board::encode_move(from, to, side, moved, captured, promoted, enpassant, cr);

    Piece Deccaptured = Board::decode_captured_piece_from_move(EncMove);
    Piece Decmoved = Board::decode_moved_piece_from_move(EncMove);
    Square Decfrom = Board::decode_square_from_move(EncMove, true);
    Square Decto = Board::decode_square_from_move(EncMove, false);
    Color Decside = Board::decode_side_to_move_from_move(EncMove);
    Piece Decpromoted = Board::decode_promoted_piece_from_move(EncMove);
    bool Decis_en_passant = Board::decode_is_en_passant_move(EncMove);
    CastlingRights Deccastling = Board::decode_castling_move(EncMove);

    return equal(to, Decto) && equal(from, Decfrom) && equal(side, Decside)
           && equal(moved, Decmoved) && equal(captured, Deccaptured)
           && equal(promoted, Decpromoted) && equal(enpassant,Decis_en_passant)
           && equal(cr, Deccastling);
}

double test_enc_dec_moves()
{
    int fail_cases = 0;
    int succ_cases = 0;
    for(int8_t i = 0; i < 64; i++)
    {
        Square from = static_cast<Square>(i);
        for(int8_t i = 0; i < 64; i++)
        {
            Square to = static_cast<Square>(i);
            for(int8_t i = 0; i < 2; i++)
            {
                Color side = static_cast<Color>(i);
                for(int8_t i = P; i < K; i++)
                {
                    Piece moved = static_cast<Piece>(i);
                    for(int8_t i = P; i < K; i++)
                    {
                        Piece captured = static_cast<Piece>(i);
                        for(int8_t i = P; i < K; i++)
                        {
                            Piece promoted = static_cast<Piece>(i);
                            for(int8_t i = 0; i < 2; i++)
                            {
                                bool enpassant = static_cast<bool>(i);
                                for(uint8_t i = 0; i <= 3; i++)
                                {
                                    CastlingRights cr = static_cast<CastlingRights>(pow(2, i));
                                    test_encode_decode_move(from, to, side, moved, captured, promoted, enpassant, cr) ? succ_cases++ : fail_cases++;
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

int main()
{
    print("test_enc_dec_moves success_rate: ");
    print(test_enc_dec_moves());
}