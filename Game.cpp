#include "Game.hpp"

Game::Game(Gamer& g1, Gamer& g2) :first_gamer(g1), second_gamer(g2){
    isGameEnded = false;
    winner = nullptr;
    currentGamer = &first_gamer;
}

Gamer* Game::getCurrentGamer() const{
    return currentGamer;
}

Gamer* Game::play(){

    currentGamer = &first_gamer; // передаем управление 
    setShips(first_gamer); // устанавливаем корабли

    currentGamer = &second_gamer;
    setShips(second_gamer);

    while (!isGameEnded){
        currentGamer = &first_gamer;
        giveShotManagement(first_gamer);

        if (!isGameEnded){
            currentGamer = &second_gamer;
            giveShotManagement(second_gamer);
        }
    }
    return winner;
}

void Game::giveShotManagement(Gamer& gamer){
    bool isHitted = true;
    ShotCoordinates shot;

    while (isHitted){ // ну по правилам игры стреляем пока попадаем
        Field *field = (currentGamer == &first_gamer) ? &second_gamer.getField() : &first_gamer.getField();
        auto remaining_ships = field->getRemainingShips();
        if (remaining_ships.empty()) {
            isGameEnded = true;
            winner = &gamer;
            break;
        }

        dispatchEvent("EVENT_TURN_STARTED");
        shot = gamer.make_shot();
        
        try {
            isHitted = field->makeShot(shot);
        } catch (const Exception & exception_message) {
            gamer.wrongShot(exception_message);
        }
    }
}

void Game::setShips(Gamer &gamer){
    Coordinates coordinates;
    Field *field = &(gamer.getField());
    for (size_t size = MAX_SHIP_SIZE; size >= MIN_SHIP_SIZE; --size){
        size_t count = 1;
        while (count <= (MAX_SHIP_SIZE+MIN_SHIP_SIZE)-size){
            coordinates = gamer.getShipCoordinates(size);
            try {
                field->setShip(coordinates, size);
                ++count;
                dispatchEvent("EVENT_SHIP_PLACED");
            } catch (const Exception & exception_message) {
                gamer.invalidShip(exception_message);
            }
        }
    }
}

