#include "AssistedGamer.hpp"

void AssistedGamer::init(Field * f1, EnemyField * f2, ...){
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

    previousShot.x = 0;
    previousShot.y = 0;

    isKilledShipInPreviousShot = false;

    alivingShips.resize(5);

    for (size_t i = 1; i <= 4; ++i){
        alivingShips[i] = 5 - i;
    }

    side_ships_count.resize(5);

    chance.resize(enemy_field_dimention);

    for (auto & iter : chance){
        
        iter.resize(enemy_field_dimention);

        for (auto & iteration_iter : iter){
            iteration_iter = 0;
        }
    }
}

size_t AssistedGamer::calculateDieChance(ShotCoordinates coordinate){
    int field_dimention = (int)enemy_field->getDimention();
    auto& enemyCells = enemy_field->getCellsStatus();

    int maxSize = 0;
    for (size_t i = 1; i <= alivingShips.size() - 1; ++i){ 
        if (alivingShips[i] > 0){
            maxSize = (int)i;
        }
    }
    
    int left;
    int right;
    int top;
    int bottom;
    
    (int)coordinate.x - maxSize + 1 > 0 ? left = (int)coordinate.x - maxSize + 1 : left = 0;
    ((int)coordinate.x + maxSize - 1) < (field_dimention - 1) ? right = (int)coordinate.x + maxSize - 1 : right = field_dimention - 1;
    (int)coordinate.y - maxSize + 1 > 0 ? top = (int)coordinate.y - maxSize + 1 : top = 0;
    ((int)coordinate.y + maxSize - 1) < (field_dimention - 1) ? bottom = (int)coordinate.y + maxSize - 1 : bottom = field_dimention - 1;
    
    for (int size = 2; size <= maxSize; ++size){
        if (0 == alivingShips[size]){
            continue;
        }
        
        for (int i = left; i<=right - size + 1; ++i){
            bool eq = false;
            bool miss = false;
            for (int z = i; z <= i + size - 1; ++z){
                if (missed_status == enemyCells[z][coordinate.y]){
                    miss = true;
                    break;
                }
                if ((int)coordinate.x == z){
                    eq = true;
                }
            }
            if (eq && !miss){
                ++chance[coordinate.x][coordinate.y];
            }
        }
        
        for (int k = top; k<=bottom - size + 1; ++k){
            bool eq = false;
            bool miss = false;
            for (int m = k; m <= k + size - 1; ++m){
                if (missed_status == enemyCells[coordinate.x][m]){
                    miss = true;
                    break;
                }
                if ((int)coordinate.y == m){
                    eq = true;
                }
            }
            if (eq && !miss){
                ++chance[coordinate.x][coordinate.y];
            }
        }
    }
    
    if (alivingShips[1] > 0){ 
        ++chance[coordinate.x][coordinate.y];
    }

    return chance[coordinate.x][coordinate.y];
}

