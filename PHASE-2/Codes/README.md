## Project Details
Project Name: Skip List Data Structure with Real-World Applications
Course: Data Structures & Algorithms (DSA) / Object-Oriented Programming (OOP)
What we built: A custom Skip List in C++ used for two demos: a gaming leaderboard and a financial stock market order book.



## "Why a Skip List?"
A regular linked list takes O(n) time to find an item because you have to check nodes one by one.
Balanced trees (like AVL or Red-Black trees) take O(log n) time, but they require complex tree rotations to stay balanced.
A Skip List uses "express lanes" (levels). By flipping a digital coin (probability p = 0.5) some nodes become taller and jump over multiple elements. This gives you fast
O(log n) search and insert speeds without complicated rotations.



## How our Core Code Works
Coin Toss (getRandomLevel): Explains how a node decides its height.
The Update Array (update[]): When inserting or deleting a number, the code keeps track of which nodes need their forward pointers reconnected at each level.
Clean Deletion (deleteData): Reconnects the pointers around the removed node and shrinks currentLevel if the top level becomes empty.
Level Display (displayLevels): Prints the visual ASCII chart showing which numbers are on Level 0, Level 1, Level 2, etc.



## our Two Demos
## 1. Leaderboard Game Demo
Uses scores as the Skip List keys.
Uses a std::vector<string> for the values so multiple players with the same score are tied together instead of overwriting each other.
Uses a map (playerScore) to quickly find what score a player currently has when they want to update it.

## 2. Limit Order Book Demo
Models a real stock/crypto exchange with two Skip Lists: one for BUY orders (bidBook) and one for SELL orders (askBook).
When a buyer enters the market, it finds the lowest seller using getMinimum() and executes trades automatically.



## 🏛 Architectural Overview
Level 3 : HEAD ---------------------------------------------> [850] -------------> NULL
           |                                                    |
Level 2 : HEAD -------------------------> [720] ------------> [850] -------------> NULL
           |                                |                   |
Level 1 : HEAD -----------> [100] ------> [720] ------------> [850] ------> [910] -> NULL
           |                  |             |                   |             |
Level 0 : HEAD -> [50] -> [100] -> [250]-> [720] -> [800] -> [850] -> [890] -> [910] -> NULL



## Add Compilation Instructions
g++ -std=c++11 -O2 skiplist_phase2_main.cpp basic_module.cpp leaderboard_module.cpp order_book_module.cpp -o skiplist_app
./skiplist_app

