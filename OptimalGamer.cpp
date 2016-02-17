#include "OptimalGamer.hpp"
#include <iostream>
#include <cmath>

// Алгоритм таков: расставляем большие корабли по бокам, а единички в центре и таким образом, чтобы они покрыли максимальную площадь.
// Ну а стреляем очевидно: если у нас нет четырехпалубника, то мы не ищем четвертую клетку и так далее.
// Подсчитываем шанс смерти корабля, а при его гибели его окрестность помечаем некликабельной.

void OptimalGamer::init(Field * f1, EnemyField * f2, ...){
    field = f1;
    enemy_field = f2;

    size_t enemy_field_dimention = enemy_field->getDimention();

    for (size_t i = 0; i < enemy_field_dimention; ++i){
        for (size_t j = 0; j < enemy_field_dimention; ++j){
            shoting_queue.push_back(ShotCoordinates(i, j));
        }
    }
    
    previousShot.x = 0;
    previousShot.y = 0;

    isKilledShipInPreviousShot = false;

    isInitializedCorrect = true;
    
    alivingShips.resize(5);

    for (size_t i = 1; i <= 4; ++i){
        alivingShips[i] = 5 - i;
    }

    side_ships_count.resize(5);

    size_t field_dimention = enemy_field->getDimention();

    chance.resize(field_dimention);

    for (auto & iter : chance){
        
        iter.resize(field_dimention);

        for (auto & iteration_iter : iter){
            iteration_iter = 0;
        }
    }
}



Coordinates OptimalGamer::getShipCoordinates(size_t size){
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
        for (size_t i = 1; i <=size; ++i){
            auto nearShips =
                field->getNeighboringShips(coordinates.x + ((i-1)*!(coordinates.orientation)), coordinates.y + ((i-1)*coordinates.orientation));

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

size_t OptimalGamer::calculateDieChance(ShotCoordinates cell){
    size_t field_dimention = (size_t)enemy_field->getDimention();
    auto& enemyCells = enemy_field->getCellsStatus();

    size_t maxSize = 0;
    for (size_t i = 1; i <= alivingShips.size() - 1; ++i){ //максимальный размер корабля на поле
        if (alivingShips[i] > 0){
            maxSize = (size_t)i;
        }
    }
    
    size_t left;
    size_t right;
    size_t top;
    size_t bottom;
    
    (size_t)cell.x - maxSize + 1 > 0 ? left = (size_t)cell.x - maxSize + 1 : left = 0;
    ((size_t)cell.x + maxSize - 1) < (field_dimention - 1) ? right = (size_t)cell.x + maxSize - 1 : right = field_dimention - 1;
    (size_t)cell.y - maxSize + 1 > 0 ? top = (size_t)cell.y - maxSize + 1 : top = 0;
    ((size_t)cell.y + maxSize - 1) < (field_dimention - 1) ? bottom = (size_t)cell.y + maxSize - 1 : bottom = field_dimention - 1;
    
    for (size_t size = 2; size <= maxSize; ++size){
        if (0 == alivingShips[size]){
            continue;
        }
        
        for (size_t i = left; i<=right - size + 1; ++i){
            bool eq = false;
            bool miss = false;
            for (size_t z = i; z <= i + size - 1; ++z){
                if (missed_status == enemyCells[z][cell.y]){
                    miss = true;
                    break;
                }
                if ((size_t)cell.x == z){
                    eq = true;
                }
            }
            if (eq && !miss){
                ++chance[cell.x][cell.y];
            }
        }
        
        for (size_t k = top; k<=bottom - size + 1; ++k){
            bool eq = false;
            bool miss = false;
            for (size_t m = k; m <= k + size - 1; ++m){
                if (missed_status == enemyCells[cell.x][m]){
                    miss = true;
                    break;
                }
                if ((size_t)cell.y == m){
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

ShotCoordinates OptimalGamer::denyShip(){
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

        for (int x = left; x <= right; ++x){
            for (int y = top; y <= bottom; ++y){
                if (chance[x][y] > maximumDieShipChance && (std::abs(x - (int)cell.x + y - (int)cell.y) == 1)){
                    maximumDieShipChance = chance[x][y];
                    maximumDieShipChanceCoordinates = ShotCoordinates(x, y);
                }
            }
        }
        return maximumDieShipChanceCoordinates;
    }
    
    if (hitSize > 1){ 
        if (hitShip[0].x != hitShip[1].x){ 
            std::sort(hitShip.begin(), hitShip.end(), OptimalGamer::sortHitShipX);
            
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
            std::sort(hitShip.begin(), hitShip.end(), OptimalGamer::sortHitShipY);
            
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

ShotCoordinates OptimalGamer::make_shot(){
    if (!isInitializedCorrect){
        throw std::logic_error("Gamer uninitialized");
    }
    
    size_t field_dimention = enemy_field->getDimention();
    auto & enemyCells = enemy_field->getCellsStatus();
    
    if (died_status == enemyCells[previousShot.x][previousShot.y]){
        state = stopped;
        hitShip.push_back(previousShot);
    }
    
    if (isKilledShipInPreviousShot){
        state = searching;
        --alivingShips[hitShip.size()];
        hitShip.clear();
        isKilledShipInPreviousShot = false;
    }
    
    if (searching == state){

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

void OptimalGamer::event_KilledEnemyShip(){
    if (!isInitializedCorrect)
        throw std::logic_error("Gamer uninitialized");
    isKilledShipInPreviousShot = true;
}

bool OptimalGamer::sortHitShipX(ShotCoordinates first_coordinate, ShotCoordinates second_coordinate){
    return (first_coordinate.x < second_coordinate.x);
}

bool OptimalGamer::sortHitShipY(ShotCoordinates first_coordinate, ShotCoordinates second_coordinate){
    return (first_coordinate.y < second_coordinate.y);
}
