#include "../inc/Tressette.h"

Tressette::Tressette(Game::Teams& _players) :
    CardsGame(_players)
{
    handSize = numPlayers;
}

void Tressette::Game()
{
    CardsGame::Game();
    int max_num_of_rounds = 40 / numPlayers;

    dealCards(10 * numPlayers);

    while(roundNumber < max_num_of_rounds)
    {
        currRound.playRound();
        updateGameResult();

        if(currRound.roundResult.playerCalledBastaId.first != -1)
        {
            LOG_INFO("Early exit due to basta call");
            break;
        }
        if(deck.getDeck().size() > 0)
            dealCards(numPlayers);
    }

    if(roundNumber < max_num_of_rounds)
    {
        teams[currRound.roundResult.winPlayerId.second].points += 1;
    }
    winTeamId = -1;
    for(auto& t : teams)
    {
        teams[t.teamId].points += t.points;
    }
}

void Tressette::setColorConstraint(Color color)
{
    currRound.moveConstraints.colorToPlay = color;
}

void Tressette::preMoveSetup(fullPlayerId playerId)
{
    auto& team = teams[playerId.second];
    if(roundNumber == 1)
    {
        int pts = AcussoCheck(PlayerId);
        team.points += pts;
        LOG_DEBUG("Acusso pts: ", pts);
    }
}

void Tressette::postMoveSetup(Move move)
{
    fullPlayerId firstToPlayInThisRound = currRound.nextToPlayIndex;
    if(currRound.moveCnt == 0)
    {
        setColorConstraint(Cards::getColor(move.card));
    }
    LOG_DEBUG("teamId", move.playerId.second, "firstToPlay:", firstToPlayInThisRound.second);
    if(move.call == ConQuestaBasta && move.playerId.second == firstToPlayInThisRound.second)
    {
        LOG_DEBUG("ConQuestaBasta", firstToPlayInThisRound);
        currRound.roundResult.playerCalledBastaId = firstToPlayInThisRound;
    }
    currRound.moveCnt++;
}

Card Tressette::StrongerCard(Card card1, Card card2)
{
    if(Cards::getColor(card1) == Cards::getColor(card2))
    {
        if(getNumberStrength(Cards::getNumber(card1)) > getNumberStrength(Cards::getNumber(card2)))
        {
            return card1;
        }
        else
        {
            return card2;
        }
    }
    else
    {
        return card1; // first card played wins
    }
}

int8_t Tressette::getNumberStrength(Number number) const
{
    switch (number) {
        case Tre:     return 10;
        case Due:      return 9;
        case Asso:       return 8;
        case Re:  return 7;
        case Cavallo:    return 6;
        case Fante:    return 5;
        case Sette:      return 4;
        case Sei:   return 3;
        case Cinque:  return 2;
        case Quattro:      return 1;
        default:      return 0;
    }
}

Points Tressette::getNumberValue(Number number)
{
    switch (number) {
        case Asso:     return Points(1, 0);
        case Tre:      
        case Due:      
        case Re:       
        case Cavallo:  
        case Fante:   return Points(0, 1);
        default:      return Points(0, 0);
    }
}

const char* Tressette::acussoToString(AcussoType a)
{
    switch (a)
    {
        case AcussoType::NapolitanaSpade:   return "Napolitana di spade";
        case AcussoType::NapolitanaCoppe:   return "Napolitana di coppe";
        case AcussoType::NapolitanaDenari:  return "Napolitana di denari";
        case AcussoType::NapolitanaBastoni: return "Napolitana di bastoni";
        case AcussoType::AssoAcusso:        return "Asso d'acusso";
        case AcussoType::DueAcusso:         return "Due d'acusso";
        case AcussoType::TreAcusso:         return "Tre d'acusso";
        case AcussoType::AssoSenzaSpade:    return "Asso senza spade";
        case AcussoType::AssoSenzaCoppe:    return "Asso senza coppe";
        case AcussoType::AssoSenzaDenari:   return "Asso senza denari";
        case AcussoType::AssoSenzaBastoni:  return "Asso senza bastoni";
        case AcussoType::DueSenzaSpade:     return "Due senza spade";
        case AcussoType::DueSenzaCoppe:     return "Due senza coppe";
        case AcussoType::DueSenzaDenari:    return "Due senza denari";
        case AcussoType::DueSenzaBastoni:   return "Due senza bastoni";
        case AcussoType::TreSenzaSpade:     return "Tre senza spade";
        case AcussoType::TreSenzaCoppe:     return "Tre senza coppe";
        case AcussoType::TreSenzaDenari:    return "Tre senza denari";
        case AcussoType::TreSenzaBastoni:   return "Tre senza bastoni";
        case AcussoType::NoAcusso:           return "Nessun acusso";
    }
    return "Acusso sconosciuto";
}

int Tressette::AcussoCheck(fullPlayerId playerId)
{
    std::vector<AcussoType> currPlayerAcussos;
    int points = 0;
    Acusso(teams[playerId.second].players[playerId.first/2].second.getDeck(), points, currPlayerAcussos);

    LOG_DEBUG("Player: ", playerId.first, " acusso points: ", points);
    for(auto& a : currPlayerAcussos)
    {
        LOG_DEBUG("\tAcusso: ", a);
    }
    Acussos.insert({playerId, currPlayerAcussos});

    return points;
}

