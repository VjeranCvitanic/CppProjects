#pragma once
#include <iostream>
#include <string>
#include <vector>

enum MoveValue{c=0, d=1};

class player
{
public:
    player(std::string type_, int num_of_units_alive_ = 200) :
        type(type_), score(0)
    {}

    virtual ~player() = default;

    int getScore() const;
    std::string getType() const;
    std::vector<MoveValue> getPrevMoves();
    MoveValue getPrevMove() const;
    int getNumUnitsAlive() const;

    virtual void reset();
    void setScore(int);
    void saveMove(MoveValue);

    MoveValue move(std::vector<MoveValue> opp_prevMoves);
private:
    int score; // in evolution == units
    std::vector<MoveValue> prevMoves;
    std::string type;

    virtual MoveValue do_move(std::vector<MoveValue> opp_prevMoves) = 0;
};

class random : public player
{
public:
    random() :
        player("random")
    {}
    MoveValue do_move(std::vector<MoveValue> opp_prevMoves) override;
};

class deceiver : public player
{
public:
    deceiver() :
        player("deceiver")
    {}
    MoveValue do_move(std::vector<MoveValue> opp_prevMoves) override;
};

class cooperator : public player
{
public:
    cooperator() :
        player("cooperator")
    {}
    MoveValue do_move(std::vector<MoveValue> opp_prevMoves) override;
};

class tit_for_tat : public player
{
public:
    tit_for_tat() :
        player("tit_for_tat")
    {}
    MoveValue do_move(std::vector<MoveValue> opp_prevMoves) override;
};

class tit_for_two_tat : public player
{
public:
    tit_for_two_tat() :
        player("tit_for_two_tat")
    {}
    MoveValue do_move(std::vector<MoveValue> opp_prevMoves) override;
};

class grudger : public player
{
public:
    grudger() :
        player("grudger")
    {}
    MoveValue do_move(std::vector<MoveValue> opp_prevMoves) override;
    void reset() override;
private:
    bool hold_grudge = false;
};

class pavlov : public player
{
public:
    pavlov() :
        player("pavlov")
    {}
    MoveValue do_move(std::vector<MoveValue> opp_prevMoves) override;
};