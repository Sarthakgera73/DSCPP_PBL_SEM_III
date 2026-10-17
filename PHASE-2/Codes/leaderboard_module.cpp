
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
                   string name, int score, bool trace = false)
    {
        if (playerScore.count(name) == 1)
        {
            int oldScore = playerScore[name];
            vector<string> names;
            board.searchData(oldScore, names, trace);

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
                board.insertData(oldScore, names, trace);
            }
        }

        vector<string> names;
        board.searchData(score, names, trace);
        names.push_back(name);
        board.insertData(score, names, trace);
        playerScore[name] = score;
    }

    void deletePlayer(Leaderboard &board, map<string, int> &playerScore,
                      string name, bool trace = false)
    {
        if (playerScore.count(name) == 0)
        {
            cout << "Player not found." << endl;
            return;
        }

        int score = playerScore[name];
        vector<string> names;
        board.searchData(score, names, trace);

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
            board.insertData(score, names, trace);
        }

        playerScore.erase(name);
        cout << "Player removed successfully." << endl;
    }

    void showLeaderboard(Leaderboard &board, int limit, bool trace)
    {
        if (trace)
        {
            cout << "\nCurrent Skip List structure:" << endl;
            board.displayLevels();
            cout << "Top scores are shown first; the list is read in reverse score order." << endl;
        }

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

    void showPlayersInRange(Leaderboard &board, int low, int high, bool trace)
    {
        if (low > high)
        {
            cout << "Invalid score range." << endl;
            return;
        }

        vector<pair<int, vector<string>>> result =
            board.getRange(low, high, trace);

        if (result.empty())
        {
            cout << "No player found in this score range." << endl;
            return;
        }

        cout << "Players with scores from " << low
             << " to " << high << ":" << endl;

        for (int i = (int)result.size() - 1; i >= 0; i--)
        {
            for (int j = 0; j < (int)result[i].second.size(); j++)
            {
                cout << result[i].second[j]
                     << " : " << result[i].first << endl;
            }
        }
    }

    void searchPlayer(Leaderboard &board,
                      map<string, int> &playerScore,
                      string name,
                      bool traceMode)
    {
        if (playerScore.count(name) == 0)
        {
            cout << "Player not found." << endl;
            return;
        }

        int score = playerScore[name];
        vector<string> names;

        if (!board.searchData(score, names, traceMode))
        {
            cout << "Player details could not be retrieved." << endl;
            return;
        }

        cout << "\n--- PLAYER DETAILS ---" << endl;
        cout << "Player: " << name << endl;
        cout << "Score:  " << score << endl;

        vector<pair<int, vector<string>>> all = board.getAll();
        int rank = 1;
        bool found = false;

        for (int i = (int)all.size() - 1; i >= 0 && !found; i--)
        {
            for (int j = 0; j < (int)all[i].second.size(); j++)
            {
                if (all[i].second[j] == name)
                {
                    found = true;
                    break;
                }

                rank++;
            }
        }

        if (found)
        {
            cout << "Rank:   " << rank << endl;
        }

        if (names.size() > 1)
        {
            cout << "Players tied at this score: ";

            for (int i = 0; i < (int)names.size(); i++)
            {
                if (i > 0)
                    cout << ", ";

                cout << names[i];
            }

            cout << endl;
        }
    }

    void run()
    {
        Leaderboard board;
        map<string, int> playerScore;

        int choice = -1;
        int score, low, high;
        string name;
        bool traceMode = false;

        // Initial leaderboard data
        addPlayer(board, playerScore, "Aman", 720);
        addPlayer(board, playerScore, "Riya", 910);
        addPlayer(board, playerScore, "Kabir", 850);
        addPlayer(board, playerScore, "Neha", 850);
        addPlayer(board, playerScore, "Rahul", 780);
        addPlayer(board, playerScore, "Priya", 960);
        addPlayer(board, playerScore, "Karan", 690);
        addPlayer(board, playerScore, "Simran", 880);
        addPlayer(board, playerScore, "Rohit", 740);
        addPlayer(board, playerScore, "Ananya", 930);
        addPlayer(board, playerScore, "Vikas", 810);
        addPlayer(board, playerScore, "Ishita", 990);
        addPlayer(board, playerScore, "Aditya", 760);
        addPlayer(board, playerScore, "Meera", 870);
        addPlayer(board, playerScore, "Arjun", 950);

        while (choice != 0)
        {
            cout << "\n--- LEADERBOARD GAME ---" << endl;
            cout << "Trace Mode: "
                 << (traceMode ? "ON" : "OFF") << endl;

            cout << "1. Add or Update Player" << endl;
            cout << "2. Remove Player" << endl;
            cout << "3. Show Top 10" << endl;
            cout << "4. Score Range Query" << endl;
            cout << "5. Add Random Player" << endl;
            cout << "6. Show Skip List Structure" << endl;
            cout << "7. Toggle Trace Mode ON/OFF" << endl;
            cout << "8. Search Player Details" << endl;
            cout << "9. Display Skip List Levels" << endl;
            cout << "10. Display Skip List Traversal" << endl;
            cout << "0. Back" << endl;

            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1)
            {
                cout << "Enter player name: ";
                cin >> name;

                cout << "Enter score: ";
                cin >> score;

                addPlayer(board, playerScore, name, score, traceMode);
                cout << "Leaderboard updated." << endl;
            }
            else if (choice == 2)
            {
                cout << "Enter player name: ";
                cin >> name;

                deletePlayer(board, playerScore, name, traceMode);
            }
            else if (choice == 3)
            {
                showLeaderboard(board, 10, traceMode);
            }
            else if (choice == 4)
            {
                cout << "Enter low and high score: ";
                cin >> low >> high;

                showPlayersInRange(board, low, high, traceMode);
            }
            else if (choice == 5)
            {
                name = "Player" + to_string(rand() % 900 + 100);
                score = rand() % 1000;

                addPlayer(board, playerScore, name, score, traceMode);

                cout << name << " added with score "
                     << score << "." << endl;
            }
            else if (choice == 6)
            {
                board.displayLevels();
            }
            else if (choice == 7)
            {
                traceMode = !traceMode;

                cout << "Trace Mode is now "
                     << (traceMode ? "ON" : "OFF")
                     << "." << endl;
            }
            else if (choice == 8)
            {
                cout << "Enter player name to search: ";
                cin >> name;

                searchPlayer(board, playerScore, name, traceMode);
            }
            else if (choice == 9)
            {
                board.displayLevels();
            }
            else if (choice == 10)
            {
                board.displayTraversal();
            }
            else if (choice != 0)
            {
                cout << "Wrong choice." << endl;
            }
        }
    }
}
