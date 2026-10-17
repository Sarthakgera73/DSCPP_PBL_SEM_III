#include "basic_module.h"
#include "leaderboard_module.h"
#include "order_book_module.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

int main()
{
    srand((unsigned int)time(0));
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n========== SKIP LIST PBL PHASE ==========" << endl;
        cout << "1. Basic Implementation of Skip List" << endl;
        cout << "2. Leaderboard Game" << endl;
        cout << "3. Order Book Demo" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            basic_module::run();
        }
        else if (choice == 2)
        {
            leaderboard_module::run();
        }
        else if (choice == 3)
        {
            order_book_module::run();
        }
        else if (choice != 0)
        {
            cout << "Wrong choice." << endl;
        }
    }

    cout << "Program ended." << endl;
    return 0;
}
