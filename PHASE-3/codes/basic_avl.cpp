#include "basic_avl.h"
#include "avl_tree_module.h"
#include <iostream>
#include <string>
#include <vector>

namespace basic_avl {
void run() {
    avl_tree_module::AVLTree<int,std::string> data;
    for(int i=1;i<=10;++i) data.insertData(i,std::to_string(i*10));
    int choice=-1,key,low,high; std::string value;
    while(choice!=0) {
        std::cout<<"\n--- BASIC AVL TREE ---\n1. SET\n2. GET\n3. DEL\n4. RANGE\n5. Display pairs\n6. Display tree structure\n7. Inorder traversal\n0. Back\nChoice: ";
        if(!(std::cin>>choice)){std::cin.clear();std::cin.ignore(10000,'\n');std::cout<<"Invalid input.\n";continue;}
        if(choice==1){
            std::cout<<"Key: "; if(!(std::cin>>key)){std::cin.clear();std::cin.ignore(10000,'\n');continue;}
            std::cout<<"Value: ";std::cin>>value;data.insertData(key,value);std::cout<<"Stored.\n";
        } else if(choice==2){
            std::cout<<"Key: ";if(!(std::cin>>key)){std::cin.clear();std::cin.ignore(10000,'\n');continue;}
            if(data.searchData(key,value))std::cout<<"Value = "<<value<<"\n";else std::cout<<"Key not found.\n";
        } else if(choice==3){
            std::cout<<"Key: ";if(!(std::cin>>key)){std::cin.clear();std::cin.ignore(10000,'\n');continue;}
            std::cout<<(data.deleteData(key)?"Deleted.\n":"Key not found.\n");
        } else if(choice==4){
            std::cout<<"Low and high: ";if(!(std::cin>>low>>high)){std::cin.clear();std::cin.ignore(10000,'\n');continue;}
            if(low>high){std::cout<<"Invalid range.\n";continue;}
            std::vector<std::pair<int,std::string>> r=data.getRange(low,high);
            if(r.empty())std::cout<<"No results.\n";
            for(std::size_t i=0;i<r.size();++i)std::cout<<r[i].first<<" : "<<r[i].second<<"\n";
        } else if(choice==5)data.displayData();
        else if(choice==6)data.displayStructure();
        else if(choice==7)data.displayTraversal();
        else if(choice!=0)std::cout<<"Wrong choice.\n";
    }
}
}
