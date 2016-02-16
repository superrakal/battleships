#include "RandomGamer.hpp"

void RandomGamer::init(Field * f1, EnemyField * f2, ...){
    field = f1;
    enemy_field = f2;
    
    size_t enemy_field_dimention = enemy_field->getDimention();
    
    for (size_t i = 0; i < enemy_field_dimention; ++i){
        for (size_t j = 0; j < enemy_field_dimention; ++j){
            shoting_queue.push_back(ShotCoordinates(i, j));
        }
    }
    
    isInitializedCorrect = true;
}

Coordinates RandomGamer::getShipCoordinates(size_t size){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }

    std::srand((int)std::time(0) * std::rand());
    
    bool randomize = std::rand() % 2;
    Orientation orientation = randomize ? VERTICAL : HORIZONTAL; // случайная ориентация корабля
    
    size_t field_dimention = field->getDimention();

    size_t x,y;
    do{
        x = std::fabs((std::rand() % field_dimention) - size * orientation);
        y = std::fabs((std::rand() % field_dimention) - size * !orientation);
    } while (field->getNeighboringShips(x, y).size() != 0); // ищем координаты установки корабля
    
    return Coordinates(x,y, orientation); // возвращаем координаты и ориентацию
}

ShotCoordinates RandomGamer::make_shot(){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    size_t i = std::rand() % shoting_queue.size();
    ShotCoordinates coordinate = shoting_queue[i];
    auto iter = shoting_queue.begin() + i;
    shoting_queue.erase(iter);
    return coordinate; // стреляем по случайной клетке
}