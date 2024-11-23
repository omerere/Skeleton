#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type) 
    : name(name), type(type) 
{
}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

const string Settlement::toString() const
{
    switch (type) {
        case SettlementType::VILLAGE: 
            return "Village";
        case SettlementType::CITY: 
            return "City";
        case SettlementType::METROPOLIS: 
            return "Metropolis";
    }
}