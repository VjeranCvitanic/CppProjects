#pragma once
#include "Utils.h"
#include <cstdint>
#include <random>

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

enum Color{
    white = 0,
    black = 1
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

enum Square
{
    h8 = 0, g8, f8, e8, d8, c8, b8, a8,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h1, g1, f1, e1, d1, c1, b1, a1
};

class Board
{
public:
    Board();
    Board(char* fenPosition); // TODO

    // move bitmaps
    U64 pawn_attack_mask(Square, Color);
    //U64 pawn_advance_mask(Square, Color);
    U64 knight_move_mask(Square square);
    U64 king_move_mask(Square square);
    U64 bishop_move_mask(Square square);
    U64 rook_move_mask(Square square);

    U64 bishop_moves_on_the_fly(Square square, U64 board);
    U64 rook_moves_on_the_fly(Square square, U64 board);

    void calculate_magic_numbers(SliderPiece piece_type);
    U64 calculate_magic_number(SliderPiece piece_type, Square square);

    bool is_square_attacked(Square square, Color byColor);

//private:
    U64 bitmaps[9];
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
    U64 bishop_attacks[64][1024];

    constexpr static char unicode_symbols[][7] = {
        "♔","♕","♖","♗","♘","♙",  //  WHITE k q r b k p
        "♚","♛","♜","♝","♞","♟︎"   //  BLACK k q r b k p
    };

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

    U64 get_occupancy(uint16_t key, U64 attack_mask);
    U32 random_32();
    U64 random_64();
    U64 sparse_random_64();

    Color otherColor(Color color);
};
