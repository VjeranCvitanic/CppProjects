#include "../inc/Tressette.h"

Tressette::Tressette(Game::Players& _players) :
    CardsGame(_players)
{
    gameType = TressetteGame;
    handSize = numPlayers;
}

int8_t Tressette::Game()
{
    CardsGame::Game();
    int8_t winner = 0;

    dealCards(10 * numPlayers);

    while(deck.getDeck().size() > 0)
    {
        playRound();
        dealCards(numPlayers);
    }

    while(currRound.roundNumber < 40 / (numPlayers))
        playRound();

    return winner;
}

void Tressette::setColorConstraint(Color color)
{
    moveConstraints.colorToPlay = color;
}

void Tressette::playRound()
{
    InitRound();
    CardSet playedHand;
    firstCardPlayedInRoundColor = InvalidColor;
    for(int i = currRound.nextToPlayIndex; i < currRound.nextToPlayIndex + numPlayers; i++)
    {
        if(currRound.roundNumber == 1)
        {
            int pts = AcussoCheck(players[i%numPlayers].playerPtr);
            players[i%numPlayers].points += pts;
            LOG_DEBUG("Acusso pts: ", pts);
        }
        setColorConstraint(firstCardPlayedInRoundColor);
        Card playedCard = players[i%numPlayers].playerPtr->PlayCard(playedHand);
        if(firstCardPlayedInRoundColor == InvalidColor)
        {
            firstCardPlayedInRoundColor = Cards::getColor(playedCard);
        }
        LOG_INFO("Player ", i%numPlayers + 1, " played: ", Cards::CardToString(playedCard));
        playedHand.push_back(playedCard);
        informPlayers(playedCard, i % numPlayers);
    }

    Card roundWinner;
    int8_t winnerPos = (HandWinner(playedHand, roundWinner) + currRound.nextToPlayIndex) % numPlayers;
    informPlayers(playedHand, roundWinner, winnerPos);

    currRound.nextToPlayIndex = winnerPos;

    LOG_INFO("Round winner card: ", Cards::CardToString(roundWinner), "player: ", winnerPos + 1, "player points: ", players[winnerPos].points);
}

void Tressette::printGameState()
{
    printLines();
    CardsGame::printGameState();
    print("First color in round: ");
    print(Cards::ColorToString(firstCardPlayedInRoundColor));
    newLine();
    print("Round number: ");
    print(currRound.roundNumber);
    newLine();
    for(int i = 0; i < players.size(); i++)
    {
        print("Player ");
        print(i + 1);
        print(" acussos: ");
        if(!Acussos.empty())
            printAcussos(std::get<1>(Acussos[i]));
        print("\n");
    }
    printLines();
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


void Tressette::printAcussos(std::vector<AcussoType> acussos)
{
    if(acussos.empty())
        return;
    for(auto& a : acussos)
        print(acussoToString(a));
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

int Tressette::AcussoCheck(PlayerBase* player)
{
    std::vector<AcussoType> accussos;
    int points = 0;
    Acusso(player->GetHand(), points, accussos);

    LOG_DEBUG("Player: ", +player->getPlayerId(), " acusso points: ", points);
    for(auto& a : accussos)
    {
        LOG_DEBUG("\tAcusso: ", a);
    }
    Acussos.push_back(std::make_tuple(player, accussos));

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
        Color senza = InvalidColor;
        bool flag = true;
        for(int c = Spade; c <= Bastoni; c++)
        {
            if(!Cards::isCardInCardSet(hand, Cards::makeCard(static_cast<Color>(c), static_cast<Number>(n))))
            {
                if(senza == InvalidColor)
                {
                    senza = static_cast<Color>(c);
                }
                else {
                    flag = false;
                    break;
                }
            }
        }
        if(flag && senza == InvalidColor)
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
    if(moveConstraints.colorToPlay != InvalidColor && Cards::getColor(card) != moveConstraints.colorToPlay)
    {
        for(int i = Asso; i <= Re; i++)
        {
            Card _card = std::make_tuple(moveConstraints.colorToPlay, static_cast<Number>(i));
            if(Cards::isCardInCardSet(hand, _card))
            {
                LOG_DEBUG("Constraint not met: played card: ");
                Cards::logCard(card);
                LOG_DEBUG("Color to play: ", moveConstraints.colorToPlay);
                LOG_DEBUG("Current hand: ");
                Cards::logCards(hand);
                return false;
            }
        }
    }

    return true;
}

void Tressette::InformDealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dealtCards)
{
    for(auto& p : players)
    {
        p.playerPtr->dealtCards(dealtCards);
    }
}

std::shared_ptr<CardsGame> Tressette::createGame(Game::Players& players)
{
    return std::make_unique<Tressette>(players);
}
