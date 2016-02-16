#pragma once
#include "BaseGamer.hpp"

enum ShipPlacement{
	undefined = 0, manual, optimal
};

enum CrawlerState{
    in_search, stop // Машина состояний  
};

class AssistedGamer : public Gamer{
	private:
	    View* view;
	    ShipPlacement ship_placement = undefined;

	    std::vector<Coordinates> borderShip;
        std::vector<ShotCoordinates> hitShip;

        ShotCoordinates previousShot;

        bool isKilledShipInPreviousShot;
        
        CrawlerState state = in_search;
        
        std::vector<size_t> alivingShips; // оставшиеся корабли (0 - ничего, 1 -однопалубные и тд)
        std::vector<size_t> side_ships_count; //количество кораблей на границе поля (0-лево, 1-верх и по кругу)
        std::vector<std::vector<size_t>> chance;

        std::string bestShootStr(ShotCoordinates coordinates);
        Coordinates getShipCoordinatesFromConsole(size_t size);
        Coordinates getShipCoordinatesOptimal(size_t size);

        size_t previous_side = 0;
	    
	public:
	    void init (Field * f1, EnemyField * f2, ...) override;
	    
	    Coordinates getShipCoordinates(size_t size) override;
	    ShotCoordinates make_shot() override;
	    ShotCoordinates getBestShot();
    	size_t calculateDieChance(ShotCoordinates coordinates);
        ShotCoordinates denyShip();

	    void invalidShip(const Exception & e) override;
	    void wrongShot(const Exception & e) override;
	    
    	void event_KilledEnemyShip() override;
	 	static bool sortHitShipX(ShotCoordinates first_coordinate, ShotCoordinates second_coordinate);
        static bool sortHitShipY(ShotCoordinates first_coordinate, ShotCoordinates second_coordinate);
};