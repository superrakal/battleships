#include <cstddef>

#pragma once

enum Orientation{
    HORIZONTAL = 0, VERTICAL
};

struct Coordinates{
    size_t x = 0;
    size_t y = 0;
    Orientation orientation = HORIZONTAL;
    Coordinates(size_t x, size_t y, Orientation orientation) : x(x), y(y), orientation(orientation) {}
    Coordinates() {}
};

class Ship{
    private:
        size_t size;
        size_t hit = 0;
        Coordinates coordinates;
        
    public:
        Ship(size_t s, Coordinates coordinates) :size(s), coordinates(coordinates){}
        size_t getSize() const;
        Coordinates getCoordinates() const;
        void shoot();
        size_t getHit() const;
        bool isDead() const;
};