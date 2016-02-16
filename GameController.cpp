#include "GameController.hpp"

GameController::GameController(std::string first_gamer_type_s, std::string second_gamer_type_s, int rounds_count){
    this->rounds_count = rounds_count;
    this->first_gamer_type_s = first_gamer_type_s;
    this->second_gamer_type_s = second_gamer_type_s;
}

GamerType GameController::getGamerType(Gamer *gamer) const{
    return (first_gamer == gamer) ? first_gamer_type : second_gamer_type;
}

void GameController::initGame(){
    
    GamerObject gSubstance = GamerFactory::createGamer(first_gamer_type_s);
    this->first_gamer = gSubstance.gamerPointer;
    this->first_gamer_type = gSubstance.gamerType;
    first_gamer_field = new Field(GameController::field_dimention);
    first_gamer_enemy_field = new EnemyField(first_gamer_field->getCellsStatus(), first_gamer_field->getDimention());
    first_gamer_field->addEventListener("EVENT_SHIP_KILLED", this, &GameController::event_ShipKilled);

    gSubstance = GamerFactory::createGamer(second_gamer_type_s);
    this->second_gamer = gSubstance.gamerPointer;
    this->second_gamer_type = gSubstance.gamerType;
    second_gamer_field = new Field(GameController::field_dimention);
    second_gamer_enemy_field = new EnemyField(second_gamer_field->getCellsStatus(), second_gamer_field->getDimention());
    second_gamer_field->addEventListener("EVENT_SHIP_KILLED", this, &GameController::event_ShipKilled);
    
    view = new ConsoleView;

    
    (human == first_gamer_type) ? first_gamer->init(first_gamer_field, second_gamer_enemy_field, view) 
                                : first_gamer->init(first_gamer_field, second_gamer_enemy_field);
    
    (human == second_gamer_type) ? second_gamer->init(second_gamer_field, first_gamer_enemy_field, view)
                                 : second_gamer->init(second_gamer_field, first_gamer_enemy_field);
    
    game = new Game(*first_gamer, *second_gamer);
    
    game->addEventListener("EVENT_SHIP_PLACED", this, &GameController::event_ShipPlaced);
    game->addEventListener("EVENT_TURN_STARTED", this, &GameController::event_YourTurn);
}

void GameController::deInitGame(){
    delete game;
    delete first_gamer_enemy_field;
    delete second_gamer_enemy_field;
    delete first_gamer_field;
    delete second_gamer_field;
    delete first_gamer;
    delete second_gamer;
    delete view;
    game = nullptr;
    first_gamer_enemy_field = nullptr;
    second_gamer_enemy_field = nullptr;
    first_gamer_field = nullptr;
    second_gamer_field = nullptr;
    first_gamer = nullptr;
    second_gamer = nullptr;
    view = nullptr;
}

void GameController::event_ShipPlaced() const{
    Gamer * currentGamer = game->getCurrentGamer();
    if (human == getGamerType(currentGamer))
        view->showMyField(&currentGamer->getField());
    
}

void GameController::event_ShipKilled() const{
    (first_gamer == game->getCurrentGamer()) ? first_gamer->event_KilledEnemyShip() : second_gamer->event_KilledEnemyShip();
}

void GameController::event_YourTurn() const{
    Gamer * currentGamer = game->getCurrentGamer();
    if (human == getGamerType(currentGamer)){
        view->showMessage("It's your turn now!");
    
        view->showMessage("Your field:");
        view->showMyField(&currentGamer->getField());
    
        view->showMessage("Enemy field:");
        view->showEnemyField(&currentGamer->getEview());
    
        view->showMessage("Make a shot. For example 'A1'");
    }
    
    currentGamer->event_YourTurn();
}

void GameController::startPlaying(){
    time_start = clock();
    for (int i = 1; i <= rounds_count; ++i){

        initGame();
        
        if (human == getGamerType(game->getCurrentGamer())){
            view->showMessage("Welcome to the Battleship!");
        }

        Gamer *winner = game->play();
        
        (winner == first_gamer) ? ++first_gamer_wins_count: ++second_gamer_wins_count;

        if (i != rounds_count){
            deInitGame();
        }
    }
    
    time_finish = clock();
    view->showGameStatistic(first_gamer_wins_count, second_gamer_wins_count, time_start, time_finish);
    deInitGame();
}