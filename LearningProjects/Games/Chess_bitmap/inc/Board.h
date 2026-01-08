#pragma once
#include "Utils.h"
#include <cstdint>
#include <random>
#include <stack>
#include <string>
#include <tuple>
#include <vector>
#include <cstring>
#include <exception>

class Bot;

#define a_file 0x8080808080808080
#define h_file 0x0101010101010101
#define b_file 0x4040404040404040
#define g_file 0x0202020202020202

#define hg_file (h_file | g_file)
#define ab_file (a_file | b_file)

#define rank_1 0xff00000000000000
#define rank_2 0x00ff000000000000
#define rank_7 0x000000000000ff00
#define rank_8 0x00000000000000ff

#define _PRECOMPUTED_MAGIC_NUMBERS_

static std::vector<std::string> fen = {"r3kP1r/p2pBppN/n4n2/1pPNP2P/6P1/3P4/P1P1K3/q5b1 b kq b6 0 0\0",
    "r7/8/7R/8/8/8/8/8 b kq b6 0 0\0",
    "r3k3/8/7R/8/8/8/8/8 b kq - 0 0\0"//,
    //"8/P7/8/8/8/8/8/5k1K w k - 0 0\0"
};

/*
uint32_t move

0000 0000 0000 0000 0000 0000 0000 0000       total (32 bits)
                                11 1111       source square (6 bits)
                         1111 11              destination square (6 bits)
                       1                      side (1 bit)
                    111                       piece moved (3 bits)
                111                           captured piece (3 bits)
            11 1                              promoted piece (3 bits)
           1                                  en passant capture flag (1 bit)
      111 1                                   castling flag (4 bits)
     1                                        double pawn push (1 bit)           // for restoring board state ˇ
1111                                          previous castling rights (4 bits)
*/

enum ReturnValue
{
    RETURN_SUCCESS = 0,
    RETURN_INVALID_INPUT,
    RETURN_INVALID_PROMOTION_PIECE,
    RETURN_ILLEGAL_MOVE,
    RETURN_HELP,
    RETURN_INVALID_FEN,
    RETURN_REVERT,
    RETURN_PRINT
};

enum Color{
    white = 0,
    black = 1,
    no_color
};

enum SliderPiece{
    rook = 0,
    bishop
};

enum bitmap_enum
{
    white_pieces = white,
    black_pieces = black,
    pawns,
    knights,
    bishops,
    rooks,
    queens,
    kings
};

enum Piece{
    NoPiece = 0,
    AnyPiece = 1,
    P = pawns,
    N,
    B,
    R,
    Q,
    K
};

#define PAWN_VALUE 1
#define KNIGHT_VALUE 3
#define BISHOP_VALUE 3
#define ROOK_VALUE 5
#define QUEEN_VALUE 9
#define KING_VALUE INT_MAX

constexpr static int piece_values[8] = {
    0, 0, PAWN_VALUE, KNIGHT_VALUE, BISHOP_VALUE, ROOK_VALUE, QUEEN_VALUE, KING_VALUE
};

constexpr static char unicode_symbols[][7] = {
        "♔","♕","♖","♗","♘","♙",  //  WHITE k q r b k p
        "♚","♛","♜","♝","♞","♟︎"   //  BLACK k q r b k p
    };

enum CastlingRights{
    white_kingside  = 0b1000,
    white_queenside = 0b0100,
    black_kingside  = 0b0010,
    black_queenside = 0b0001,
    no_castling     = 0b0000
};

enum Square
{
    h8 = 0, g8, f8, e8, d8, c8, b8, a8,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h1, g1, f1, e1, d1, c1, b1, a1, no_square
};

typedef uint32_t Move;
typedef std::tuple<Square, Square> SquareTuple;

struct BoardState {
    U64 bitmaps[9];
    Color side_to_move;
    int8_t castling_rights;
    Square en_passant_square;
};

class Board
{
public:
    Board();
    Board(const char* fenPosition);
    Board(std::string fenPosition);

    bool operator==(Board&);


    std::vector<Move> ListOfMoves;

    int8_t parseFen(const char* fenPosition);
    void parseFenAdditionalInfo(const char* fenPositionAdditionalInfo);
    int8_t set_char_to_bitmap(char c, int8_t index);

    std::vector<std::tuple<Square, Square>> pseudoLegalMoves;
    std::vector<Move> legalMoves;
    std::vector<Move> encodedPseudoLegalMoves;

    // move bitmaps
    U64 pawn_attack_mask(Square, Color);
    U64 pawn_advance_moves(Square, Color);
    U64 knight_move_mask(Square square);
    U64 king_move_mask(Square square);
    U64 bishop_move_mask(Square square);
    U64 rook_move_mask(Square square);

    U64 bishop_moves_on_the_fly(Square square, U64 board);
    U64 rook_moves_on_the_fly(Square square, U64 board);

