#include "../headers/player.h"
#include "../headers/game.h"
#include "../headers/randomgame.h"
#include "../headers/seedgame.h"
#include "../headers/loadgame.h"
#include "../headers/boardgame.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

const int NUM_PLAYERS = 4;

string toUpper(string str);

int main(int argc, char *argv[]) {
    Game *game;
    bool enhancement = false;
    if (argc > 2) {
        string arg1 = argv[1];
        string arg2 = argv[2];
        if (arg1 == "-seed") {
            try {
                int num = std::stoi(arg2);
                game = new SeedGame(num);
            } catch (const std::invalid_argument& e) {
                cout << "Invalid Usage" << endl;
                return 1;
            }
        } else if (arg1 == "-load") {
            ifstream file {arg2};
            if (file.is_open()) { // chech if file is valid
                game = new LoadGame(arg2);
            } else {
                cout << "Invalid File" << endl;
                return 2;
            }
        } else if (arg1 == "-board") {
            ifstream file {arg2};
            if (file.is_open()) {
                game = new BoardGame(arg2);
            } else {
                cout << "Invalid File" << endl;
                return 2;
            }
        } else {
            cout << "Invalid Usage" << endl;
            return 1;
        }
    } else {
        game = new RandomGame;
    }
    string final_arg = argv[argc - 1];
    if (final_arg == "-enhance") {
        enhancement = true;
        game->enhance();
    }
    std::vector<Player *> players = {};
    std::vector<string> colours = {"Blue", "Red", "Orange", "Yellow"};
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        Player *player = new Player {game, colours[i], 0, {}, {}};
        players.emplace_back(player);
    }
    game->setUpGame(players);
    while (true) {
        if (cin.eof()) {
            break;
        }
        cout << game->getBoard();
        int assignment;
        string save_file;
        int i = 0;
        int counter = 0;
        bool increasing = true;
        bool collect = false;
        if (game->getTurn() == 0) {
            while (counter < NUM_PLAYERS * 2) {
                if (i >= NUM_PLAYERS) {
                    --i;
                    increasing = false;
                    collect = true;
                }
                cout << "Player " << players[i]->getName() << " where do you want to complete an Assignment? > ";
                if (cin >> assignment) {
                    if (!game->addFirstAssignment(players[i], assignment, collect)) {
                        cout << "You cannot build here" << endl;
                        continue;
                    }
                } else if (cin.eof()) {
                    break;
                } else {
                    cout << "Invalid Command" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores entire command
                    continue;
                }
                if (increasing) {
                    ++i;
                } else {
                    --i;
                }
                ++counter;
                cout << game->getBoard();
            } 
            game->incTurn();
        }       
        string command;
        int num;
        string colour;
        string res1;
        string res2;
        bool play_again;
        while (true) {
            if (cin.eof()) {
                break;
            }
            i = (game->getTurn() - 1) % NUM_PLAYERS; // Finds which player's turn it is
            cout << "Student " << players[i]->getName() << "'s turn." << endl;
            players[i]->getStatus(cout);
            cout << "> ";
            while (cin >> command) {
                if (command == "fair") {
                    players[i]->setFair();
                    cout << "> ";
                    continue;
                } else if (command == "load") {
                    players[i]->setLoaded();
                    cout << "> ";
                    continue;
                } else if (command == "roll") {
                    if (!players[i]->roll_dice(cin, cout)) {
                        cout << "Invalid roll." << endl;
                        cin.clear();
                        cin.ignore();
                        continue;
                    } else if (!game->gainedResources(players)) {
                        cout << "No students gained resources." << endl;
                        if (game->geeseRolled()) {
                            game->loseResources(players, cout);
                            while (true) {
                                cout << "Choose where to place the GEESE. > ";
                                if (cin >> num && game->moveGeese(num)) {
                                    if (game->adjacentStudents(num, players, players[i], cout)) {
                                        while (true) {
                                            cout << "Choose a student to steal from. > ";
                                            if (cin >> command && game->steal(players[i], players, command, cout, num)) {
                                                break;
                                            } else if (cin.eof()) {
                                                break;
                                            } else if (cin.fail()) {
                                                cin.clear();
                                                cin.ignore();
                                                cout << "Invalid player." << endl;
                                            } else {
                                                cout << "Invalid player." << endl;
                                            }
                                        }
                                    } else {
                                        cout << "Student " << players[i]->getName() << " has no students to steal from." << endl;
                                        break;
                                    }
                                } else if (cin.eof()) {
                                    break;
                                } else if (cin.fail()){
                                    cout << "Can't move the GEESE there." << endl;
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    continue;
                                } else {
                                    cout << "Can't move the GEESE there." << endl;
                                    continue;
                                }
                                break;
                            }
                        }
                    }
                } else {
                    cout << "Invalid Command" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "> ";
                    continue;
                }
                cout << "> ";
                game->incTurn();
                while (cin >> command) {
                    if (command == "board") {
                        cout << game->getBoard();
                    } else if (command == "status") {
                        game->getStatuses(players, cout);
                    } else if (command == "criteria") {
                        players[i]->printCriteria(cout);
                    } else if (command == "achieve") {
                        if (cin >> num && game->availableGoal(players[i], num)) {
                            if (!game->buyGoal(players[i], num)) {
                                cout << "You do not have enough resources." << endl;
                            }
                        } else if (cin) {
                            cout << "Invalid Goal" << endl;
                        } else if (cin.eof()) {
                            break;
                        } else {
                            cout << "Invalid Command" << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    } else if (command == "complete") {
                        if (cin >> num && game->availableCriterion(players[i], num)) {
                            if (!game->buyCriteria(players[i], num)) {
                                cout << "You do not have enough resources." << endl;
                            }
                        } else if (cin) {
                            cout << "Invalid Criterion" << endl;
                        } else if (cin.eof()) {
                            break;
                        } else {
                            cout << "Invalid Command" << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    } else if (command == "improve") {
                        if (cin >> num && game->improvableCriterion(players[i], num)) {
                            if (!game->improveCriterion(players[i], num)) {
                                cout << "You do not have enough resources." << endl;
                            }
                        } else if (cin) {
                            cout << "Invalid Criterion" << endl;
                        } else if (cin.eof()) {
                            break;
                        } else {
                            cout << "Invalid Command" << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    } else if (command == "trade") {
                        if (cin >> colour && cin >> res1 && cin >> res2 && 
                            game->tradeable(players[i], colour, res1, res2, players)) {
                            cout << players[i]->getName() << " offers " << colour << " one " << toUpper(res1);
                            cout << " for one " << toUpper(res2) << ". Does " << colour << " accept this offer?";
                            cout << " > ";
                            while (cin >> command) {
                                if (command == "yes") {
                                    if (game->trade(players[i], colour, res1, res2, players)) {
                                        cout << "The trade was successful" << endl;
                                        break;
                                    } else {
                                        cout << "One of the players does not have enough resources." << endl;
                                        break;
                                    }
                                } else if (command == "no") {
                                    break;
                                } else if (cin.eof()) {
                                    break;
                                } else {
                                    cout << "Invalid Response" << endl;
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                                cout << "> ";
                            }
                        } else if (cin) {
                            cout << "Invalid Trade Proposal" << endl;
                        } else if (cin.eof()) {
                            break;
                        } else {
                            cout << "Invalid Command" << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    } else if (command == "exchange" && enhancement) {
                        if (cin >> res1 && cin >> res2 && game->exchangeable(players[i], res1, res2)) {
                            if (game->exchange(players[i], res1, res2)) {
                                cout << players[i]->getName() << " traded " << TRADE_IN << " " << toUpper(res1);
                                cout << " for one " << toUpper(res2) << "." << endl;
                            } else {
                                cout << "You do not have enough resources." << endl;
                            }
                        } else if (cin) {
                            cout << "Invalid Exchange" << endl;
                        } else if (cin.eof()) {
                            break;
                        } else {
                            cout << "Invalid Command" << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    } else if (command == "next") {
                        break;
                    } else if (command == "save") {
                        if (cin >> save_file) {
                            game->save(players, save_file);
                        } else {
                            break;
                        }
                    } else if (command == "help") {
                        cout << "Valid commands:" << endl;
                        cout << "board" << endl;
                        cout << "status" << endl;
                        cout << "criteria" << endl;
                        cout << "achieve <goal>" << endl;
                        cout << "improve <criterion>" << endl;
                        cout << "trade <colour> <give> <take>" << endl;
                        if (enhancement) {
                            cout << "exchange <give> <receive>" << endl;
                        }
                        cout << "next" << endl;
                        cout << "save <file>" << endl;
                        cout << "help" << endl;
                    } else if (cin.eof()) {
                        break;
                    } else {
                        cout << "Invalid Command" << endl;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    if (game->winner(players[i])) {
                        cout << "Player " << players[i]->getName() << " Wins!" << endl;
                        cout << "Would you like to play again?" << endl;
                        cout << "> ";
                        while (cin >> command) {
                            if (command == "yes") {
                                play_again = true;
                                break;
                            } else if (command == "no") {
                                play_again = false;
                                break;
                            } else {
                                cout << "Invalid Response" << endl;
                                cout << "> ";
                            }
                        }
                        if (play_again) {
                            game->resetGame(players);
                            break;
                        } else {
                            for (int i = 0; i < NUM_PLAYERS; ++i) {
                            delete players[i];
                            }
                            delete game;
                            return 0;
                        }
                    }
                    cout << "> ";
                }
                break;
            }           
            if (cin.eof()) {
                break;
            }
            if (play_again) {
                play_again = false;
                break;
            }
            cout << game->getBoard();
        }
    }
    if (cin.eof()) {
        game->save(players, "backup.sv");
    }
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        delete players[i];
    }
    delete game;
}
