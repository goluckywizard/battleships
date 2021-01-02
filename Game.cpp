#include "Game.h"
#pragma once
std::unique_ptr<Gamer> choose_Gamer(int k) {
    if (k == 1) {
        return std::make_unique<ConsoleGamer>();
    } else if (k == 2) {
        return std::make_unique<RandomGamer>();
    } else {
        return std::make_unique<OptimalGamer>();
    }
}
bool Game::check_ships(std::shared_ptr<Gamer> &gamer) {
    bool table[10][10];
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            table[i][j] = gamer->getShiptable(i, j);
        }
    }
    int count_ships[5] = {0, 0, 0, 0, 0};
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if ((table[i][j+1] && table[i+1][j]) || (table[i][j]&& table[i+1][j+1])) {
                std::cout << "Korabli stoyat ryadom! ";
                return false;
            }
        }
    }
    for (int i = 0; i < 10; ++i) {
        int in_a_row = 0;
        for (int j = 0; j < 10; ++j) {
            if (table[i][j]) {
                ++in_a_row;
            }
            else {
                ++count_ships[in_a_row];
                in_a_row = 0;
            }
        }
        ++count_ships[in_a_row];
    }
    for (int i = 0; i < 10; ++i) {
        int in_a_row = 0;
        for (int j = 0; j < 10; ++j) {
            if (table[j][i]) {
                ++in_a_row;
            }
            else {
                if (in_a_row > 4)
                {
                    std::cout << "Korabl dlini 5 ili bolee! ";
                    return false;
                }
                ++count_ships[in_a_row];
                in_a_row = 0;
            }
        }
        ++count_ships[in_a_row];
    }
    for (int i = 0; i < 5; ++i) {
        std::cout << i << ": " << count_ships[i] << "\n";
    }
    if (count_ships[4] != 1 || count_ships[3] != 2 || count_ships[2] != 3) {
        /*if (count_ships[4] != 1)
            std::cout << "4: ";
        if (count_ships[3] != 2)
            std::cout << "3: ";
        if (count_ships[2] != 3)
            std::cout << "4: ";
        if (count_ships[1] != 4)
            std::cout << "3: ";*/
        std::cout << "Not enough ships!\n";
        return false;
    }
    if (count_ships[1] != 24)
        return false;

    return true;
}

void Game::Turn(const std::shared_ptr<Gamer>& attacker, const bool defenders_table[10][10], int &hits) {
    struct ship_part {
        int place;
        bool isMarked;
    };
    //system("cls");
    int turn = attacker->play();
    while (defenders_table[turn / 10][turn % 10]) {
        int x_cur = turn / 10, y_cur = turn % 10;
        std::list<ship_part> ship;
        ship_part first_part{x_cur * 10 + y_cur, false};
        ship.push_back(first_part);
        bool isAllShip = false;
        while (!isAllShip) {
            isAllShip = true;
            for (auto &iter : ship) {
                if (!iter.isMarked) {
                    iter.isMarked = true;
                    isAllShip = false;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            if (iter.place / 10 + i <= 9 && iter.place / 10 + i >= 0 && iter.place % 10 + j <= 9 && iter.place % 10 + j >= 0) {
                                bool isAlreadyPart = false;
                                for (auto &iterator : ship) {
                                    isAlreadyPart |= (iterator.place == (iter.place + i*10 + j));
                                }
                                if (defenders_table[iter.place / 10 + i][iter.place % 10 + j] == 1 && !isAlreadyPart) {
                                    ship.push_back({(iter.place + i*10 + j), false});
                                }
                            }
                        }
                    }
                }
            }
        }
        std::cout << std::endl;
        isAllShip = true;
        for (auto &iter : ship) {
            if (attacker->getAttacktable(iter.place / 10, iter.place % 10) == 0 && iter.place != turn) {
                isAllShip = false;
            }
        }
        hits++;
        if (isAllShip) {
            attacker->turn_result(KILL, turn);
            std::cout << "KILLED!\n";
        }
        else {
            attacker->turn_result(HIT, turn);
            std::cout << "HIT!\n";
        }
        //system("cls");
        turn = attacker->play();
    }
    attacker->turn_result(MISS, turn);
    std::cout << "MISS!\n";
}
void Game::doGame(int g_type1, int g_type2) {
    std::shared_ptr<Gamer> Gamer1(std::move(choose_Gamer(g_type1)));
    std::shared_ptr<Gamer> Gamer2(std::move(choose_Gamer(g_type2)));
    Gamer1->shipArrangement();
    while (!check_ships(Gamer1)) {
        std::cout << "Nepravilno :(\n";
        Gamer1->shipArrangement();
    }
    bool first_table[10][10];
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            first_table[i][j] = Gamer1->getShiptable(i, j);
        }
    }
    std::cout << "Pole prinyato OwO\n";
    Gamer2->shipArrangement();
    //system("cls");
    while (!check_ships(Gamer2)) {
        std::cout << "Nepravilno :(\n";
        Gamer2->shipArrangement();
    }
    bool second_table[10][10];
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            second_table[i][j] = Gamer2->getShiptable(i, j);
        }
    }
    std::cout << "Pole prinyato OwO\n";
    //system("cls");
    int hits1 = 0, hits2 = 0;
    while (hits1 != 20 && hits2 != 20) {
        Turn(Gamer1, second_table, hits1);
        if (hits1 == 20) {
            std::cout << "FIRST WON";
            break;
        }
        Turn(Gamer2, first_table, hits2);
        if (hits2 == 20) {
            std::cout << "SECOND WON";
            break;
        }
    }
}
Game::Game(int argc, char *argv[]) {
    argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
    option::Stats stats(usage, argc, argv);
    std::vector<option::Option> options(stats.options_max);
    std::vector<option::Option> buffer(stats.buffer_max);
    option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

    if (parse.error())
        exit(1);

    if (options[HELP] || argc == 0) {
        option::printUsage(std::cout, usage);
        exit(0);
    }
    int count = 1, g_type1 = 2, g_type2 = 2;
    for (int i = 0; i < parse.optionsCount(); ++i)
    {
        option::Option& opt = buffer[i];
        //fprintf(stdout, "Argument #%d is ", i);
        switch (opt.index())
        {
            case COUNT:
                count = atoi(opt.arg);
                break;
            case FIRST:
                if (strncmp(opt.arg, "random", 6) == 0) {
                    g_type1 = 2;
                } else if (strncmp(opt.arg, "console", 7) == 0) {
                    g_type1 = 1;
                } else if (strncmp(opt.arg, "optimal", 7) == 0) {
                    g_type1 = 3;
                }
                //std::cout << g_type1;
                break;
            case SECOND:
                if (strncmp(opt.arg, "random", 6) == 0) {
                    g_type1 = 2;
                } else if (strncmp(opt.arg, "console", 7) == 0) {
                    g_type1 = 1;
                } else if (strncmp(opt.arg, "optimal", 7) == 0) {
                    g_type1 = 3;
                }
                //std::cout << g_type1;
                break;
        }
    }
    for (int i = 0; i < count; ++i)
        doGame(g_type1, g_type2);
    /**/
}
