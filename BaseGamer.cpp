#include "BaseGamer.hpp"

const EnemyField& Gamer::getEview() const{
    return *enemy_field;
}

Field& Gamer::getField() const{
    return *field;
}
