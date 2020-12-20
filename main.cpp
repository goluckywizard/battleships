#include <iostream>
#include <regex>
#include <memory>
#include <iomanip>
enum DotState {UNKNOWN = 0, MISS, HIT};
class Gamer {
public:
    bool ship_table[10][10];
    virtual void shipArrangement() = 0;
    virtual int play() = 0;
    virtual void turn_result(int turn, int number) = 0;
};

/*class RandomGamer : public Gamer{
    void shipArrangement(bool table[10][10]) override {

    }
    void play (bool **table) override {

    }
    RandomGamer() = default;
};*/
class ConsoleGamer : public Gamer {
private:
    int attack_table[10][10] = {0};
public:
    //bool ship_table[10][10] = {false};
     void shipArrangement() override {
         std::cout << "Vvedite svoe pole\n";
        for (int i = 0; i < 10; ++i) {
            std::string line;
            getline(std::cin, line);
            for (int j = 0; j < 10; ++j) {
                ship_table[i][j] = (line[j] == '*');
            }
        }
    }
    int play() override {
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
    void turn_result(int turn, int number) override {
         attack_table[number / 10][number % 10] = turn;
     }
};

std::unique_ptr<Gamer> choose_Gamer(int k = 1) {
    if (k == 1) {
        return std::make_unique<ConsoleGamer>();
    }
}

class Game {
    bool check_ships(bool table[10][10]) {
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
        /*for (int i = 0; i < 5; ++i) {
            std::cout << i << ": " << count_ships[i] << "\n";
        }*/
        if (count_ships[4] != 1 || count_ships[3] != 2 || count_ships[2] != 3)
            return false;
        if (count_ships[1] != 24)
            return false;

        return true;
    }

    void Turn(const std::shared_ptr<Gamer>& gamer, int &hits) {
        //system("cls");
        int turn = gamer->play();
        while ((*gamer).ship_table[turn / 10][turn % 10]) {
            gamer->turn_result(HIT, turn);
            hits++;
            std::cout << "HIT!\n";
            //system("cls");
            turn = gamer->play();
        }
        gamer->turn_result(MISS, turn);
        std::cout << "MISS!\n";
    }

    void doGame(const std::shared_ptr<Gamer>& gamer1, const std::shared_ptr<Gamer>& gamer2) {
        int hits1 = 0, hits2 = 0;
        while (hits1 != 20 && hits2 != 20) {
            Turn(gamer1, hits1);
            Turn(gamer2, hits2);
        }
    }
public:
    Game() {
        std::shared_ptr<Gamer> Gamer1 = std::move(choose_Gamer(1));
        std::shared_ptr<Gamer> Gamer2 = std::move(choose_Gamer(1));
        Gamer1->shipArrangement();
        while (!check_ships(Gamer1->ship_table)) {
            std::cout << "Nepravilno :(";
            Gamer1->shipArrangement();
        }
        Gamer2->shipArrangement();
        //system("cls");
        while (!check_ships(Gamer2->ship_table)) {
            std::cout << "Nepravilno :(";
            Gamer2->shipArrangement();
        }
        //system("cls");
        int hits1 = 0, hits2 = 0;
        while (hits1 != 20 && hits2 != 20) {
            Turn(Gamer1, hits1);
            Turn(Gamer2, hits2);
        }
    }
};
int main(int argc, char *argv[]) {
    Game first;
    return 0;
}
