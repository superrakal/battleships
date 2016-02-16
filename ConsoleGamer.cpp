#include "ConsoleGamer.hpp"

void ConsoleGamer::init(Field * f1, EnemyField * f2, ...){
    field = f1;
    enemy_field = f2;

    size_t enemy_field_dimention = enemy_field->getDimention();

    for (size_t i = 0; i < enemy_field_dimention; ++i){
        for (size_t j = 0; j < enemy_field_dimention; ++j){
            shoting_queue.push_back(ShotCoordinates(i, j));
        }
    }

    va_list vl;
    va_start(vl, f2);
    view = va_arg(vl, View*);
    va_end(vl);
    isInitializedCorrect = true;
}

Coordinates ConsoleGamer::getShipCoordinates(size_t size){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    if (size == 4){ // ввод корректен например a4 v
        view->showMyField(field);
    }
    return view->getCoordinatesForShip(size);
}

ShotCoordinates ConsoleGamer::make_shot(){
    if (!isInitializedCorrect){ // таким образом обеспечиваем безопасность вызова
        throw std::logic_error("Gamer uninitialized");
    }
    return view->getCoordinatesToShoot();
}

void ConsoleGamer::invalidShip(const Exception &e){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    view->showMessage("You can't place a ship here. Try again");
}

void ConsoleGamer::wrongShot(const Exception &e){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    view->showMessage("You can't shoot here. Try again");
}

void ConsoleGamer::event_KilledEnemyShip(){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    view->showMessage("Enemy's ship is killed!");
}