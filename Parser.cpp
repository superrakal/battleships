#include "Parser.hpp"

namespace Parser{
    option::ArgStatus checkArg(const option::Option& option, bool msg){
        if (option.name[0] == 'c'){
            if (option.arg == 0 || option.arg[0] == 0){
                return option::ARG_ILLEGAL;
            }
            for (size_t i = 0; i< strlen(option.arg); ++i){
                char c = option.arg[i];
                c = c - '0';
                if (c > 9 || c < 0){
                    return option::ARG_ILLEGAL;
                }
            }
            return option::ARG_OK;
        }
        if (option.arg != 0 && option.arg[0] != 0){
            return option::ARG_OK;
        }
        return option::ARG_ILLEGAL;
    }

    Config parse(int argc, char** argv){
        option::Descriptor usage[] =
        {
            {UNDEFINED, 0, "", "",  option::Arg::None, "Welcome to the Battleship!\nThere are 3 gamer types avaliable: OptimalGamer, ConsoleGamer, RandomGamer.\n\n"
                "Options:" },
            {HELP, 0,"h", "help",   option::Arg::None, "  --help, -h  \tPrint help information and exit application." },
            {FIRST, 0,"f","first",  Parser::checkArg, "  --first, -f  \tFirst gamer type." },
            {SECOND, 0,"s","second",Parser::checkArg, "  --second, -s  \tSecond gamer type." },
            {COUNT, 0,"c","count",  Parser::checkArg, "  --count, -c  \tRounds count." },
            {UNDEFINED, 0, "", "",  option::Arg::None, "\nExamples:\n"
                "  example ./Battleship -f ConsoleGamer -s OptimalGamer -c 1\n"
                "  example ./Battleship --first RandomGamer --second OptimalGamer --count 100\n" },
            {0,0,0,0,0,0}
        };

        argc-=(argc>0); argv+=(argc>0);
        option::Stats  stats(usage, argc, argv);
        std::vector<option::Option> options(stats.options_max);
        std::vector<option::Option> buffer(stats.buffer_max);
        option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);
        
        if (options[HELP] || argc == 0) {
            option::printUsage(std::cout, usage);
            throw std::invalid_argument("You pass invalid argument. Run with -h arg to see help");
        }
        
        if (parse.error() || !options[FIRST] || !options[SECOND] || !options[COUNT] || argc > 6)
            throw std::invalid_argument("You pass invalid argument. Run with -h arg to see help");
        
        Config conf(options[FIRST].arg, options[SECOND].arg, std::stoi(options[COUNT].arg));
        return conf;
    }
}
