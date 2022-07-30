#ifndef __SCORE_DATA_HPP__
#define __SCORE_DATA_HPP__

#include <string>

struct ScoreData
{
    int ranking;
    std::string name;
    int injureScore;
    int killScore;
};

struct InvestData
{
    int ranking;
    std::string name;
    int investScore;
};

#endif