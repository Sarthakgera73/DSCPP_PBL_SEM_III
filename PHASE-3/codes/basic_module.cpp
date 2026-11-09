
#include "basic_module.h"
#include "skip_list_module.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

namespace basic_module
{
    void run()
    {
        skip_list_module::SkipList<int, string> data;

        // Sample data added for demonstration
        data.insertData(1, "10");
        data.insertData(2, "20");
        data.insertData(3, "30");
        data.insertData(4, "40");
        data.insertData(5, "50");
        data.insertData(6, "60");
        data.insertData(7, "70");
        data.insertData(8, "80");
        data.insertData(9, "90");
        data.insertData(10, "100");

        int choice = -1;
        int key, low, high;
        string value;
        bool traceMode = false;

        while (choice != 0)
        {
            cout << "\n--- BASIC SKIP LIST IMPLEMENTATION ---" << endl;
            cout << "Trace Mode: "
                 << (traceMode ? "ON" : "OFF") << endl;

            cout << "1. SET (Insert or Update)" << endl;
            cout << "2. GET (Search)" << endl;
            cout << "3. DEL (Delete)" << endl;
            cout << "4. RANGE Query" << endl;
            cout << "5. Display Key-Value Pairs" << endl;
            cout << "6. Display Skip List Levels" << endl;
            cout << "7. Display Skip List Traversal" << endl;
            cout << "8. Toggle Trace Mode ON/OFF" << endl;
            cout << "0. Back" << endl;

            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1)
            {
                cout << "Enter integer key: ";
                cin >> key;

                cout << "Enter value: ";
                cin >> value;

                data.insertData(key, value, traceMode);

                cout << "Value stored successfully." << endl;
            }
            else if (choice == 2)
            {
                cout << "Enter key to search: ";
                cin >> key;

                if (data.searchData(key, value, traceMode))
                {
                    cout << "Value = " << value << endl;
                }
                else
                {
                    cout << "Key not found." << endl;
                }
            }
            else if (choice == 3)
            {
                cout << "Enter key to delete: ";
                cin >> key;

                cout << (data.deleteData(key)
                    ? "Key deleted successfully."
                    : "Key not found.") << endl;
            }
            else if (choice == 4)
            {
                cout << "Enter low and high key: ";
                cin >> low >> high;

                if (low > high)
                {
                    cout << "Invalid range." << endl;
                    continue;
                }

                vector<pair<int, string>> result =
                    data.getRange(low, high, traceMode);

                if (result.empty())
                {
                    cout << "No key found in this range." << endl;
                }

                for (int i = 0; i < (int)result.size(); i++)
                {
                    cout << result[i].first << " : "
                         << result[i].second << endl;
                }
            }
            else if (choice == 5)
            {
                data.displayData();
            }
            else if (choice == 6)
            {
                data.displayLevels();
            }
            else if (choice == 7)
            {
                data.displayTraversal();
            }
            else if (choice == 8)
            {
                traceMode = !traceMode;

                cout << "Trace Mode is now "
                     << (traceMode ? "ON" : "OFF")
                     << "." << endl;
            }
            else if (choice != 0)
            {
                cout << "Wrong choice." << endl;
            }
        }
    }
}