    void generate_pseudo_legal_moves();
    Move encode_pseudo_legal_move(Square from_square, Square to_square);
    void encode_pseudo_legal_moves();
    void generate_legal_moves();

    Piece get_piece_on_square(Square square);

    void calculate_magic_numbers(SliderPiece piece_type);
    U64 calculate_magic_number(SliderPiece piece_type, Square square);

    bool is_square_attacked(Square square, Color byColor);

    void make_move(Move move);
    void unmake_move(Move move);

    BoardState save_board_state();
    void restore_board_state(BoardState&& state);

    static Square decode_square_from_move(Move move, bool is_from_or_to);
    static Color decode_side_to_move_from_move(Move move);
    static Piece decode_moved_piece_from_move(Move move);
    static Piece decode_captured_piece_from_move(Move move);
    static Piece decode_promoted_piece_from_move(Move move);
    static bool decode_is_en_passant_move(Move move);
    static CastlingRights decode_castling_move(Move move);
    static int8_t decode_prev_castling_rights(Move move);
    static bool decode_is_double_pawn_push(Move move);

    static void encode_promotion_piece(Move& move, Piece promo);


    static Move encode_move(Square from, Square to, Color side,
                        Piece moved_piece,
                        Piece captured_piece = NoPiece,
                        Piece promotion_piece = NoPiece,
                        bool is_en_passant = false,
                        CastlingRights castling = no_castling,
                        bool is_double_pawn_push = false,
                        int8_t prevCR = no_castling);
    Move encode_move(Square from, Square to, Piece promotion = NoPiece);
    bool is_move_en_passant(Square from, Square to);
    bool is_move_double_pawn_push(Move move);
    bool is_move_double_pawn_push(Square from, Square to);
    static bool is_move_promotion(Move move);
    Square calculate_en_passant_square(Move move);

    void update_en_passant_square(Move move);    
    void update_castling_rights(Move move);
    void update_move_count(Move move, bool add);

    CastlingRights get_castling(Square from, Square to);
    bool is_move_legal(Move move);

    int8_t UserMoveInterface(Move& move);
    SquareTuple parse_input_squares(std::string);
    Piece parse_input_promoted(std::string);
    ReturnValue parse_user_input(std::string input, Move& move);

    static Move blackout_promotion_info(Move move);

    ReturnValue UserMoveCheck(Move& move, Square from, Square to, Piece promotion = NoPiece);

    void addPromotionsToLegalMoves();

    void CommitMove(Move move);
    Move RevertMove();

    void CommitAndUpdate(Move move);
    void RevertAndUpdate();

    void PlayOutGame();

    void GameLoop();
    void UpdateState(Move move);
    void EndGame();

    bool isDraw();
    bool isGameEnd();

    int count_pieces_value(Color c);

//private:
    U64 bitmaps[8];
    U32 random_seed = RANDOM_SEED;

    U64 magic_numbers_rook[64];
    U64 magic_numbers_bishop[64];

    uint8_t bishop_num_occ_bits[64];
    uint8_t rook_num_occ_bits[64];

    U64 pawn_attacks[2][64];
    U64 king_attacks[64];
    U64 knight_attacks[64];

    U64 bishop_masks[64];
    U64 rook_masks[64];

    U64 rook_attacks[64][4096];
    U64 bishop_attacks[64][512];

    uint8_t halfmove_clock;
    uint8_t fullmove_number;
    Color side_to_move;
    int8_t castling_rights; // 4 bits: white KQ, black kq
    Square en_passant_square;

    // Bot is forward-declared above; store pointers here so the full definition is not required in this header.
    Bot* bots[2] = { nullptr, nullptr };

    //Move LastMovePlayed;
    //all moves in a list w info about board state (does it get info move encode decode)

    constexpr static U64 magic_numbers_rook_precomputed[] =
    {
        36029351611875328ULL,
        882723120224935936ULL,
        72075323679705152ULL,
        1188954768392323200ULL,
        36033197213091840ULL,
        36030996109328768ULL,
        11817454220530942210ULL,
        612490787363487872ULL,
        1297177431244836896ULL,
        10412392726016295112ULL,
        281544233206016ULL,
        599823244098871552ULL,
        441634315902713900ULL,
        577023720007141392ULL,
        9224497971395166468ULL,
        6924847385692635201ULL,
        4647864898787737632ULL,
        18014535951060992ULL,
        577166639875760147ULL,
        282574624878592ULL,
        152226285437855744ULL,
        288653138406155264ULL,
        4647807174440913416ULL,
        288619603276398796ULL,
        9529757688592121856ULL,
        54078402449133568ULL,
        9007478432866560ULL,
        2305852909118554144ULL,
        1126056675770497ULL,
        9288687116880896ULL,
        9223389646221265433ULL,
        2594358442347004548ULL,
        44255619843200ULL,
        35186578309124ULL,
        72199774643691520ULL,
        74309980156596224ULL,
        9223515420051383296ULL,
        9223407427519579200ULL,
        576478348985763842ULL,
        9570703292498372ULL,
        10485013253365727236ULL,
        4612952935029145633ULL,
        4512407061798912ULL,
        6917637059255599112ULL,
        5193776304633184260ULL,
        612491748412784768ULL,
        2306971177417179144ULL,
        143230734827524ULL,
        9331500488534786304ULL,
        9241387606015711488ULL,
        90164793854464ULL,
        288270027290378496ULL,
        1125934300430464ULL,
        18577967089975808ULL,
        1756685338241401088ULL,
        9223376576639435264ULL,
        282300148482178ULL,
        9077568604144771ULL,
        4638971779447005185ULL,
        9232660865973495841ULL,
        9225061025899089922ULL,
        304837416958034449ULL,
        72215996760852868ULL,
        577586962001857538ULL
    };

