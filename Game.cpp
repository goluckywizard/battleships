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
bool Game::check_ships(bool table[10][10]) {
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

void Game::Turn(const std::shared_ptr<Gamer>& attacker, const std::shared_ptr<Gamer>& defender, int &hits) {
    struct ship_part {
        int place;
        bool isMarked;
    };
    //system("cls");
    int turn = attacker->play();
    while ((*defender).ship_table[turn / 10][turn % 10]) {
        bool isKill = true;
        int x_cur = turn / 10, y_cur = turn % 10;
        //std::pair<int, bool> ship_part;
        std::list<ship_part> ship;
        ship_part first_part{x_cur * 10 + y_cur, false};
        //first_part.isMarked = true;
        //first_part.place = x_cur * 10 + y_cur;
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
                                if (defender->ship_table[iter.place / 10 + i][iter.place % 10 + j] == 1 && !isAlreadyPart) {
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
            if (attacker->attack_table[iter.place / 10][iter.place % 10] == 0 && iter.place != turn) {
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
    while (!check_ships(Gamer1->ship_table)) {
        std::cout << "Nepravilno :(\n";
        Gamer1->shipArrangement();
    }
    std::cout << "Pole prinyato OwO\n";
    Gamer2->shipArrangement();
    //system("cls");
    while (!check_ships(Gamer2->ship_table)) {
        std::cout << "Nepravilno :(\n";
        Gamer2->shipArrangement();
    }
    std::cout << "Pole prinyato OwO\n";
    //system("cls");
    int hits1 = 0, hits2 = 0;
    while (hits1 != 20 && hits2 != 20) {
        Turn(Gamer1, Gamer2, hits1);
        if (hits1 == 20) {
            std::cout << "FIRST WON";
            break;
        }
        Turn(Gamer2, Gamer1, hits2);
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
                } else if (strncmp(opt.arg, "optimal", 7)) {
                    g_type1 = 3;
                }
                //std::cout << g_type1;
                break;
            case SECOND:
                if (opt.arg == "random") {
                    g_type2 = 2;
                } else if (opt.arg == "console") {
                    g_type2 = 1;
                } else if (opt.arg == "optimal") {
                    g_type2 = 3;
                }
                break;
        }
    }
    for (int i = 0; i < count; ++i)
        doGame(g_type1, g_type2);
    /**/
}

void OptimalGamer::turn_result (int turn, int number) {
    if (turn == KILL) {
        //attack_table[number / 10][number % 10] = HIT;
        std::list<ship_part> ship;
        ship_part first_part{number, false};
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
                                if (this->ship_table[iter.place / 10 + i][iter.place % 10 + j] == 1 && !isAlreadyPart) {
                                    ship.push_back({(iter.place + i*10 + j), false});
                                }
                            }
                        }
                    }
                }
            }
        }
        for (auto &iter : ship) {
            attack_table[iter.place / 10][iter.place % 10] = KILL;
        }
    }
    else
        attack_table[number / 10][number % 10] = turn;
}
void RandomGamer::shipArrangement(){
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
    std::uniform_int_distribution<int> dist(0, 9);
    //std::cout << dist(mt);
    int ship_table_int[10][10];
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            ship_table_int[i][j] = 0;
        }
    }
    for (int length = 4; length > 0; --length) {
        for (int amount = 0; amount < 5 - length; ++amount) {
            /*for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    std::cout << ship_table_int[i][j];
                }
                std::cout << std::endl;
            }*/
            //int x = std::rand() % 10;
            bool right_arrange = false;
            int x = 1, y = 1, rotate = 1;
            while (!right_arrange) {
                right_arrange = true;
                x = dist(mt);
                y = dist(mt);
                rotate = dist(mt) % 2;
                /*x = rand() % 10;
                y = rand() % 10;
                rotate = rand() % 2;*/
                /*if (ship_table_int[x][y] != 0) {
                    right_arrange = false;
                    continue;
                }*/
                //std::cout << "x:" << x << " y:" << y << " rotate:" << rotate << " length:" << length << " amount:" << amount << "   \n";
                if (rotate == 1) {
                    if (x + length - 1 > 9) {
                        right_arrange = false;
                    } else
                        for (int i = x; i <= x + length - 1; ++i) {
                            if (ship_table_int[i][y] != 0) {
                                right_arrange = false;
                                //std::cout << length << amount << i << y << ship_table_int[i][y] << "pizdec\n";
                            }
                        }
                } else {
                    if (y + length - 1 > 9) {
                        right_arrange = false;
                    } else
                        for (int i = y; i <= y + length - 1; ++i) {
                            if (ship_table_int[x][i] != 0) {
                                right_arrange = false;
                            }
                        }
                }
                //if (!right_arrange) {std::cout << "hochuumeret";}
            }

            if (rotate == 1) {
                if (x + length - 1 <= 9) {
                    for (int i = x; i <= x + length - 1; ++i) {
                        ship_table_int[i][y] = 2;
                    }
                }
            } else if (y + length - 1 <= 9) {
                for (int i = y; i <= y + length - 1; ++i) {
                    ship_table_int[x][i] = 2;
                }
            }
            for (int i = 1; i < 9; ++i) {
                if (ship_table_int[i][0] == 0 &&
                    (ship_table_int[i + 1][1] == 2 || ship_table_int[i][1] == 2 || ship_table_int[i - 1][1] == 2 || \
                                            ship_table_int[i + 1][0] == 2 || ship_table_int[i - 1][0] == 2)) {
                    ship_table_int[i][0] = 1;
                }
                if (ship_table_int[0][i] == 0 &&
                    (ship_table_int[1][i + 1] == 2 || ship_table_int[1][i] == 2 || ship_table_int[1][i - 1] == 2 || \
                                            ship_table_int[0][i + 1] == 2 || ship_table_int[0][i - 1] == 2)) {
                    ship_table_int[0][i] = 1;
                }
            }
            for (int i = 1; i < 9; ++i) {
                if (ship_table_int[i][9] == 0 &&
                    (ship_table_int[i + 1][9] == 2 || ship_table_int[i][8] == 2 || ship_table_int[i - 1][8] == 2 || \
                                            ship_table_int[i + 1][9] == 2 || ship_table_int[i - 1][9] == 2)) {
                    ship_table_int[i][9] = 1;
                }
                if (ship_table_int[9][i] == 0 &&
                    (ship_table_int[9][i + 1] == 2 || ship_table_int[8][i] == 2 || ship_table_int[8][i - 1] == 2 || \
                                            ship_table_int[9][i + 1] == 2 || ship_table_int[9][i - 1] == 2)) {
                    ship_table_int[9][i] = 1;
                }
            }
            if (ship_table_int[0][0] == 0 && (ship_table_int[0][1] == 2 || ship_table_int[1][0] == 2)) {
                ship_table_int[0][0] = 1;
            }
            if (ship_table_int[9][0] == 0 && (ship_table_int[9][1] == 2 || ship_table_int[8][0] == 2)) {
                ship_table_int[9][0] = 1;
            }
            if (ship_table_int[0][9] == 0 && (ship_table_int[0][8] == 2 || ship_table_int[1][9] == 2)) {
                ship_table_int[0][9] = 1;
            }
            if (ship_table_int[9][9] == 0 && (ship_table_int[9][8] == 2 || ship_table_int[8][9] == 2)) {
                ship_table_int[9][9] = 1;
            }
            for (int i = 1; i < 9; ++i) {
                for (int j = 1; j < 9; ++j) {
                    if (ship_table_int[i][j] == 0 &&
                        (ship_table_int[i + 1][j + 1] == 2 || ship_table_int[i][j + 1] == 2 ||
                         ship_table_int[i - 1][j + 1] == 2 || \
                                                    ship_table_int[i + 1][j] == 2 || ship_table_int[i][j] == 2 ||
                         ship_table_int[i - 1][j] == 2 || \
                                                    ship_table_int[i + 1][j - 1] == 2 || ship_table_int[i][j - 1] == 2 ||
                         ship_table_int[i - 1][j - 1] == 2)) {
                        ship_table_int[i][j] = 1;
                    }
                }
            }
            /*for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    std::cout << ship_table_int[i][j];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;*/
        }
    }
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (ship_table_int[i][j] == 2) {
                ship_table[i][j] = true;
                std::cout <<"#";
            }
            else std::cout <<"0";
        }
        std::cout << std::endl;
    }
}
void ConsoleGamer::shipArrangement()  {
    std::cout << "Vvedite svoe pole\n";
    for (int i = 0; i < 10; ++i) {
        std::string line;
        getline(std::cin, line);
        for (int j = 0; j < 10; ++j) {
            ship_table[i][j] = (line[j] == '*');
        }
    }
}
int ConsoleGamer::play() {
    std::cout << "   ABCDEFGHIJ\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << std::setw(3) << std::left << i+1;
        for (int j = 0; j < 10; ++j) {
            switch (attack_table[i][j]) {
                case UNKNOWN:
                    std::cout << "0";
                    break;
                case MISS:
                    std::cout << "#";
                    break;
                case HIT:
                    std::cout << "*";
                    break;
                case KILL:
                    std::cout << "X";
                    break;
            }
        }
        std::cout << std::endl;
    }
    std::string turn;
    std::cin >> turn;
    int value = 0;
    if (turn.size() < 3) {
        value = (int(turn[0]) - 65) + (int(turn[1]) - 49) * 10;
    } else if (turn.size() == 3) {
        value = (int(turn[0]) - 65) + 9 * 10;
    } else {
        value = 0;
    }
    //value = (int(letter) - 65) + (int(number) - 49) * 10;
    //std::cout << value;
    return value;
}

void ConsoleGamer::turn_result(int turn, int number){
    struct ship_part {
        int place;
        bool isMarked;
    };
    if (turn == KILL) {
        //attack_table[number / 10][number % 10] = HIT;
        std::list<ship_part> ship;
        ship_part first_part{number, false};
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
                                if (attack_table[iter.place / 10 + i][iter.place % 10 + j] == 2 && !isAlreadyPart) {
                                    ship.push_back({(iter.place + i*10 + j), false});
                                }
                            }
                        }
                    }
                }
            }
        }
        for (auto &iter : ship) {
            attack_table[iter.place / 10][iter.place % 10] = KILL;
        }
    }
    else
        attack_table[number / 10][number % 10] = turn;
}