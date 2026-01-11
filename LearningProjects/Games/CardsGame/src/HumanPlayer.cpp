#include "../inc/HumanPlayer.h"
#include <cctype>
#include <cstdio>
#include <tuple>
#include <vector>

Card HumanPlayer::PlayCard(Hand playedHand)
{
    if(hand.cards.empty())
    {
        LOG_ERROR("No cards in hand to play");
        return Cards::makeCard(InvalidColor, InvalidNumber);
    }

    Card playedCard = parseInput();
    hand.cards.erase(std::remove(hand.cards.begin(), hand.cards.end(), playedCard), hand.cards.end());
    return playedCard;
}

Card HumanPlayer::parseInput()
{
    print("Enter your card: Color (S, B, D, C), Number (1 - 13) / help (to see hand) / game (to see game state)\n");
    ReturnVal val = DEFAULT;
    Card card = std::make_tuple(InvalidColor, InvalidNumber);
    std::string input;

    do
    {
        switch (val) {
            case RETURN_HELP:
                printHand();
                break;
            case RETURN_GAME_STATE:
                gamePtr->printGameState();
                break;
            case RETURN_INVALID_INPUT:
                print("Invalid input: "), print(input), print(", please try again\n");
                break;
            case RETURN_SUCCESS:
                if(isCardInHand(card))
                    return card;
            case RETURN_INVALID_CARD:
                print("Invalid card: "), print(input), print("\nHand: "), printHand(), print("please try again\n");
                break;
            case RETURN_CONSTRAINT:
                print("Constraint has not been met! Please try again\n");
                break;
            default:
                break;
        }

        std::cin >> input;
        getchar();

        val = parse(input, card);
    }while(true);


    LOG_ERROR("Impossible");
    return std::make_tuple(InvalidColor, InvalidNumber);
}

void HumanPlayer::setRoundEnd(bool winner, Points roundValue)
{
    setRoundEndDefault(winner, roundValue);
    print("Round ended. You (Id: "), print(playerId), print(") ");
    if(winner)
        print("won");
    else
        print("lost");
    print(" this round (points: "), print(roundValue), print(").\n");
    printLines();
    getchar();
}

ReturnVal HumanPlayer::parse(std::string input, Card& card)
{
    if(input == "help")
        return RETURN_HELP;
    if(input == "game")
        return RETURN_GAME_STATE;

    Color color;
    int _number = 0;

    if (input.size() < 2 || input.size() > 3)
    {
        return RETURN_INVALID_INPUT;
    } 

    switch(std::toupper(input[0]))
    {
        case 'S' :
            color = Spade;
            break;
        case 'D' :
            color = Denari;
            break;
        case 'B' :
            color = Bastoni;
            break;
        case 'C' :
            color = Coppe;
            break;
        default:
            return RETURN_INVALID_INPUT;
    }

    for (size_t i = 1; i < input.size(); ++i) {
        if (!std::isdigit(input[i]))
            return RETURN_INVALID_INPUT;
        _number = _number * 10 + (input[i] - '0');
    }

    card = std::make_tuple(color, Cards::intToNumber(_number));
    
    if(!checkConstraints(card))
        return RETURN_CONSTRAINT;

    return RETURN_SUCCESS;
}


void HumanPlayer::updateLastPlayedCard(Card playedCard, int playerId)
{
    PlayerBase::updateLastPlayedCard(playedCard, playerId);
    print("\t\tPlayer ");
    print(playerId + 1);
    print(" -> Card played: ");
    print(Cards::CardToString(playedCard));
    newLine();
}

void HumanPlayer::startNewRound()
{
    PlayerBase::startNewRound();
    printLines();
    print("New round started.\nYour hand:");
    printHand();
    printLines();
}


void HumanPlayer::startGame()
{
    printLines();
    print("Game started.\nYour id: ");
    print(playerId);
    if(teammateId != -1)
        print("\nTeammate id: "),print(teammateId);
    newLine();
    if(gamePtr->getGameType() == BriscolaGame)
    {
        print("Strong color: ");
        print(Cards::ColorToString(gamePtr->getStrongColor()));
        newLine();
        print("Last card: ");
        print(Cards::CardToString(gamePtr->getLastCard()));
        newLine();
    }
    printLines();
}

void HumanPlayer::dealtCards(std::vector<std::tuple<PlayerBase*, Card>>& dCards)
{
    PlayerBase::dealtCards(dCards);
    print("Dealt cards:\n");
    for(auto& tuple : dCards)
    {
        print("Player ");
        print(std::get<0>(tuple)->getPlayerId());
        print(" draw ");
        print(Cards::CardToString(std::get<1>(tuple)));
        newLine();
    }
}