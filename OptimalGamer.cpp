#include "Game.h"
#pragma once

void OptimalGamer::shipArrangement() {
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
    bounds[0] = true;
    bounds[1] = true;
    bounds[2] = true;
    bounds[3] = true;
    for (int i = 0; i < ships[0]; ++i) {
        bounds[5 + i] = true;
    }
    for (int bound = 1; bound < 4; ++bound) {
        int place = 1;
        for (place = 1; place < 1 + ships[bound * 2 - 1]; ++place) {
            bounds[bound * 10 + place] = true;
        }
        ++place;
        int k = place;
        for (place; place < k + ships[bound * 2]; ++place) {
            bounds[bound * 10 + place] = true;
        }
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
        ship_table[i][9] = bounds[10 + i];
        ship_table[9][i] = bounds[29 - i];
        ship_table[i][0] = bounds[39 - i];
    }
    ship_table[0][0] = true;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (ship_table[i][j]) {
                std::cout << "#";
            } else
                std::cout << "0";
        }
        std::cout << std::endl;
    }
}

int OptimalGamer::play(){
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
