#pragma once
#include "Field.hpp"
#include "View.hpp"
#include <ctime> 
#include <cstdarg> 

class Gamer{
    protected:
        Field *field; // поле геймера
        EnemyField *enemy_field; // поле с туманом войны врага
        bool isInitializedCorrect = false; // флаг валидной инициализации

        std::vector<ShotCoordinates> shoting_queue; // вектор выстрелов
    public:
        Gamer(){};
        virtual void init(Field *, EnemyField *, ...) = 0;
        virtual ~Gamer(){}
        
        Field & getField() const;
        const EnemyField & getEview() const; 
        
        virtual Coordinates getShipCoordinates(size_t size) = 0;
        virtual ShotCoordinates make_shot() = 0;

        virtual void invalidShip(const Exception & e){} // неправильная постановка корабля
        virtual void wrongShot(const Exception & e){} // неправильный / некорректный выстрел
        
        virtual void event_KilledEnemyShip(){} // ивент на убийство вражеского корабля
        virtual void event_YourTurn(){} // ивент на фокусировку начала входа
};