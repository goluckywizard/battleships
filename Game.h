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
public:
    int attack_table[10][10];
    bool ship_table[10][10];
    virtual void shipArrangement() = 0;
    virtual int play() = 0;
    virtual void turn_result(int turn, int number) = 0;
};

class ConsoleGamer : public Gamer {
private:
public:
    //int attack_table[10][10] = {0};
    //bool ship_table[10][10] = {false};
    void shipArrangement() override;
    int play() override;
    void turn_result(int turn, int number) override;
};

class RandomGamer: public Gamer {
private:
    //int attack_table[10][10] = {};
public:
    //bool ship_table[10][10] = {false};
    void shipArrangement() override;

    int play() override {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, 9);
        int x = dist(mt), y = dist(mt);
        while (attack_table[x][y] != 0) {
            x = dist(mt);
            y = dist(mt);
        }
        return x * 10 + y;
    }

    void turn_result (int turn, int number) override {
        if (turn == KILL)
            attack_table[number / 10][number % 10] = HIT;
        else
            attack_table[number / 10][number % 10] = turn;
    };
};

class OptimalGamer: public Gamer {
private:
    struct ship_part {
        int place;
        bool isMarked;
    };
public:
    //bool ship_table[10][10] = {false};
    void shipArrangement() override {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                ship_table[i][j] = false;
            }
        }
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                attack_table[i][j] = 0;
            }
        }
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, 6);
        int left = 3;
        bool bounds[40] = {false};
        //int position = 1;
        std::vector<int> ships = {3, 1, 1, 2, 2, 2, 3, 1};
        for (int i = 0; i < ships.size() - 1; ++i) {
            int swap = dist(mt);
            std::swap(ships[i], ships[swap]);
        }
        bounds[0] = true; bounds[1] = true; bounds[2] = true; bounds[3] = true;
        for (int i = 0; i < ships[0]; ++i) {
            bounds[5+i] = true;
        }
        for (int bound = 1; bound < 4; ++bound) {
            int place = 1;
            for (place = 1; place < 1+ships[bound*2 - 1]; ++place) {
                bounds[bound*10 + place] = true;
            }
            ++place;
            int k = place;
            for (place; place < k+ships[bound*2]; ++place) {
                bounds[bound*10 + place] = true;
            }
            //++place;
        }
        for (int place = 7; place > 7 - ships[7]; --place) {
            bounds[30 + place] = true;
        }
        std::uniform_int_distribution<int> dista(2, 7);
        int x = dista(mt);
        int y = dista(mt);
        //std::cout << x << y << "\n";
        ship_table[x][y] = true;
        for (int i = 0; i < 10; ++i) {
            ship_table[0][i] = bounds[i];
            ship_table[i][9] = bounds[10+i];
            ship_table[9][i] = bounds[29-i];
            ship_table[i][0] = bounds[39-i];
        }
        ship_table[0][0] = true;
        /*for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j)
            {
                if (i != 0 && j != 0)
                    ship_table[i][j] = false;
            }
        }*/
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (ship_table[i][j]) {
                    std::cout <<"#";
                }
                else
                    std::cout <<"0";
            }
            std::cout << std::endl;
        }
    }
    int play() override {
        int hit = -1;
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (attack_table[i][j] == HIT) {
                    for (int x = -1; x <= 1; ++x) {
                        for (int y = -1; y <= 1; ++y)
                        {
                            if (attack_table[i+x][j+y] == 0)
                                hit = (i+x)*10+(j+y);
                        }
                    }
                }
            }
        }
        if (hit == -1) {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(0, 9);
            int x = dist(mt), y = dist(mt);
            while (attack_table[x][y] != 0) {
                x = dist(mt);
                y = dist(mt);
            }
            hit = x * 10 + y;
        }
        return hit;
    };
    void turn_result (int turn, int number) override;
};

std::unique_ptr<Gamer> choose_Gamer(int k);

class Game {
    bool check_ships(bool table[10][10]);
    void Game::Turn(const std::shared_ptr<Gamer>& attacker, const std::shared_ptr<Gamer>& defender, int &hits);
    void doGame(int g_type1, int g_type2);
public:
    Game(int argc, char *argv[]);
};


