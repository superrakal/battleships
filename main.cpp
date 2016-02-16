#include "GameController.hpp"
#include "Parser.hpp"

using namespace Parser;

int main(int argc, char* argv[])
{
    try {
        Config game_config = Parser::parse(argc, argv); // Парсим входящие параметры и генерим конфиг
        GameController game_controller(game_config.first, game_config.second, game_config.count); // Передаем конфиг в контроллер игры
        
        game_controller.startPlaying(); // играем
        
    } catch (std::exception & exception) {
    	std::cout << exception.what() << std::endl;
    } catch (...){
        std::cout << "Nevermind" << std::endl;
    }

    return 0;
}