ShotCoordinates AssistedGamer::denyShip(){
    int field_dimention = (int)enemy_field->getDimention();
    auto & enemyCells = enemy_field->getCellsStatus();
    
    for (auto & iter : chance){
        for (auto & iteration_iter : iter){
            iteration_iter = 0;
        }
    }
    
    int hitSize = (int) hitShip.size();
    if (1 == hitSize){
        ShotCoordinates coordinate = hitShip[0];

        int maxSize = 0;
        for (size_t i = 1; i <= alivingShips.size() - 1; ++i){ 
            if (alivingShips[i] > 0){
                maxSize = (int)i;
            }
        }
        
        int left;
        int right;
        int top;
        int bottom;
        
        (int)coordinate.x - maxSize + 1 > 0 ? left = (int)coordinate.x - maxSize + 1 : left = 0;
        ((int)coordinate.x + maxSize - 1) < (field_dimention - 1) ? right = (int)coordinate.x + maxSize - 1 : right = field_dimention - 1;
        (int)coordinate.y - maxSize + 1 > 0 ? top = (int)coordinate.y - maxSize + 1 : top = 0;
        ((int)coordinate.y + maxSize - 1) < (field_dimention - 1) ? bottom = (int)coordinate.y + maxSize - 1 : bottom = field_dimention - 1;

        for (int size = 2; size <= maxSize; ++size){
            if (alivingShips[size] == 0){
                continue;
            }
            for (int i = left; i < right - size + 2; ++i){
                bool eq = false;
                bool miss = false;
                for (int j = i; j <= i + size - 1; ++j){
                    if (missed_status == enemyCells[j][coordinate.y]){
                        miss = true;
                        break;
                    }
                    if ((int)(coordinate.x) == j)
                        eq = true;
                    
                }
                if (eq && !miss){
                    for (int j = i; j <= i + size - 1; ++j){
                        if (ghost_status == enemyCells[j][coordinate.y])
                            ++chance[j][coordinate.y];
                    }
                }
            }
            
            for (int k = top; k < bottom - size + 2; ++k){
                bool eq = false;
                bool miss = false;
                for (int m = k; m <= k + size - 1; ++m){
                    if (missed_status == enemyCells[coordinate.x][m]){
                        miss = true;
                        break;
                    }
                    if ((int)(coordinate.y) == m){
                        eq = true;
                    }
                }
                if (eq && !miss){
                    for (int m = k; m <= k + size - 1; ++m){
                        if (ghost_status == enemyCells[coordinate.y][m])
                            ++chance[coordinate.x][m];
                    }
                }
            }
        }
        
        chance[coordinate.x][coordinate.y] = 0;

        size_t maximumDieShipChance = 0;
        ShotCoordinates maximumDieShipChanceCoordinates;

        for (int x = 0; x < field_dimention; ++x){
            for (int y = 0; y < field_dimention; ++y){
                if ((chance[x][y] > maximumDieShipChance)){
                    maximumDieShipChance = chance[x][y];
                    maximumDieShipChanceCoordinates = ShotCoordinates(x, y);
                }
            }
        }
        return maximumDieShipChanceCoordinates;
    }
    
    if (hitSize > 1){ 
        if (hitShip[0].x != hitShip[1].x){ 
            std::sort(hitShip.begin(), hitShip.end(), AssistedGamer::sortHitShipX);
            
            if ((hitShip[1].x - hitShip[0].x) == 2){
                return ShotCoordinates(hitShip[0].x + 1, hitShip[0].y);
            }

            if ((hitShip[1].x - hitShip[0].x) == 3){
                return ShotCoordinates(hitShip[0].x + 2, hitShip[0].y);
            }

            if (hitShip[0].x != 0){
                if (ghost_status == enemyCells[hitShip[0].x - 1][hitShip[0].y]){
                    previousShot.x = hitShip[0].x - 1;
                    previousShot.y = hitShip[0].y;
                    return previousShot;
                }
            }
            if ((int)(hitShip[0].x) != (field_dimention - 1)){
                if (ghost_status == enemyCells[hitShip[hitSize - 1].x + 1][hitShip[0].y]){
                    previousShot.x = hitShip[hitSize - 1].x + 1;
                    previousShot.y = hitShip[0].y;
                    return previousShot;
                }
            }
        } else{ 
            std::sort(hitShip.begin(), hitShip.end(), AssistedGamer::sortHitShipY);
            
            if ((hitShip[1].y - hitShip[0].y) == 2){
                return ShotCoordinates(hitShip[0].x, hitShip[0].y + 1);
            }

            if ((hitShip[1].y - hitShip[0].y) == 3){
                return ShotCoordinates(hitShip[0].x, hitShip[0].y + 2);
            }
            
            if (hitShip[0].y != 0){
                if (ghost_status == enemyCells[hitShip[0].x][hitShip[0].y - 1]){
                    previousShot.y = hitShip[0].y - 1;
                    previousShot.x = hitShip[0].x;
                    return previousShot;
                }
            }
            if ((int)(hitShip[0].y) != (field_dimention - 1)){
                if (ghost_status == enemyCells[hitShip[0].x][hitShip[hitSize - 1].y + 1]){
                    previousShot.y = hitShip[hitSize - 1].y + 1;
                    previousShot.x = hitShip[0].x;
                    return previousShot;
                }
            }
        }

    }
    
    for (int x = 0; x < field_dimention; ++x){
        for (int y = 0; y < field_dimention; ++y){
            if (ghost_status == enemyCells[x][y]){
                return ShotCoordinates(x,y);
            }
        }
    }
    return ShotCoordinates();
}

ShotCoordinates AssistedGamer::getBestShot(){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    
    size_t field_dimention = enemy_field->getDimention();
    auto & enemyCells = enemy_field->getCellsStatus();
    

    if (died_status == enemyCells[previousShot.x][previousShot.y]){
        state = stop;
        hitShip.push_back(previousShot);
    }
    
    if (isKilledShipInPreviousShot){
        state = in_search;
        --alivingShips[hitShip.size()];
        hitShip.clear();
        isKilledShipInPreviousShot = false;
    }
    
    if (state == in_search){

        for (auto & iter : chance){
            for (auto & iteration_iter : iter){
                iteration_iter = 0;
            }
        }

        size_t maximumDieShipChance = 0;
        ShotCoordinates maximumDieShipChanceCoordinates;

        for (size_t x = 0; x < field_dimention; ++x){
            for (size_t y = 0; y < field_dimention; ++y){
                if (enemyCells[x][y] == ghost_status){
                    size_t chance = calculateDieChance(ShotCoordinates(x, y));
                    if (chance > maximumDieShipChance){
                        maximumDieShipChance = chance;
                        maximumDieShipChanceCoordinates = ShotCoordinates(x, y);
                    }
                }
            }
        }

        return maximumDieShipChanceCoordinates;

    } else { 
        
        ShotCoordinates maximumDieShipChanceCoordinates = denyShip();
        return maximumDieShipChanceCoordinates; 
    }
}

