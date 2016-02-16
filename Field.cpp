#include "Field.hpp"


Field::Field(const size_t &dimnetion){
    field_dimention = dimnetion;

    cells.resize(field_dimention);
    for (auto & iter : cells){
        
        iter.resize(field_dimention);
        for (auto & iter_for_iter : iter)
            iter_for_iter = nullptr;
    }
    
    cellsStatus.resize(field_dimention);
    for (auto & iter : cellsStatus){
        iter.resize(field_dimention);
        for (auto & iter_for_iter : iter){
            iter_for_iter = ghost_status;
        }
    }
}

Field::~Field(){
    std::vector<Ship *> ships;

    for (auto & iter : cells){
        for (auto & iter_for_iter : iter){
            if (std::find(ships.begin(), ships.end(), iter_for_iter) == ships.end())
                ships.push_back(iter_for_iter);
        }
    }

    for (auto & iter : ships){
        if (nullptr != iter)
            delete iter;
    }
}

size_t Field::getDimention() const{
    return Field::field_dimention;
}

std::vector< std::vector < Ship* > > & Field::getCells(){
    return cells;
}

std::vector< std::vector < Status > > & Field::getCellsStatus(){
    return cellsStatus;
}

std::vector<Ship*> & Field::getRemainingShips(){
    return remaining_ships;
}

std::vector<ShotCoordinates> Field::getArea(size_t x, size_t y) const{

    std::vector<ShotCoordinates> area;
    
    if (x != 0){
        area.push_back(ShotCoordinates(x-1, y));
    }
    if (y != 0){
        area.push_back(ShotCoordinates(x, y-1));
    }
    if (x != (field_dimention - 1)){
        area.push_back(ShotCoordinates(x+1, y));
    }
    if (y != (field_dimention - 1)){
        area.push_back(ShotCoordinates(x, y+1));
    }
    if ((x != 0) && (y != 0)){
        area.push_back(ShotCoordinates(x-1, y-1));
    }
    if ((x != (field_dimention - 1)) && (y != 0)){
        area.push_back(ShotCoordinates(x+1, y-1));
    }
    
    if ((x != 0) && (y != (field_dimention - 1))){
        area.push_back(ShotCoordinates(x-1, y+1));
    }
    
    if ((x !=(field_dimention - 1) ) && (y != (field_dimention - 1))){
        area.push_back(ShotCoordinates(x+1, y+1));
    }
    
    return area;
}

std::vector<Ship*> Field::getNeighboringShips(size_t x, size_t y) const{
    std::vector<Ship*> neighboring_ships;
    auto area = getArea(x, y);
    for (auto & iter : area){
        if (nullptr != cells[iter.x][iter.y]){
            neighboring_ships.push_back(cells[iter.x][iter.y]);
        }
    }
    return neighboring_ships;
}

std::vector<ShotCoordinates> Field::getShipArea(Coordinates cell, size_t size) const{
    
    std::vector<ShotCoordinates> shipArea;

    for (size_t x = cell.x; x <= cell.x + (size-1)*(!cell.orientation); ++x){
        for (size_t y = cell.y; y <= cell.y + (size-1)*(cell.orientation); ++y){
            auto cellAureole = getArea(x, y);
            for (auto & iter : cellAureole){
                if (shipArea.end() == std::find(shipArea.begin(), shipArea.end(), iter)){
                    shipArea.push_back(iter);
                }
            }
        }
    }
    for (size_t x = cell.x; x <= cell.x + (size-1)*(!cell.orientation); ++x){
        for (size_t y = cell.y; y <= cell.y + (size-1)*(cell.orientation); ++y){
            auto iter = std::find(shipArea.begin(), shipArea.end(), ShotCoordinates(x,y));
            if (shipArea.end() != iter){
                shipArea.erase(iter);
            }
        }
    }
    return shipArea;
}

void Field::setShip(const Coordinates & cell, size_t size){

    if ((cell.x > Field::field_dimention) || (cell.x + (size-1)*(!cell.orientation) >= Field::field_dimention) 
        || (cell.y > Field::field_dimention) || (cell.y + (size-1)*(cell.orientation) >= Field::field_dimention)){
        throw OutOfFieldError();
    }

    std::vector<Ship*> neighboring_ships;
    
    for (size_t x = cell.x; x <= cell.x + (size-1)*(!cell.orientation); ++x){
        for (size_t y = cell.y; y <= cell.y + (size-1)*(cell.orientation); ++y){
            neighboring_ships = getNeighboringShips(x, y);
            if ((!(neighboring_ships.empty())) || (nullptr != cells[cell.x][cell.y])){
                throw TooCloseError();
            }
        }
    }
    
    Ship *s = new Ship(size, cell);
    for (size_t x = cell.x; x <= cell.x + (size-1)*(!cell.orientation); ++x){
        for (size_t y = cell.y; y <= cell.y + (size-1)*(cell.orientation); ++y)
            cells[x][y] = s;
    }

    remaining_ships.push_back(s);
}

bool Field::makeShot(const ShotCoordinates & cell){
    if ((died_status == cellsStatus[cell.x][cell.y]) || (missed_status == cellsStatus[cell.x][cell.y])){
        throw CantShootHereError();
    }
    if (nullptr == cells[cell.x][cell.y]){
        cellsStatus[cell.x][cell.y] = missed_status;
        return false;
    } else{
        cellsStatus[cell.x][cell.y] = died_status;
        cells[cell.x][cell.y]->shoot();
        auto iter = std::find(remaining_ships.begin(), remaining_ships.end(), cells[cell.x][cell.y]);
        if ((*iter)->isDead()){
            auto shipArea = getShipArea((*iter)->getCoordinates(), (*iter)->getSize());
            for (const auto & iter : shipArea){
                cellsStatus[iter.x][iter.y] = missed_status;
            }
            remaining_ships.erase(iter);
            dispatchEvent("EVENT_SHIP_KILLED");
        }
        return true;
    }
}

std::vector< std::vector< Status > > & EnemyField::getCellsStatus() const{
    return cellsStatus;
}

size_t EnemyField::getDimention() const{
    return field_dimention;
}