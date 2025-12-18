#include "../../inc/Board.h"

int8_t Board::parse_user_input(char* input)
{
    if(std::strcmp(input, "help") == 0)
        return 1;
    SquareTuple mt = parse_input_squares(input);
    Square from = std::get<0>(mt);
    Square to = std::get<1>(mt);
    Piece promoted = NoPiece;

    if(from == no_square)
        return -2;
    if(to == from || to == no_square)
    {
        promoted = parse_input_promoted(input);
        if(promoted == NoPiece)
            return -2;
    }

    return UserMove(from, to, promoted);
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

// e2e4 / a7b8 Q (promotion)
void Board::UserMoveInterface()
{
    int8_t validInput = 1;
    char input[7] = {};
    print("Enter your next move ("), print(side_to_move == white ? "w" : "b"), print(" to move)\n");

    while(validInput != 0)
    {
        std::cin >> input;

        validInput = parse_user_input(input); // todo - user inputs just a square, return all moves legal from that square
        if(validInput == 1)
            printLegalMoves(), print("Enter your move: ");
        else if(validInput == -2)
            print("Invalid input, try again:\n");
        else if(validInput == -1)
            print("Move is not legal, input 'help' for list of legal moves or try again:\n");
    }
}

int8_t Board::UserMove(Square from, Square to, Piece promoted)
{
    if((from == to) && (promoted == NoPiece)) // user has to provide promoted piece type if promotion is played
        return -2;
    Move move = encode_move(from, to, promoted);
    if(is_move_legal(move) == false)
        return -1;
    make_move(move);

    return 0;
}

SquareTuple Board::parse_input_squares(char* input)
{
    if(!input) 
        return { no_square, no_square };

    // expect at least "e2e4"
    if(std::strlen(input) < 4)
        return { no_square, no_square };

    Square from = RankFileToSquare(input[0], input[1]);
    Square to   = RankFileToSquare(input[2], input[3]);

    if(from == no_square || to == no_square)
        return { no_square, no_square };

    return std::make_tuple(from, to);
}

Piece Board::parse_input_promoted(char* input)
{
    if(!input) 
        return NoPiece;

    if(std::strlen(input) < 6)
        return NoPiece;

    switch(input[5])
    {
        case 'q': return Q;
        case 'r': return R;
        case 'b': return B;
        case 'n': return N;
        default:  return NoPiece;
    }

    return NoPiece;
}