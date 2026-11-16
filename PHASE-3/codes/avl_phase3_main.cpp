#include "basic_avl.h"
#include "leaderboard_avl.h"
#include "order_book_avl.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main(){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int choice=-1;
    while(choice!=0){
        std::cout<<"\n========== AVL TREE PBL PHASE III ==========\n"
                 <<"1. Basic AVL Tree\n2. Leaderboard Game\n3. Order Book Demo\n0. Exit\nChoice: ";
        if(!(std::cin>>choice)){std::cin.clear();std::cin.ignore(10000,'\n');std::cout<<"Invalid input.\n";continue;}
        if(choice==1)basic_avl::run();
        else if(choice==2)leaderboard_avl::run();
        else if(choice==3)order_book_avl::run();
        else if(choice!=0)std::cout<<"Wrong choice.\n";
    }
    std::cout<<"Program ended.\n";
    return 0;
}
