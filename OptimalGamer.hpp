#pragma once
#include "BaseGamer.hpp"

enum State{
    searching, stopped // Машина состояний  
};

class OptimalGamer : public Gamer{
    private:
        std::vector<Coordinates> borderShip;
        std::vector<ShotCoordinates> hitShip;

        ShotCoordinates previousShot;

        bool isKilledShipInPreviousShot;
        
        State state = searching;
        
        std::vector<size_t> alivingShips; // оставшиеся корабли (0 - ничего, 1 -однопалубные и тд)
        std::vector<size_t> side_ships_count; //количество кораблей на границе поля (0-лево, 1-верх и по кругу)
        std::vector<std::vector<size_t>> chance;
        
        size_t previous_side = 0;
        
    public:
        void init (Field * f1, EnemyField * f2, ...) override;
        
        Coordinates getShipCoordinates(size_t size) override;
        ShotCoordinates make_shot() override;
        
        void invalidShip(const Exception & e) override {--side_ships_count[previous_side];} // тут же удаляем неправильно поставленный корабль
        void wrongShot(const Exception & e) override {}
        
        void event_KilledEnemyShip() override;
        void event_YourTurn() override{}
        
        size_t calculateDieChance(ShotCoordinates coordinates);
        ShotCoordinates denyShip();
        
        static bool sortHitShipX(ShotCoordinates first_coordinate, ShotCoordinates second_coordinate);
        static bool sortHitShipY(ShotCoordinates first_coordinate, ShotCoordinates second_coordinate);
    };
