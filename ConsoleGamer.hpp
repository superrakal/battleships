#pragma once
#include "BaseGamer.hpp"

class ConsoleGamer : public Gamer{
	private:
	    View* view;
	public:
	    void init (Field * f1, EnemyField * f2, ...) override;
	    
	    Coordinates getShipCoordinates(size_t size) override;
	    ShotCoordinates make_shot() override;
	    
	    void invalidShip(const Exception & e) override;
	    void wrongShot(const Exception & e) override;
	    
    	void event_KilledEnemyShip() override;
};