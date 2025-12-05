#include "Utils.h"

class Piece {
public:
    Piece();
    virtual ~Piece() = 0;

    virtual std::vector<Position> GetPseudoLegalMoves();
private:
    Color color;
};