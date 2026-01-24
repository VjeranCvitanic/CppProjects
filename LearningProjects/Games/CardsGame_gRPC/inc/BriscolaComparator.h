#include "BriscolaRules.h"
#include "CardsComparator.h"

class BriscolaComparator : public CardComparator
{
public:
    explicit BriscolaComparator() {}

    bool isStronger(const Card& a, const Card& b, const Card& strongColor) const override
    {
        return BriscolaRules::compare(a, b, strongColor);
    }
};
