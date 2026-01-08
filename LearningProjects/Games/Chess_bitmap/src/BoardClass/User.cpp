#include "../../inc/Board.h"
#include <cstdint>
#include <string>

ReturnValue Board::parse_user_input(std::string input, Move& move)
{
    if(input == "help")
        return RETURN_HELP;
    if(input == "revert")
        return RETURN_REVERT;
    if(input == "print")
        return RETURN_PRINT;
    SquareTuple mt = parse_input_squares(input);
    Square from = std::get<0>(mt);
    Square to = std::get<1>(mt);
    Piece promoted = NoPiece;

    if(from == no_square)
        return RETURN_INVALID_INPUT;

    Move m = encode_move(from, to);
    if(is_move_promotion(m))
    {
        while(promoted < N)
        {
            print("Enter promoted piece(n, b, r, q): ");
            std::string promo;
            std::cin >> promo;

            promoted = parse_input_promoted(promo);
        }
    }

    return UserMoveCheck(move, from, to, promoted);
}

int8_t Board::parseFen(const char* fenPosition)
{
    const char* ptr = fenPosition;
    if(!ptr || strlen(fenPosition) > (64 + 7 + 14 + 1) || strlen(fenPosition) < 15)   // 64 squares + 7 slashes + null terminator + 14 chars for
                                                                                               // side to move, castling, en passant, halfmove clock, fullmove number
        return RETURN_INVALID_FEN;
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
                return RETURN_INVALID_FEN;  // invalid FEN
            i++;
        }

        ptr++;
    }

    if(countBits(bitmaps[kings]) != 2)
    {
        return RETURN_INVALID_FEN;
    }

    parseFenAdditionalInfo(ptr);

    return 0;
}

void Board::parseFenAdditionalInfo(const char* ptr)
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

// e2e4 / a7b8 Q (promotion)
int8_t Board::UserMoveInterface(Move& move)
{
    int8_t validInput = 1;
    std::string input;
    print("Enter your next move ("), print(side_to_move == white ? "w" : "b"), print(" to move) / help / revert / print \n");

    while(validInput != 0)
    {
        std::cin >> input;

        validInput = parse_user_input(input, move); // todo - user inputs just a square, return all moves legal from that square
        if(validInput == RETURN_HELP)
            printLegalMoves(), print("Enter your move: ");
        else if(validInput == RETURN_PRINT)
        {
           printAllBoards();
        }
        else if(validInput == RETURN_REVERT)
        {
            if(bots[side_to_move]) // cant revert when playing with bot - TODO
                continue;

            return RETURN_REVERT;
            
        }
        else if(validInput == RETURN_INVALID_INPUT)
            print("Invalid input, try again:\n");
        else if(validInput == RETURN_ILLEGAL_MOVE)
            print("Move is not legal, input 'help' for list of legal moves or try again:\n");
        else if(validInput == RETURN_INVALID_PROMOTION_PIECE)
            print("Invalid promotion piece input, try again:\n");
    }

    return RETURN_SUCCESS;
}

ReturnValue Board::UserMoveCheck(Move& move, Square from, Square to, Piece promoted)
{
    move = encode_move(from, to, promoted);
    if(is_move_legal(move) == false)
        return RETURN_ILLEGAL_MOVE;

    return RETURN_SUCCESS;
}

SquareTuple Board::parse_input_squares(std::string input)
{
    // expect at least "e2e4"
    if(input.size() < 4)
        return { no_square, no_square };

    Square from = RankFileToSquare(input[0], input[1]);
    Square to   = RankFileToSquare(input[2], input[3]);

    if(from == no_square || to == no_square)
        return { no_square, no_square };

    return std::make_tuple(from, to);
}

Piece Board::parse_input_promoted(std::string input)
{
    switch(tolower(input[0]))
    {
        case 'q': return Q;
        case 'r': return R;
        case 'b': return B;
        case 'n': return N;
        default:  return NoPiece;
    }

    return NoPiece;
}