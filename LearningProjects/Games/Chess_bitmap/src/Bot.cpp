#include "../inc/Bot.h"
#include <climits>
#include <cstdint>
#include <vector>

Move Bot::makeAMove(Board& board)
{
    return MinMaxIn(board);

    return 0;
}

Move Bot::MinMaxIn(Board& board)
{
    int max = INT_MIN;
    Move maxMove;

    board.generate_legal_moves();
    board.addPromotionsToLegalMoves();

    std::vector<Move> copy = board.legalMoves;

    for(auto& m : copy)
    {
        board.CommitAndUpdate(m);

        int newVal = MinMax(board, false, 4);
        if(newVal == max)
        {
            if(rand() % 2 == 0) // 50%
            {
                maxMove = m;
            }
        }
        if(newVal > max)
        {
            max = newVal;
            maxMove = m;
        }
        board.RevertAndUpdate();
    }

    return maxMove;
}

int Bot::MinMax(Board& board, bool maximizer, int8_t depth)
{
    if(depth == 0)
    {
        int val = evaluatePosition(board);
        return maximizer ? val : -1*val;
    }

    board.generate_legal_moves();
    board.addPromotionsToLegalMoves();

    int max = maximizer ? INT_MIN : INT_MAX;

    std::vector<Move> copy = board.legalMoves;

    for(auto& m : copy)
    {
        board.CommitAndUpdate(m);
        int newVal = MinMax(board, !maximizer, depth - 1);
        
        max = maximizer ? ((newVal > max) ? newVal : max) : ((newVal < max) ? newVal : max);
        board.RevertAndUpdate();
    }
    
    return max;
}

int Bot::evaluatePosition(Board& board)
{
    if(board.isGameEnd())
    {
        if(board.isDraw())
            return 0;
        else
            return INT_MAX;
    }
    
    int pieces_sum = board.count_pieces_value(white) - board.count_pieces_value(black);

    return pieces_sum;
}
