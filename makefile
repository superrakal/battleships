all:
	g++ main.cpp Parser.cpp View.cpp GameController.cpp Game.cpp BaseGamer.cpp OptimalGamer.cpp ConsoleGamer.cpp RandomGamer.cpp Field.cpp Ship.cpp -std=c++11 -o Battleship -Wall
clear:
	rm Battleship
