#include "../inc/Briscola.h"
#include <cstdint>

Briscola::Briscola(Game::Players& _players) :
    CardsGame(_players)
{
    lastCard = getCard(0);
    strongColor = Cards::getColor(lastCard);
    LOG_INFO("Strong color: ", Cards::ColorToString(strongColor));
    LOG_INFO("Last card: ", Cards::CardToString(lastCard));

    handSize = HandSize;

    gameType = BriscolaGame;
}

int8_t Briscola::Game()
{
    CardsGame::Game();
    int8_t winner = 0;

    dealCards(2 * numPlayers);

    while(deck.getDeck().size() > 0)
    {
        dealCards(handSize);

        playRound();
    }

    while(currRound.roundNumber < 10)
        playRound();

    return winner;
}

Card Briscola::StrongerCard(Card card1, Card card2)
{
    Color color1 = Cards::getColor(card1);
    Color color2 = Cards::getColor(card2);
    Number number1 = Cards::getNumber(card1);
    Number number2 = Cards::getNumber(card2);

    if(color1 == color2)
    {
        if(getNumberStrength(number1) > getNumberStrength(number2))
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
        if(color2 == strongColor)
        {
            return card2;
        }
        else if(color1 == strongColor)
        {
            return card1;
        }
        else
        {
            return card1; // first card played wins
        }
    } 
}

int8_t Briscola::getNumberStrength(Number number) const
{
    switch (number) {
        case Asso:     return 10;
        case Tre:      return 9;
        case Re:       return 8;
        case Cavallo:  return 7;
        case Fante:    return 6;
        case Sette:    return 5;
        case Sei:      return 4;
        case Cinque:   return 3;
        case Quattro:  return 2;
        case Due:      return 1;
        default:      return 0;
    }
}

Points Briscola::getNumberValue(Number number)
{
    switch (number) {
        case Asso:     return Points(11, 0);
        case Tre:      return Points(10, 0);
        case Re:       return Points(4, 0);
        case Cavallo:  return Points(3, 0);
        case Fante:    return Points(2, 0);
        default:      return Points(0, 0);
    }
}

void Briscola::printGameState()
{
    printLines();
    CardsGame::printGameState();
    print("Strong color: ");
    print(Cards::ColorToString(strongColor));
    newLine();
    print("Last card: ");
    print(Cards::CardToString(lastCard));
    newLine();
    print("Round number: ");
    print(currRound.roundNumber);
    newLine();
    printLines();
}

Color Briscola::getStrongColor() const
{
    return strongColor;
}

Card Briscola::getLastCard() const
{
    return lastCard;
}

std::shared_ptr<CardsGame> Briscola::createGame(Game::Players& players)
{
    return std::make_unique<Briscola>(players);
}