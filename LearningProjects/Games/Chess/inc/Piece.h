#pragma once
#include <vector>
#include <cstdint>

enum Color
{
    white = 0,
    black
};

class Piece
{
public:
    Piece();
    virtual ~Piece() = 0;

    virtual std::vector<uint8_t> GetPossibleMoves();
    virtual void CalculateMoves();
private:

};