    constexpr static U64 magic_numbers_bishop_precomputed[] =
    {
        990669111951872ULL,
        2524285363720111104ULL,
        9444053968818044952ULL,
        1165484472927296ULL,
        577025970035294729ULL,
        3461302325791950353ULL,
        27110125902758280ULL,
        578733460286411784ULL,
        11630550504720976000ULL,
        756922642967233280ULL,
        299634169757824ULL,
        10412326753715361792ULL,
        288249137642612161ULL,
        290486720578715650ULL,
        1154066135138836480ULL,
        1767663957609284624ULL,
        1125938852004352ULL,
        27092002521253381ULL,
        16044108434907264ULL,
        3378008979149057ULL,
        72622760205090816ULL,
        10088626188824086536ULL,
        4621820633720950785ULL,
        18296148968408096ULL,
        595671703468819456ULL,
        79113231356135440ULL,
        4901121493951610896ULL,
        297256267238490124ULL,
        9223653580701974528ULL,
        4616207760013410600ULL,
        6918094179100967428ULL,
        148764479470047872ULL,
        5068817336111392ULL,
        7397694620569632ULL,
        1153555132543142192ULL,
        563500250432000ULL,
        9011648841040130ULL,
        1697654551625808ULL,
        11133470682174981121ULL,
        15768097429398018ULL,
        110347298420033809ULL,
        1126459477364768ULL,
        2018175652944937985ULL,
        6918182146473728514ULL,
        2882374201413468672ULL,
        19299728961311232ULL,
        2312607240722448514ULL,
        14988895457386695170ULL,
        2308246645257273408ULL,
        83880646884196608ULL,
        72850498126608ULL,
        18014400860651536ULL,
        15008289877731117184ULL,
        14808468962245222656ULL,
        182609124627054612ULL,
        1738534802186838096ULL,
        36592607107745796ULL,
        76724336455591936ULL,
        1152956690255315968ULL,
        18585045086503940ULL,
        58274653430788ULL,
        2594215258067503616ULL,
        72427444963451410ULL,
        9042539394900032ULL
    };

#ifdef _PRECOMPUTED_MAGIC_NUMBERS_
    const U64* magic_numbers_rook_ptr = magic_numbers_rook_precomputed;
    const U64* magic_numbers_bishop_ptr = magic_numbers_bishop_precomputed;
#else
    const U64* magic_numbers_rook_ptr = magic_numbers_rook;
    const U64* magic_numbers_bishop_ptr = magic_numbers_bishop;
#endif

    void initialize();

    void initialize_attacks();
    void init_leapers_attacks();
    void init_sliders_attacks();

    void initialize_num_occ_bits();

    U64 get_bishop_attack(Square square, U64 board);
    U64 get_rook_attack(Square square, U64 board);

    U64 get_queen_attack(Square square, U64 board);

    U64 get_king_attack(Square square);
    U64 get_knight_attack(Square square);
    U64 get_pawn_attack(Square, Color color);

    static uint8_t countBits(U64 board);
    static int8_t lsb_position(U64 board);

    static bool getBit(const U64 &bitmap, Square square);
    static void setBit(U64 &bitmap, Square square);
    static void clearBit(U64 &bitmap, Square square);

    static void printBoardBits(U64 board);
    void printBoardSymbols();
    void printAllBoards() const;
    void printAdditionalInfo();
    void printLegalMoves();
    static void printDecodedMove(Move move);
    void printMove(Move move);
    void printSquare(Square square);

    static char pieceToChar(Piece piece, Color color);


    static std::string SquareToRankFile(Square square);
    Square RankFileToSquare(char file, char rank);

    U64 get_occupancy(uint16_t key, U64 attack_mask);
    U32 random_32();
    U64 random_64();
    U64 sparse_random_64();

    static Color otherColor(Color color);

    U64 all_pieces_bitmap();

    bool verify_magics(SliderPiece piece, Square square);

    bool verify_all_magics();

    void setBots(Bot* bot1, Bot* bot2);
};
