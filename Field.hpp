#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm> 
#include <iterator>
#include "Exceptions.hpp"
#include "Dispatcher.hpp"
#include "Ship.hpp"

struct ShotCoordinates{
    size_t x = 0;
    size_t y = 0;

    ShotCoordinates(size_t shot_x, size_t shot_y) : x(shot_x), y(shot_y) {}
    
    bool operator==(const ShotCoordinates &other){
        return ((x == other.x) && (y == other.y));
    }
    
    ShotCoordinates(){}
    
    ShotCoordinates(const ShotCoordinates & other){
        x = other.x;
        y = other.y;
    }
};

enum Status{
    ghost_status = 0, missed_status, died_status // Машина состояний 
};

class Field : public EventDispatcher{
    private:
        std::vector< std::vector < Ship* > > cells;
        std::vector<Ship*> remaining_ships;
        size_t field_dimention;
        std::vector< std::vector< Status > > cellsStatus;

        
    public:
        Field(const size_t & d);
        ~Field();
        size_t getDimention() const;
        bool makeShot(const ShotCoordinates &);
        void setShip(const Coordinates &, size_t size);
        std::vector<ShotCoordinates> getArea(size_t x, size_t y) const;
        std::vector<ShotCoordinates> getShipArea(Coordinates coords, size_t size) const;
        std::vector<Ship*> getNeighboringShips(size_t x, size_t y) const;
        std::vector < std::vector <Status>> & getCellsStatus(); // Вектор векторов со статусами клеток
        std::vector < std::vector <Ship*>> & getCells(); // Вектор векторов кораблей
        std::vector <Ship*> & getRemainingShips();
};

class EnemyField{

    private:
        std::vector <std::vector <Status>> & cellsStatus;
        size_t field_dimention;

    public:
        EnemyField(std::vector< std::vector< Status > > & cellstatus, size_t dimention) : cellsStatus(cellstatus), field_dimention(dimention){};
        std::vector< std::vector< Status >> & getCellsStatus() const;
        size_t getDimention() const;
};