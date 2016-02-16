#pragma once
#include <iostream>
#include "Field.hpp"
#include <ctime>

class View{
    public:
        virtual void showMyField(Field *field) const = 0;
        virtual void showEnemyField(const EnemyField *field) const = 0;
        virtual void showGameStatistic(int first_gamer_wins_count, int second_gamer_wins_count, long time_start, long time_finish) const = 0;
        
        virtual void showMessage(std::string message) const = 0;
        
        virtual Coordinates getCoordinatesForShip(size_t size) = 0;
        virtual ShotCoordinates getCoordinatesToShoot() = 0;
        virtual ~View(){}
};

class ConsoleView : public View{
    private:
        const std::string space = " ";
        const std::string missed_shot = "@";
        const std::string died_ship_part = "x";
        const std::string ship_part = "□";
        const std::string ghost_cell = "—";

    public:
        void showMyField(Field *field) const override;
        void showEnemyField(const EnemyField *field) const override;
        void showGameStatistic(int first_gamer_wins_count, int second_gamer_wins_count, long time_start, long time_finish) const override;

        void showMessage(std::string message) const;

        Coordinates getCoordinatesForShip(size_t size) override;
        ShotCoordinates getCoordinatesToShoot() override;
};