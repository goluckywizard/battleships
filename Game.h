#pragma once
#include "optionparser.h"
#include <iostream>
#include <regex>
#include <memory>
#include <iomanip>
#include <random>
#include <list>
struct Arg: public option::Arg
{
    static void printError(const char* msg1, const option::Option& opt, const char* msg2)
    {
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
    }

    static option::ArgStatus Unknown(const option::Option& option, bool msg)
    {
        if (msg) printError("Unknown option '", option, "'\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Required(const option::Option& option, bool msg)
    {
        if (option.arg != 0)
            return option::ARG_OK;

        if (msg) printError("Option '", option, "' requires an argument\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus NonEmpty(const option::Option& option, bool msg)
    {
        if (option.arg != 0 && option.arg[0] != 0)
            return option::ARG_OK;

        if (msg) printError("Option '", option, "' requires a non-empty argument\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Numeric(const option::Option& option, bool msg)
    {
        char* endptr = 0;
        if (option.arg != 0 && strtol(option.arg, &endptr, 10)){};
        if (endptr != option.arg && *endptr == 0)
            return option::ARG_OK;

        if (msg) printError("Option '", option, "' requires a numeric argument\n");
        return option::ARG_ILLEGAL;
    }
};
using option::Option;
using option::Descriptor;
using option::Parser;
using option::Stats;
using option::ArgStatus;
enum  optionIndex { UNKNOWN, HELP, COUNT, FIRST, SECOND };
const option::Descriptor usage[] =
        {
                {UNKNOWN, 0, "", "", option::Arg::None, "USAGE: example [options]\n\n"
                                                       "Options:" },
                {HELP, 0,"h", "help", option::Arg::None, "  --help, -h  \tPrint usage and exit." },
                {COUNT, 0,"c","count", Arg::Numeric, "  --count, -c  \tCount of games." },
                {FIRST, 0,"f","first", Arg::NonEmpty, "  --first, -f  \tFirst player's type." },
                {SECOND, 0,"s","second", Arg::NonEmpty, "  --second, -s  \tSecond player's type." },
                {UNKNOWN, 0, "", "", option::Arg::None, "\nExamples:\n"
                                                       "  example --unknown -- --this_is_no_option\n"
                                                       "  example -unk --plus -ppp file1 file2\n" },
                {0,0,0,0,0,0}
        };
enum DotState {UNKNOW = 0, MISS, HIT, KILL};

class Gamer {
protected:
    std::vector<std::vector<int> > attack_table;
    std::vector<std::vector<bool> > ship_table;
public:
    Gamer() {
        for (int i = 0; i < 10; ++i) {
            ship_table.emplace_back(10);
            attack_table.emplace_back(std::vector<int>(10));
        }
    }
    virtual void shipArrangement() = 0;
    virtual int play() = 0;
    virtual void turn_result(int turn, int number) = 0;
    std::vector<std::vector<bool> > getShiptable() const{
        return ship_table;
    }
    std::vector<std::vector<int> > getAttacktable() const {
        return attack_table;
    }
};

class ConsoleGamer : public Gamer {
private:
public:
    void shipArrangement() override;
    int play() override;
    void turn_result(int turn, int number) override;
};

class RandomGamer: public Gamer {
private:
public:
    void shipArrangement() override;

    int play() override;

    void turn_result (int turn, int number) override;
};

class OptimalGamer: public Gamer {
private:
    struct ship_part {
        int place;
        bool isMarked;
    };
public:
    void shipArrangement() override;
    int play() override;
    void turn_result (int turn, int number) override ;
};

std::unique_ptr<Gamer> choose_Gamer(int k);

class Game {
    bool check_ships(const std::vector<std::vector<bool> > &table);
    void Turn(const std::shared_ptr<Gamer>& attacker, const std::vector<std::vector<bool> > defenders_table, int &hits);
    void doGame(int g_type1, int g_type2);
public:
    Game(int argc, char *argv[]);
};



