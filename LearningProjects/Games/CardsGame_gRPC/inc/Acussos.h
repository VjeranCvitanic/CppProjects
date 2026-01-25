#pragma once

#include "Types.h"
#include <string>

namespace Acussos
{
    int AcussoCheck(const CardSet& hand, AcussosMap& acussos);
    void CalculateAcussoPoints(const CardSet& hand, int& points, std::vector<AcussoType>& Acussos);
    int Napolitana(const CardSet& hand, std::vector<AcussoType>& Acussos);
    int SameNumberAcusso(const CardSet& hand, std::vector<AcussoType>& Acussos);

    std::string AcussoToString(AcussoType a);
};