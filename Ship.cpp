#include "Ship.hpp"

size_t Ship::getSize() const{
    return Ship::size;
}

size_t Ship::getHit() const{
    return Ship::hit;
}

Coordinates Ship::getCoordinates() const{
    return Ship::coordinates;
}

bool Ship::isDead() const{
    return (Ship::hit == Ship::size);
}

void Ship::shoot(){
    ++hit;
}