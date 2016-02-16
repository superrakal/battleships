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

size_t AssistedGamer::calculateDieChance(ShotCoordinates cell){
    int field_dimention = (int)enemy_field->getDimention();
    auto& enemyCells = enemy_field->getCellsStatus();

    int maxSize = 0;
    for (size_t i = 1; i <= alivingShips.size() - 1; ++i){ //максимальный размер корабля на поле
        if (alivingShips[i] > 0){
            maxSize = (int)i;
        }
    }
    
    int left;
    int right;
    int top;
    int bottom;
    
    (int)cell.x - maxSize + 1 > 0 ? left = (int)cell.x - maxSize + 1 : left = 0;
    ((int)cell.x + maxSize - 1) < (field_dimention - 1) ? right = (int)cell.x + maxSize - 1 : right = field_dimention - 1;
    (int)cell.y - maxSize + 1 > 0 ? top = (int)cell.y - maxSize + 1 : top = 0;
    ((int)cell.y + maxSize - 1) < (field_dimention - 1) ? bottom = (int)cell.y + maxSize - 1 : bottom = field_dimention - 1;
    
    for (int size = 2; size <= maxSize; ++size){
        if (0 == alivingShips[size]){
            continue;
        }
        
        for (int i = left; i<=right - size + 1; ++i){
            bool eq = false;
            bool miss = false;
            for (int z = i; z <= i + size - 1; ++z){
                if (missed_status == enemyCells[z][cell.y]){
                    miss = true;
                    break;
                }
                if ((int)cell.x == z){
                    eq = true;
                }
            }
            if (eq && !miss){
                ++chance[cell.x][cell.y];
            }
        }
        
        for (int k = top; k<=bottom - size + 1; ++k){
            bool eq = false;
            bool miss = false;
            for (int m = k; m <= k + size - 1; ++m){
                if (missed_status == enemyCells[cell.x][m]){
                    miss = true;
                    break;
                }
                if ((int)cell.y == m){
                    eq = true;
                }
            }
            if (eq && !miss){
                ++chance[cell.x][cell.y];
            }
        }
    }
    
    if (alivingShips[1] > 0){ //можно поставить однопалубник, если еще остались
        ++chance[cell.x][cell.y];
    }

    return chance[cell.x][cell.y];
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
        ShotCoordinates cell = hitShip[0];

        int maxSize = 0;
        for (size_t i = 1; i <= alivingShips.size() - 1; ++i){ //максимальный размер
            if (alivingShips[i] > 0){
                maxSize = (int)i;
            }
        }
        
        int left;
        int right;
        int top;
        int bottom;
        
        (int)cell.x - maxSize + 1 > 0 ? left = (int)cell.x - maxSize + 1 : left = 0;
        ((int)cell.x + maxSize - 1) < (field_dimention - 1) ? right = (int)cell.x + maxSize - 1 : right = field_dimention - 1;
        (int)cell.y - maxSize + 1 > 0 ? top = (int)cell.y - maxSize + 1 : top = 0;
        ((int)cell.y + maxSize - 1) < (field_dimention - 1) ? bottom = (int)cell.y + maxSize - 1 : bottom = field_dimention - 1;

        for (int size = 2; size <= maxSize; ++size){
            if (alivingShips[size] == 0){
                continue;
            }
            for (int i = left; i < right - size + 2; ++i){
                bool eq = false;
                bool miss = false;
                for (int z = i; z <= i + size - 1; ++z){
                    if (missed_status == enemyCells[z][cell.y]){
                        miss = true;
                        break;
                    }
                    if ((int)(cell.x) == z){
                        eq = true;
                    }
                }
                if (eq && !miss){
                    for (int z = i; z <= i + size - 1; ++z){
                        ++chance[z][cell.y];
                    }
                }
            }
            
            for (int k = top; k < bottom - size + 2; ++k){
                bool eq = false;
                bool miss = false;
                for (int m = k; m <= k + size - 1; ++m){
                    if (missed_status == enemyCells[cell.x][m]){
                        miss = true;
                        break;
                    }
                    if ((int)(cell.y) == m){
                        eq = true;
                    }
                }
                if (eq && !miss){
                    for (int m = k; m <= k + size - 1; ++m){
                        ++chance[cell.x][m];
                    }
                }
            }
        }
        
        chance[cell.x][cell.y] = 0;
        
        size_t maximumDieShipChance = 0;
        ShotCoordinates maximumDieShipChanceCoordinates;

        for (int x = 0; x < field_dimention; ++x){
            for (int y = 0; y < field_dimention; ++y){
                if (chance[x][y] > maximumDieShipChance){
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
    
    if (in_search == state){

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

        previousShot = maximumDieShipChanceCoordinates;
        return maximumDieShipChanceCoordinates;

    } else { 
        
        ShotCoordinates maximumDieShipChanceCoordinates = denyShip();
        previousShot = maximumDieShipChanceCoordinates;

        return maximumDieShipChanceCoordinates; // вернет координату наиболее вероятного удара
    }
}

Coordinates AssistedGamer::getShipCoordinates(size_t size){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    if (size == 4){ // ввод корректен например a4 v
        view->showMyField(field);
    }
    return view->getCoordinatesForShip(size);
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
