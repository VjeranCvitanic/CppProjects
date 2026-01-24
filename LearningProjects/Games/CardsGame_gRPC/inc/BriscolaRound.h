#pragma once

#include "CardsRound.h"
#include "Types.h"

namespace BriscolaRound_NS
{
    class BriscolaRules : public CardsRound_NS::RoundRules
    {
    public:
        BriscolaRules();
        int8_t getNumberStrength(Number number) override;
        Points getNumberValue(Number number) override;
        Card StrongerCard(const Card& card1, const Card& card2, Color strongColor) override;
        bool IsMoveLegal(const Move&, const CardsRound_NS::RoundState& state, ReturnValue& reason) override;
    };

    class BriscolaRoundState : public CardsRound_NS::RoundState
    {
    public:
        BriscolaRoundState(Color _strongColor, int _handsize, PlayerId _nextToPlayId, CardsRound_NS::Teams _teams) :
            strongColor(_strongColor), CardsRound_NS::RoundState(_handsize, _nextToPlayId, _teams)
        {}
        Color strongColor;
    };

    class BriscolaRound : public CardsRound_NS::CardsRound
    {
    public:
        BriscolaRound(BriscolaRules& _rules, BriscolaRoundState& _state, EventEmitter& _eventEmitter);
        void preMoveSetup() override;
    };
}
