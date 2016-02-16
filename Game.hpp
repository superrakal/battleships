#pragma once
#include "Gamer.hpp"

class Game : public EventDispatcher{
    private:

        Gamer& first_gamer;
        Gamer& second_gamer;
        
        Gamer *currentGamer;
        
        Gamer *winner;
        
        bool isGameEnded;

        const size_t MAX_SHIP_SIZE = 4;
        const size_t MIN_SHIP_SIZE = 1;
    public:
        Game(Gamer& first_gamer, Gamer& second_gamer);
        
        Gamer * getCurrentGamer() const;
        Gamer * play();
        void setShips(Gamer & gamer);
        void giveShotManagement(Gamer & g);
};