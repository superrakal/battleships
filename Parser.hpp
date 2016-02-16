#pragma once

#include <vector>
#include <iostream>
#include <cstring> 
#include <stdexcept>
#include "optionparser.h"


namespace Parser{
	enum optionIndex { UNDEFINED, HELP, FIRST, SECOND, COUNT };

	struct Config{
	    std::string first;
	    std::string second;
	    int count;
	    Config(std::string f, std::string s, int c) : first(f), second(s), count(c){};
	    Config() : first(""), second(""), count(0){};
	};

	    Config parse(int argc, char** argv);
		option::ArgStatus checkArg(const option::Option& option, bool msg);
}