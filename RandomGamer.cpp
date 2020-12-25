#include "Game.h"
#pragma once
void RandomGamer::shipArrangement(){
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            attack_table[i][j] = 0;
        }
    }
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 9);
    int ship_table_int[10][10];
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            ship_table_int[i][j] = 0;
        }
    }
    for (int length = 4; length > 0; --length) {
        for (int amount = 0; amount < 5 - length; ++amount) {
            bool right_arrange = false;
            int x = 1, y = 1, rotate = 1;
            while (!right_arrange) {
                right_arrange = true;
                x = dist(mt);
                y = dist(mt);
                rotate = dist(mt) % 2;
                if (rotate == 1) {
                    if (x + length - 1 > 9) {
                        right_arrange = false;
                    } else
                        for (int i = x; i <= x + length - 1; ++i) {
                            if (ship_table_int[i][y] != 0) {
                                right_arrange = false;
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
        }
    }
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (ship_table_int[i][j] == 2) {
                ship_table[i][j] = true;
                std::cout <<"#";
            }
            else
                std::cout <<"0";
        }
        std::cout << std::endl;
    }
}

int RandomGamer::play(){
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
void RandomGamer::turn_result (int turn, int number) {
    if (turn == KILL)
    attack_table[number / 10][number % 10] = HIT;
    else
    attack_table[number / 10][number % 10] = turn;
}