#include "../../inc/Board.h"


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
                    for(int8_t p = P; p <= K; p++)
                    {
                        if(getBit(bitmaps[p], sq))
                        {
                            piece =  -1 * (p-P) + (11 - c * 6);
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

void Board::printDecodedMove(Move move)
{
    Square from = decode_square_from_move(move, true);
    Square to = decode_square_from_move(move, false);
    Color side = decode_side_to_move_from_move(move);
    Piece moved_piece = decode_moved_piece_from_move(move);
    Piece promotion_piece = decode_promoted_piece_from_move(move);
    Piece captured_piece = decode_captured_piece_from_move(move);
    CastlingRights castling = decode_castling_move(move);
    bool is_en_passant = decode_is_en_passant_move(move);

    print("Move: ");
    print(SquareToRankFile(from));
    print(" -> ");
    print(SquareToRankFile(to));
    print(", Side: ");
    print(side == white ? "White" : "Black");
    print(", Moved piece: ");
    print(pieceToChar(moved_piece, side));
    if(captured_piece != NoPiece)
    {
        print(", Captured piece: ");
        print(pieceToChar(captured_piece, otherColor(side)));
    }
    if(promotion_piece != NoPiece)
    {
        promotion_piece = promotion_piece == NoPiece ? P : promotion_piece;
        print(", Promotion to: ");
        print(pieceToChar(promotion_piece, side));
    }
    if(is_en_passant)
    {
        print(", En passant");
    }
    if(castling != no_castling)
    {
        print(", Castling: ");
        if(castling & white_kingside) print("White kingside ");
        if(castling & white_queenside) print("White queenside ");
        if(castling & black_kingside) print("Black kingside ");
        if(castling & black_queenside) print("Black queenside ");
    }

    newLine();
}

void Board::printLegalMoves()
{
    print("Legal moves:\n");
    for(const auto& move : legalMoves)
    {
        printMove(move);
    }
}

void Board::printMove(Move move)
{
    print(SquareToRankFile(decode_square_from_move(move, true)));
    print(" -> ");
    print(SquareToRankFile(decode_square_from_move(move, false)));
    newLine();
}

void Board::printSquare(Square square)
{
    if(square == no_square)
    {
        print("no_square");
        return;
    }
    int8_t rank = square / 8;
    int8_t file = square % 8;

    print(static_cast<char>('h' - file));
    print(static_cast<char>('8' - rank));
}