Coordinates AssistedGamer::getShipCoordinates(size_t size){
    Coordinates answer;

    if (ship_placement == undefined)
    {
        std::string ship_placement_style_s;
        view->showMessage("Choose ship placement style (manual/optimal)");
        std::cin >> ship_placement_style_s;
        if (ship_placement_style_s == "manual"){
            ship_placement = manual;
        } else if (ship_placement_style_s == "optimal"){
            ship_placement = optimal;
        } else {
            throw std::logic_error("Undefined sjip placement style, Please try again");
        }
    }
    switch (ship_placement){
        case manual: {
            answer = getShipCoordinatesFromConsole(size);
            break;
        }
        case optimal: {
            answer = getShipCoordinatesOptimal(size);
            break;
        }
        default: {
            throw std::logic_error("Undefined sjip placement style, Please try again");
        }
    }
    return answer;
}

Coordinates AssistedGamer::getShipCoordinatesFromConsole(size_t size){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    if (size == 4){ 
        view->showMyField(field);
    }
    return view->getCoordinatesForShip(size);
}

Coordinates AssistedGamer::getShipCoordinatesOptimal(size_t size){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    
    std::srand((int)std::time(0) * std::rand());
    size_t field_dimention = field->getDimention();
    Coordinates coordinates;
    
    if (size == 1){
        do{
            coordinates.x = std::rand() % (field_dimention - 4) + 2;
            coordinates.y = std::rand() % (field_dimention - 4) + 2;
            coordinates.orientation = HORIZONTAL;
        } while (0 != field->getNeighboringShips(coordinates.x, coordinates.y).size());
        return coordinates;
    }

    size_t side = 0;
    bool isDone = false;
    
    do {
        do{
            side = std::rand() % 4;
        }while (side_ships_count[side] >= 2);

        switch (side) {
            case 0: //лево
                coordinates.orientation = VERTICAL;
                coordinates.x = 0;
                coordinates.y = std::rand() % (field_dimention - size);
                break;
            case 1: //верх
                coordinates.orientation = HORIZONTAL;
                coordinates.y = 0;
                coordinates.x = std::rand() % (field_dimention - size);
                break;
            case 2: //право
                coordinates.orientation = VERTICAL;
                coordinates.x = field_dimention - 1;
                coordinates.y = std::rand() % (field_dimention - size);
                break;
            case 3: //низ
                coordinates.orientation = HORIZONTAL;
                coordinates.y = field_dimention - 1;
                coordinates.x = std::rand() % (field_dimention - size);
                break;
        }
        for (size_t i = 1; i<=size; ++i){
            auto nearShips =
                field->getNeighboringShips(coordinates.x + ((size-1)*!(coordinates.orientation)), coordinates.y + ((size-1)*coordinates.orientation));

            if (nearShips.size() == 0){
                isDone = true;
            } else {
                isDone = false;
                break;
            }
        }
    } while (!isDone);

    ++side_ships_count[side];
    previous_side = side;
    return coordinates;
}

ShotCoordinates AssistedGamer::make_shot(){
    if (!isInitializedCorrect){ // таким образом обеспечиваем безопасность вызова
        throw std::logic_error("Gamer uninitialized");
    }
    view->showMessage("Attention! You shoud look at " + bestShootStr(getBestShot()));
    ShotCoordinates coordinates = view->getCoordinatesToShoot();
    previousShot = coordinates;
    return coordinates;
}

std::string AssistedGamer::bestShootStr(ShotCoordinates coordinates) {
    std::string answer;
    answer += 'A' + coordinates.x;
    answer += coordinates.y + '0';
    return answer;
}

void AssistedGamer::invalidShip(const Exception &e){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    view->showMessage("You can't place a ship here. Try again");
}

void AssistedGamer::wrongShot(const Exception &e){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    view->showMessage("You can't shoot here. Try again");
}

void AssistedGamer::event_KilledEnemyShip(){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    isKilledShipInPreviousShot = true;
    view->showMessage("Enemy's ship is killed!");
}

bool AssistedGamer::sortHitShipX(ShotCoordinates first_coordinate, ShotCoordinates second_coordinate){
    return (first_coordinate.x < second_coordinate.x);
}

bool AssistedGamer::sortHitShipY(ShotCoordinates first_coordinate, ShotCoordinates second_coordinate){
    return (first_coordinate.y < second_coordinate.y);
}
