#include "Types.h"

class CardComparator
{
public:
    virtual ~CardComparator() = default;

    virtual bool isStronger(const Card& a, const Card& b, const Card& strongColor) const = 0;
};
