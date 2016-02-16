#include "View.hpp"

void ConsoleView::showMyField(Field* field) const{

    size_t field_dimention = field->getDimention();
    auto cells = field->getCells();
    auto cellsStatus = field->getCellsStatus();
    size_t i = 1;
    
    std::cout << "   A B C D E F G H I J " << std::endl;

    for (size_t y = 0; y < field_dimention; ++y){
        if (i == 10){
            std::cout << (i++) << space;
        }
        else{
            std::cout << (i++) << space << space;
        }
        for (size_t x = 0; x < field_dimention; ++x){
            if (nullptr == cells[x][y]){
                if (ghost_status == cellsStatus[x][y]){
                    std::cout << ghost_cell << space;
                } else {
                    std::cout << missed_shot << space;
                }
            } else{
                if (ghost_status == cellsStatus[x][y]){
                    std::cout << ship_part << space;
                } else {
                    std::cout<<died_ship_part<<space;
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void ConsoleView::showEnemyField(const EnemyField* field) const{
    size_t field_dimention = field->getDimention();
    auto cellsStatus = field->getCellsStatus();
    size_t i = 1;

    std::cout << "   A B C D E F G H I J " << std::endl;

    for (size_t y = 0; y < field_dimention; ++y){
        
        (i == 10) ? std::cout << (i++) << space : std::cout << (i++) << space << space;

        for (size_t x = 0; x < field_dimention; ++x){
            if (ghost_status == cellsStatus[x][y])
                std::cout << ghost_cell << space;
            
            if (missed_status == cellsStatus[x][y])
                std::cout << missed_shot << space;

            if (died_status == cellsStatus[x][y])
                std::cout << died_ship_part << space;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void ConsoleView::showGameStatistic(int first_gamer_wins_count, int second_gamer_wins_count, long time_start, long time_finish) const{
    std::cout << "Statistic:" << std::endl;
    std::cout << first_gamer_wins_count << " : "<< second_gamer_wins_count << std::endl;
    std::cout << "Playing time (~sec):" << (time_finish - time_start) / CLOCKS_PER_SEC << std::endl;
}

void ConsoleView::showMessage(std::string message) const{
    std::cout << message << std::endl;
}

Coordinates ConsoleView::getCoordinatesForShip(size_t size){
    bool isEnded = false;
    size_t x = 0, y = 0;
    Orientation orientation = HORIZONTAL;
    std::string input_string;
    while (!isEnded){
        size_t letterIndex = 0;
        size_t numberIndex = 1;
        size_t orientationIndex = 3;
        std::cout << "Place a ship with size "<< size << std::endl;
        do{
            getline(std::cin, input_string);
        } while("" == input_string);

        if ((input_string[numberIndex] >= '1') && (input_string[numberIndex] <= '9')){
            if (('1' == input_string[numberIndex]) && ('0' == input_string[numberIndex + 1])){
                if (input_string.length() != 5){
                    std::cout << "[1;31mBad input. Try again[0m" << std::endl;
                    continue;
                }
                ++orientationIndex;
                y = 9;
                
            } else{
                if (input_string.length() != 4){
                    std::cout << "[1;31mBad input. Try again[0m" << std::endl;
                    continue;
                }
                y = input_string[numberIndex] - (int)'1';
            }
        } else{
            std::cout << "[1;31mBad input. Try again[0m" << std::endl;
            continue;
        }
        
        if (((input_string[letterIndex] >= 'A') && (input_string[letterIndex] <= 'I')) || ('J' == input_string[letterIndex])){
            if ('J' != input_string[letterIndex]){
                x = input_string[letterIndex] - size_t('A');
            } else{
                x = 9;
            }
        } else{
            std::cout << "[1;31mBad input. Try again[0m" << std::endl;
            continue;
        }
        
        if ('v' == input_string[orientationIndex]){
            isEnded = true;
            orientation = VERTICAL;
        } else if ('h' == input_string[orientationIndex]){
            isEnded = true;
            orientation = HORIZONTAL;
        } else{
            std::cout << "[1;31mBad input. Try again[0m" << std::endl;
            continue;
        }
    }
    return Coordinates(x, y, orientation);
}

ShotCoordinates ConsoleView::getCoordinatesToShoot(){
    std::string input_string;

    size_t letterIndex = 0;
    size_t numberIndex = 1;
    size_t x = 0, y = 0;
    bool isEnded = false;
    
    while (!isEnded){
        do{
            getline(std::cin, input_string);
        } while("" == input_string);
        
        if ((input_string[numberIndex] >= '1') && (input_string[numberIndex] <= '9')){
            if (('1' == input_string[numberIndex]) && ('0' == input_string[numberIndex + 1])){
                if (input_string.length() != 3){
                    std::cout << "[1;31mBad input. Try again[0m" << std::endl;
                    continue;
                }
                y = 9;
            } else{
                if (input_string.length() != 2){
                    std::cout << "[1;31mBad input. Try again[0m" << std::endl;
                    continue;
                }
                y = input_string[numberIndex] - (int)'1';
            }
        } else{
            std::cout << "[1;31mBad input. Try again[0m" << std::endl;
            continue;
        }

        if (((input_string[letterIndex] >= 'A') && (input_string[letterIndex] <= 'I')) || ('J' == input_string[letterIndex])){
            if ('J' != input_string[letterIndex]){
                x = input_string[letterIndex] - size_t('A');
                isEnded = true;
            } else{
                x = 9;
                isEnded = true;
            }
        } else{
            std::cout << "[1;31mBad input. Try again[0m" << std::endl;
            continue;
        }

    }
    return ShotCoordinates(x,y);
}