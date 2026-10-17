
#include "order_book_module.h"
#include "skip_list_module.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

namespace order_book_module
{
    typedef skip_list_module::SkipList<double, int> PriceBook;

    struct Trade
    {
        double price;
        int quantity;
        string side;
    };

    void placeOrder(PriceBook &bidBook, PriceBook &askBook,
                    vector<Trade> &tradeLog, string side,
                    double price, int quantity)
    {
        int remaining = quantity;

        if (side == "BUY")
        {
            while (remaining > 0)
            {
                double bestPrice;
                int available;

                if (!askBook.getMinimum(bestPrice, available) || bestPrice > price)
                {
                    break;
                }

                int traded = remaining < available ? remaining : available;
                remaining -= traded;
                available -= traded;

                if (available == 0)
                {
                    askBook.deleteData(bestPrice);
                }
                else
                {
                    askBook.insertData(bestPrice, available);
                }

                tradeLog.push_back({bestPrice, traded, "BUY"});
                cout << "Matched " << traded << " at " << bestPrice << endl;
            }

            if (remaining > 0)
            {
                int oldQuantity = 0;
                bidBook.searchData(price, oldQuantity);
                bidBook.insertData(price, oldQuantity + remaining);
                cout << "Remaining BUY order added to book." << endl;
            }
        }
        else if (side == "SELL")
        {
            while (remaining > 0)
            {
                double bestPrice;
                int available;

                if (!bidBook.getMaximum(bestPrice, available) || bidBook.getMaximum(bestPrice, available) == false || bestPrice < price)
                {
                    break;
                }

                int traded = remaining < available ? remaining : available;
                remaining -= traded;
                available -= traded;

                if (available == 0)
                {
                    bidBook.deleteData(bestPrice);
                }
                else
                {
                    bidBook.insertData(bestPrice, available);
                }

                tradeLog.push_back({bestPrice, traded, "SELL"});
                cout << "Matched " << traded << " at " << bestPrice << endl;
            }

            if (remaining > 0)
            {
                int oldQuantity = 0;
                askBook.searchData(price, oldQuantity);
                askBook.insertData(price, oldQuantity + remaining);
                cout << "Remaining SELL order added to book." << endl;
            }
        }
        else
        {
            cout << "Side must be BUY or SELL." << endl;
        }
    }

    void showOrderBook(PriceBook &bidBook, PriceBook &askBook)
    {
        vector<pair<double, int>> asks = askBook.getAll();
        vector<pair<double, int>> bids = bidBook.getAll();

        cout << "\n---------- ORDER BOOK ----------" << endl;
        cout << "ASKS (lowest price first)" << endl;
        for (int i = 0; i < (int)asks.size(); i++)
        {
            cout << asks[i].first << " x " << asks[i].second << endl;
        }

        cout << "BIDS (highest price first)" << endl;
        for (int i = (int)bids.size() - 1; i >= 0; i--)
        {
            cout << bids[i].first << " x " << bids[i].second << endl;
        }
        cout << "--------------------------------" << endl;
    }

    void showTradeLog(vector<Trade> &tradeLog)
    {
        if (tradeLog.empty())
        {
            cout << "No trade has taken place." << endl;
            return;
        }

        cout << "\nTrade Log:" << endl;
        for (int i = 0; i < (int)tradeLog.size(); i++)
        {
            cout << tradeLog[i].side << " matched " << tradeLog[i].quantity
                 << " at " << tradeLog[i].price << endl;
        }
    }

    void run()
    {
        PriceBook bidBook;
        PriceBook askBook;
        vector<Trade> tradeLog;
        int choice = -1;
        int quantity;
        double price;
        string side;
        bool traceMode = false;

        bidBook.insertData(99.00, 25);
        bidBook.insertData(98.50, 30);
        askBook.insertData(101.00, 20);
        askBook.insertData(102.00, 40);

        while (choice != 0)
        {
            cout << "\n--- ORDER BOOK DEMO ---" << endl;
            cout << "Trace Mode: "
                 << (traceMode ? "ON" : "OFF") << endl;

            cout << "1. Place Order" << endl;
            cout << "2. Place Random Order" << endl;
            cout << "3. Show Order Book" << endl;
            cout << "4. Show Trade Log" << endl;
            cout << "5. Show Skip List Structure" << endl;
            cout << "6. Show Skip List Traversal" << endl;
            cout << "7. Toggle Trace Mode ON/OFF" << endl;
            cout << "0. Back" << endl;
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1)
            {
                cout << "Enter side (BUY/SELL): ";
                cin >> side;
                cout << "Enter price: ";
                cin >> price;
                cout << "Enter quantity: ";
                cin >> quantity;

                if (price <= 0 || quantity <= 0)
                {
                    cout << "Price and quantity must be positive." << endl;
                }
                else
                {
                    placeOrder(bidBook, askBook, tradeLog, side, price, quantity);
                }
            }
            else if (choice == 2)
            {
                side = rand() % 2 == 0 ? "BUY" : "SELL";
                price = 95.0 + (rand() % 1001) / 100.0;
                quantity = rand() % 40 + 1;
                cout << "Random order: " << side << " " << quantity << " at " << price << endl;
                placeOrder(bidBook, askBook, tradeLog, side, price, quantity);
            }
            else if (choice == 3)
            {
                showOrderBook(bidBook, askBook);
            }
            else if (choice == 4)
            {
                showTradeLog(tradeLog);
            }
            else if (choice == 5)
            {
                cout << "\n========== BID BOOK STRUCTURE ==========" << endl;
                bidBook.displayLevels();
                cout << "\n========== ASK BOOK STRUCTURE ==========" << endl;
                askBook.displayLevels();
            }
            else if (choice == 6)
            {
                cout << "\n========== BID BOOK TRAVERSAL ==========" << endl;
                bidBook.displayTraversal();
                cout << "\n========== ASK BOOK TRAVERSAL ==========" << endl;
                askBook.displayTraversal();
            }
            else if (choice == 7)
            {
                traceMode = !traceMode;

                cout << "Trace Mode is now "
                     << (traceMode ? "ON" : "OFF") << "." << endl;
            }
            else if (choice != 0)
            {
                cout << "Wrong choice." << endl;
            }
        }
    }
} // namespace order_book_module
