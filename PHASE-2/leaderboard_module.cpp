#include "leaderboard_module.h"
#include "skip_list_module.h"

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
using namespace std;

namespace leaderboard_module
{
    typedef skip_list_module::SkipList<int, vector<string>> Leaderboard;

    void addPlayer(Leaderboard &board, map<string, int> &playerScore,
                   string name, int score)
    {
        if (playerScore.count(name) == 1)
        {
            int oldScore = playerScore[name];
            vector<string> names;
            board.searchData(oldScore, names);

            for (int i = 0; i < (int)names.size(); i++)
            {
                if (names[i] == name)
                {
                    names.erase(names.begin() + i);
                    break;
                }
            }

            if (names.empty())
            {
                board.deleteData(oldScore);
            }
            else
            {
                board.insertData(oldScore, names);
            }
        }

        vector<string> names;
        board.searchData(score, names);
        names.push_back(name);
        board.insertData(score, names);
        playerScore[name] = score;
    }

    void deletePlayer(Leaderboard &board, map<string, int> &playerScore,
                      string name)
    {
        if (playerScore.count(name) == 0)
        {
            cout << "Player not found." << endl;
            return;
        }

        int score = playerScore[name];
        vector<string> names;
        board.searchData(score, names);

        for (int i = 0; i < (int)names.size(); i++)
        {
            if (names[i] == name)
            {
                names.erase(names.begin() + i);
                break;
            }
        }

        if (names.empty())
        {
            board.deleteData(score);
        }
        else
        {
            board.insertData(score, names);
        }

        playerScore.erase(name);
        cout << "Player removed successfully." << endl;
    }

    void showLeaderboard(Leaderboard &board, int limit)
    {
        vector<pair<int, vector<string>>> all = board.getAll();
        int rank = 1;

        if (all.empty())
        {
            cout << "Leaderboard is empty." << endl;
            return;
        }

        cout << "\nRank\tPlayer\t\tScore" << endl;
        cout << "--------------------------------" << endl;

        for (int i = (int)all.size() - 1; i >= 0 && rank <= limit; i--)
        {
            for (int j = 0; j < (int)all[i].second.size() && rank <= limit; j++)
            {
                string name = all[i].second[j];
                cout << rank << "\t" << name
                     << string(name.length() < 16 ? 16 - name.length() : 1, ' ')
                     << all[i].first << endl;
                rank++;
            }
        }
    }

    void showPlayersInRange(Leaderboard &board, int low, int high)
    {
        vector<pair<int, vector<string>>> result = board.getRange(low, high);
        if (result.empty())
        {
            cout << "No player found in this score range." << endl;
            return;
        }

        cout << "Players with scores from " << low << " to " << high << ":" << endl;
        for (int i = (int)result.size() - 1; i >= 0; i--)
        {
            for (int j = 0; j < (int)result[i].second.size(); j++)
            {
                cout << result[i].second[j] << " : " << result[i].first << endl;
            }
        }
    }

    void run()
    {
        Leaderboard board;
        map<string, int> playerScore;
        int choice = -1;
        int score, low, high;
        string name;

        addPlayer(board, playerScore, "Aman", 720);
        addPlayer(board, playerScore, "Riya", 910);
        addPlayer(board, playerScore, "Kabir", 850);
        addPlayer(board, playerScore, "Neha", 850);

        while (choice != 0)
        {
            cout << "\n--- LEADERBOARD GAME ---" << endl;
            cout << "1. Add or Update Player" << endl;
            cout << "2. Remove Player" << endl;
            cout << "3. Show Top 10" << endl;
            cout << "4. Score Range Query" << endl;
            cout << "5. Add Random Player" << endl;
            cout << "0. Back" << endl;
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1)
            {
                cout << "Enter player name: ";
                cin >> name;
                cout << "Enter score: ";
                cin >> score;
                addPlayer(board, playerScore, name, score);
                cout << "Leaderboard updated." << endl;
            }
            else if (choice == 2)
            {
                cout << "Enter player name: ";
                cin >> name;
                deletePlayer(board, playerScore, name);
            }
            else if (choice == 3)
            {
                showLeaderboard(board, 10);
            }
            else if (choice == 4)
            {
                cout << "Enter low and high score: ";
                cin >> low >> high;
                showPlayersInRange(board, low, high);
            }
            else if (choice == 5)
            {
                name = "Player" + to_string(rand() % 900 + 100);
                score = rand() % 1000;
                addPlayer(board, playerScore, name, score);
                cout << name << " added with score " << score << "." << endl;
            }
            else if (choice != 0)
            {
                cout << "Wrong choice." << endl;
            }
        }
    }
} // namespace leaderboard_module
