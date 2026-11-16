#include "order_book_avl.h"
#include "avl_tree_module.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace order_book_avl {
typedef avl_tree_module::AVLTree<double,int> PriceBook;
struct Trade{double price;int quantity;std::string side;};

static void place(PriceBook& bids,PriceBook& asks,std::vector<Trade>& log,
                  const std::string& side,double price,int quantity){
    int remaining=quantity;
    if(side=="BUY"){
        while(remaining>0){double best;int available;if(!asks.getMinimum(best,available)||best>price)break;
            int traded=std::min(remaining,available);remaining-=traded;available-=traded;
            if(available==0)asks.deleteData(best);else asks.insertData(best,available);
            log.push_back(Trade{best,traded,"BUY"});std::cout<<"Matched "<<traded<<" at "<<best<<"\n";
        }
        if(remaining){int old=0;bids.searchData(price,old);bids.insertData(price,old+remaining);std::cout<<"Remaining BUY added.\n";}
    }else if(side=="SELL"){
        while(remaining>0){double best;int available;if(!bids.getMaximum(best,available)||best<price)break;
            int traded=std::min(remaining,available);remaining-=traded;available-=traded;
            if(available==0)bids.deleteData(best);else bids.insertData(best,available);
            log.push_back(Trade{best,traded,"SELL"});std::cout<<"Matched "<<traded<<" at "<<best<<"\n";
        }
        if(remaining){int old=0;asks.searchData(price,old);asks.insertData(price,old+remaining);std::cout<<"Remaining SELL added.\n";}
    }else std::cout<<"Side must be BUY or SELL.\n";
}
static void showBook(PriceBook& bids,PriceBook& asks){
    std::vector<std::pair<double,int>> a=asks.getAll(),b=bids.getAll();
    std::cout<<"\nASKS (lowest first)\n";
    for(std::size_t i=0;i<a.size();++i)std::cout<<std::fixed<<std::setprecision(2)<<a[i].first<<" x "<<a[i].second<<"\n";
    std::cout<<"BIDS (highest first)\n";
    for(int i=(int)b.size()-1;i>=0;--i)std::cout<<std::fixed<<std::setprecision(2)<<b[i].first<<" x "<<b[i].second<<"\n";
}
void run(){
    PriceBook bids,asks;std::vector<Trade> log;
    bids.insertData(99.00,25);bids.insertData(98.50,30);asks.insertData(101.00,20);asks.insertData(102.00,40);
    int choice=-1,qty;double price;std::string side;
    while(choice!=0){
        std::cout<<"\n--- ORDER BOOK USING AVL ---\n1. Place order\n2. Random order\n3. Show order book\n4. Trade log\n5. BID/ASK tree structures\n6. BID/ASK inorder traversals\n0. Back\nChoice: ";
        if(!(std::cin>>choice)){std::cin.clear();std::cin.ignore(10000,'\n');continue;}
        if(choice==1){std::cout<<"Side BUY/SELL, price, quantity: ";if(!(std::cin>>side>>price>>qty)){std::cin.clear();std::cin.ignore(10000,'\n');continue;}
            if(price<=0||qty<=0)std::cout<<"Price and quantity must be positive.\n";else place(bids,asks,log,side,price,qty);}
        else if(choice==2){side=std::rand()%2?"BUY":"SELL";price=95.0+(std::rand()%1001)/100.0;qty=std::rand()%40+1;std::cout<<"Random "<<side<<" "<<qty<<" @ "<<price<<"\n";place(bids,asks,log,side,price,qty);}
        else if(choice==3)showBook(bids,asks);
        else if(choice==4){if(log.empty())std::cout<<"No trades.\n";for(std::size_t i=0;i<log.size();++i)std::cout<<log[i].side<<" matched "<<log[i].quantity<<" at "<<log[i].price<<"\n";}
        else if(choice==5){std::cout<<"BID TREE\n";bids.displayStructure();std::cout<<"ASK TREE\n";asks.displayStructure();}
        else if(choice==6){std::cout<<"BID ";bids.displayTraversal();std::cout<<"ASK ";asks.displayTraversal();}
        else if(choice!=0)std::cout<<"Wrong choice.\n";
    }
}
}
