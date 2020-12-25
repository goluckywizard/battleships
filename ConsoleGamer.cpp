#include "Game.h"
#pragma once
void ConsoleGamer::shipArrangement()  {
    std::cout << "Vvedite svoe pole\n";
    for (int i = 0; i < 10; ++i) {
        std::string line;
        getline(std::cin, line);
        while (line.size() < 9) {
            std::cout << "Not enough fields for this line\n";
            getline(std::cin, line);
        }
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