void Tressette::Acusso(CardSet hand, int& points, std::vector<AcussoType>& Acussos)
{
    points = 0;
    points += Napolitana(hand, Acussos);
    points += SameNumberAcusso(hand, Acussos);
}

int Tressette::SameNumberAcusso(CardSet hand, std::vector<AcussoType>& Acussos)
{
    int pts = 0;
    for(int n = Asso; n <= Tre; n++)
    {
        Color senza = NoColor;
        bool flag = true;
        for(int c = Spade; c <= Bastoni; c++)
        {
            if(!Cards::isCardInCardSet(hand, Cards::makeCard(static_cast<Color>(c), static_cast<Number>(n))))
            {
                if(senza == NoColor)
                {
                    senza = static_cast<Color>(c);
                }
                else {
                    flag = false;
                    break;
                }
            }
        }
        if(flag && senza == NoColor)
        {
            pts += 4;
            switch(n)
            {
                case Asso:
                    Acussos.push_back(AssoAcusso);
                    break;
                case Due:
                    Acussos.push_back(DueAcusso);
                    break;
                case Tre:
                    Acussos.push_back(TreAcusso);
                    break;
                default:
                    break;
            }
        }
        else if(flag)
        {
            pts += 3;
            switch(n)
            {
                case Asso:
                    switch(senza)
                    {
                        case Spade:
                            Acussos.push_back(AssoSenzaSpade);
                            break;
                        case Coppe:
                            Acussos.push_back(AssoSenzaCoppe);
                            break;
                        case Denari:
                            Acussos.push_back(AssoSenzaDenari);
                            break;
                        case Bastoni:
                            Acussos.push_back(AssoSenzaBastoni);
                            break;
                        default:
                            break;
                    }
                    break;
                case Due:
                    switch(senza)
                    {
                        case Spade:
                            Acussos.push_back(DueSenzaSpade);
                            break;
                        case Coppe:
                            Acussos.push_back(DueSenzaCoppe);
                            break;
                        case Denari:
                            Acussos.push_back(DueSenzaDenari);
                            break;
                        case Bastoni:
                            Acussos.push_back(DueSenzaBastoni);
                            break;
                        default:
                            break;
                    }
                    break;
                case Tre:
                    switch(senza)
                    {
                        case Spade:
                            Acussos.push_back(TreSenzaSpade);
                            break;
                        case Coppe:
                            Acussos.push_back(TreSenzaCoppe);
                            break;
                        case Denari:
                            Acussos.push_back(TreSenzaDenari);
                            break;
                        case Bastoni:
                            Acussos.push_back(TreSenzaBastoni);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return pts;
}

int Tressette::Napolitana(CardSet hand, std::vector<AcussoType>& Acussos)
{
    int pts = 0;
    if(hand.size() < 3)
    {
        return pts;
    }
    if(Cards::isCardInCardSet(hand, Cards::makeCard(Spade, Asso)) &&
       Cards::isCardInCardSet(hand, Cards::makeCard(Spade, Due)) &&
       Cards::isCardInCardSet(hand, Cards::makeCard(Spade, Tre)))
    {
        Acussos.push_back(NapolitanaSpade);
        pts += 3;
    }
    if(Cards::isCardInCardSet(hand, Cards::makeCard(Denari, Asso)) &&
       Cards::isCardInCardSet(hand, Cards::makeCard(Denari, Due)) &&
       Cards::isCardInCardSet(hand, Cards::makeCard(Denari, Tre)))
    {
        Acussos.push_back(NapolitanaDenari);
        pts += 3;
    }
    if(Cards::isCardInCardSet(hand, Cards::makeCard(Bastoni, Asso)) &&
       Cards::isCardInCardSet(hand, Cards::makeCard(Bastoni, Due)) &&
       Cards::isCardInCardSet(hand, Cards::makeCard(Bastoni, Tre)))
    {
        Acussos.push_back(NapolitanaBastoni);
        pts += 3;
    }
    if(Cards::isCardInCardSet(hand, Cards::makeCard(Coppe, Asso)) &&
       Cards::isCardInCardSet(hand, Cards::makeCard(Coppe, Due)) &&
       Cards::isCardInCardSet(hand, Cards::makeCard(Coppe, Tre)))
    {
        Acussos.push_back(NapolitanaCoppe);
        pts += 3;
    }
    return pts;
}

bool Tressette::checkConstraints(const CardSet& hand, Card card)
{
    if(currRound.moveConstraints.colorToPlay != NoColor && Cards::getColor(card) != currRound.moveConstraints.colorToPlay)
    {
        for(int i = Asso; i <= Re; i++)
        {
            Card _card = std::make_tuple(currRound.moveConstraints.colorToPlay, static_cast<Number>(i));
            if(Cards::isCardInCardSet(hand, _card))
            {
                LOG_DEBUG("Constraint not met: played card: ");
                Cards::logCard(card);
                LOG_DEBUG("Color to play: ", currRound.moveConstraints.colorToPlay);
                LOG_DEBUG("Current hand: ");
                Cards::logCards(hand);
                return false;
            }
        }
    }

    return true;
}
