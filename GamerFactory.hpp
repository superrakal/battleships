#pragma once
#include "Gamer.hpp"

enum GamerType{
    computer, human
};

struct GamerObject{
    Gamer* gamerPointer;
    GamerType gamerType;
    GamerObject(Gamer * g, GamerType gt) : gamerPointer(g), gamerType(gt){}
};

class GamerFactory{
public:
    static GamerObject createGamer(std::string substance_string){
        if ("RandomGamer" == substance_string){
            return GamerObject(new RandomGamer(), computer);
        }
        if ("ConsoleGamer" == substance_string){
            return GamerObject(new ConsoleGamer(), human);
        }
        if ("OptimalGamer" == substance_string){
            return GamerObject(new OptimalGamer(), computer);
        }
        if ("AssistedGamer" == substance_string){
            return GamerObject(new AssistedGamer(), human);
        }
        throw std::invalid_argument("invalid Gamer");
    };
};
