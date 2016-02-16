#pragma once
#include "Game.hpp"
#include "View.hpp"
#include "GamerFactory.hpp"
#include <memory>
#include <ctime>

class GameController{
    private:
        const size_t field_dimention = 10;

        Game *game;
        
        Field* first_gamer_field;
        EnemyField* first_gamer_enemy_field;
        Gamer* first_gamer;
        std::string first_gamer_type_s;
        GamerType first_gamer_type;
        
        
        Field* second_gamer_field;
        EnemyField* second_gamer_enemy_field;
        Gamer* second_gamer;
        std::string second_gamer_type_s;
        GamerType second_gamer_type;
        
        View* view;
        
        int first_gamer_wins_count = 0, second_gamer_wins_count = 0;
        int rounds_count = 0;
        long time_start;
        long time_finish;

    public:
        GameController(std::string gamer1Type_, std::string gamer2Type_, int rounds_count);
         ~GameController(){};
        
        void initGame();
        void deInitGame();
        
        void startPlaying();
        
        GamerType getGamerType(Gamer * gamer) const;
        
        void event_ShipPlaced() const;
        void event_YourTurn() const;
        void event_ShipKilled() const;
};