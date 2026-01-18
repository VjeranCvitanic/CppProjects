#include "../inc/HumanPlayer.h"
#include <cctype>
#include <cstdio>
#include <tuple>
#include <vector>

void HumanPlayer::PlayMove(const Moves& moves, Move& move)
{
    PlayerBase::PlayMove(moves, move);
    if(hand.getDeck().empty())
    {
        LOG_ERROR("No cards in hand to play");
        move.card = Cards::makeCard(InvalidColor, InvalidNumber);
        return;
    }

    parseInput(move);
}

void HumanPlayer::parseInput(Move& move)
{
    ReturnVal val = DEFAULT;
    std::string input;

    do
    {
        switch (val) {
            case RETURN_HELP:
                print("Enter your card: Color (S, B, D, C), Number (1 - 13)");
                if(gamePtr->gameType == TressetteGame)
                    print(" (optional: call (Busso (B), Striscio (S), QuestaBasta (Q))");
                print(" / help (to see hand) / game (to see game state)\n");
                printHand();
                break;
            case RETURN_GAME_STATE:
                gamePtr->printGameState();
                break;
            case RETURN_INVALID_INPUT:
                print("Invalid input: "), print(input), print(", please try again\n");
                break;
            case RETURN_SUCCESS:
                if(isCardInDeck(move.card))
                    return;
                break;
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

        val = parse(input, move);
    }while(true);


    LOG_ERROR("Impossible");
}

void HumanPlayer::setRoundEnd(const RoundResult& roundResult)
{
    PlayerBase::setRoundEnd(roundResult);
    print("Round ended. You (Id: "), print(playerId), print(") ");
    if(roundResult.winPlayerId == playerId)
        print("won");
    else
        print("lost");
    print(" this round (points: "), print(roundResult.points), print(").\n");
    printLines();
    getchar();
}

ReturnVal HumanPlayer::parse(std::string input, Move& move)
{
    LOG_DEBUG("input size: ", input.size());
    if(input == "help")
        return RETURN_HELP;
    if(input == "game")
        return RETURN_GAME_STATE;

    Color color;
    int _number = 0;
    Card card;
    Call call = NoCall;

    if (input.size() < 2 || input.size() > 4)
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
        if (std::isdigit(input[i]))
        {
            _number = _number * 10 + (input[i] - '0');
        }
        else
        {
            switch(std::toupper(input[i]))
            {
                case 'B' :
                    call = Busso;
                    break;
                case 'S' :
                    call = Striscio;
                    break;
                case 'Q' :
                    call = ConQuestaBasta;
                    break;
                default:
                    call = NoCall;
                    break;
            }
            i = input.size(); // exit loop
        }
    }

    card = std::make_tuple(color, Cards::intToNumber(_number));
    
    if(!checkConstraints(card))
        return RETURN_CONSTRAINT;

    move = {.card = card, .call = call};

    LOG_DEBUG("move", Cards::CardToString(card), " ", GameState::CallToString(call));

    return RETURN_SUCCESS;
}


void HumanPlayer::updateLastPlayedMove(Move move)
{
    PlayerBase::updateLastPlayedMove(move);
    print("\t\tPlayer ");
    print(move.playerId);
    print(" -> Card played: ");
    print(Cards::CardToString(move.card));
    if(move.call != NoCall)
    {
        print(" called: "), print(CardsGame::CallToString(move.call));
    }
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
    PlayerBase::startGame();
    printLines();
    print("Game started.\nYour id: ");
    print(playerId);
    if(teammateId.first != -1)
        print("\nTeammate id: "),print(teammateId.first);
    newLine();
    if(!gamePtr)
        LOG_ERROR("gamePtr is nullptr");
    gamePtr->printGameState();
    printLines();
}

void HumanPlayer::dealtCards(std::vector<std::tuple<fullPlayerId, Card>>& dCards)
{
    PlayerBase::dealtCards(dCards);
    print("Dealt cards:\n");
    for(auto& tuple : dCards)
    {
        print("Player ");
        print(std::get<0>(tuple));
        print(" draw ");
        print(Cards::CardToString(std::get<1>(tuple)));
        newLine();
    }
}