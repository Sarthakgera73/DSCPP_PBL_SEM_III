#include "leaderboard_avl.h"
#include "avl_tree_module.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace leaderboard_avl {
typedef avl_tree_module::AVLTree<int,std::vector<std::string>> Leaderboard;

static void addPlayer(Leaderboard& board,std::map<std::string,int>& scores,const std::string& name,int score){
    if(scores.count(name)){
        int old=scores[name];std::vector<std::string> names;board.searchData(old,names);
        for(std::size_t i=0;i<names.size();++i)if(names[i]==name){names.erase(names.begin()+i);break;}
        if(names.empty())board.deleteData(old);else board.insertData(old,names);
    }
    std::vector<std::string> names;board.searchData(score,names);names.push_back(name);
    board.insertData(score,names);scores[name]=score;
}
static void removePlayer(Leaderboard& board,std::map<std::string,int>& scores,const std::string& name){
    if(!scores.count(name)){std::cout<<"Player not found.\n";return;}
    int score=scores[name];std::vector<std::string> names;board.searchData(score,names);
    for(std::size_t i=0;i<names.size();++i)if(names[i]==name){names.erase(names.begin()+i);break;}
    if(names.empty())board.deleteData(score);else board.insertData(score,names);
    scores.erase(name);std::cout<<"Player removed.\n";
}
static void top(Leaderboard& board,int limit){
    std::vector<std::pair<int,std::vector<std::string>>> a=board.getAll();
    if(a.empty()){std::cout<<"Leaderboard is empty.\n";return;}
    int rank=1;std::cout<<"Rank\tPlayer\t\tScore\n";
    for(int i=(int)a.size()-1;i>=0&&rank<=limit;--i)
        for(std::size_t j=0;j<a[i].second.size()&&rank<=limit;++j,++rank)
            std::cout<<rank<<"\t"<<a[i].second[j]<<"\t\t"<<a[i].first<<"\n";
}
static void rangeScores(Leaderboard& board,int lo,int hi){
    if(lo>hi){std::cout<<"Invalid range.\n";return;}
    std::vector<std::pair<int,std::vector<std::string>>> a=board.getRange(lo,hi);
    if(a.empty())std::cout<<"No players in range.\n";
    for(int i=(int)a.size()-1;i>=0;--i)
        for(std::size_t j=0;j<a[i].second.size();++j)std::cout<<a[i].second[j]<<" : "<<a[i].first<<"\n";
}
static void searchPlayer(Leaderboard& board,std::map<std::string,int>& scores,const std::string& name){
    if(!scores.count(name)){std::cout<<"Player not found.\n";return;}
    int score=scores[name];std::vector<std::string> names;board.searchData(score,names);
    std::vector<std::pair<int,std::vector<std::string>>> a=board.getAll();int rank=1;bool found=false;
    for(int i=(int)a.size()-1;i>=0&&!found;--i)for(std::size_t j=0;j<a[i].second.size();++j){
        if(a[i].second[j]==name){found=true;break;}++rank;
    }
    std::cout<<"Player: "<<name<<"\nScore: "<<score<<"\nRank: "<<rank<<"\n";
    if(names.size()>1){std::cout<<"Tied with: ";for(std::size_t i=0;i<names.size();++i){if(i)std::cout<<", ";std::cout<<names[i];}std::cout<<"\n";}
}
void run(){
    Leaderboard board;std::map<std::string,int> scores;
    addPlayer(board,scores,"Aman",720);addPlayer(board,scores,"Riya",910);addPlayer(board,scores,"Kabir",850);
    addPlayer(board,scores,"Neha",850);addPlayer(board,scores,"Rahul",780);addPlayer(board,scores,"Priya",960);
    addPlayer(board,scores,"Karan",690);addPlayer(board,scores,"Simran",880);addPlayer(board,scores,"Rohit",740);
    addPlayer(board,scores,"Ananya",930);addPlayer(board,scores,"Vikas",810);addPlayer(board,scores,"Ishita",990);
    addPlayer(board,scores,"Aditya",760);addPlayer(board,scores,"Meera",870);addPlayer(board,scores,"Arjun",950);
    int choice=-1,score,lo,hi;std::string name;
    while(choice!=0){
        std::cout<<"\n--- LEADERBOARD USING AVL ---\n1. Add/update player\n2. Remove player\n3. Top 10\n4. Score range\n5. Add random player\n6. Search player\n7. AVL structure\n8. Inorder traversal\n0. Back\nChoice: ";
        if(!(std::cin>>choice)){std::cin.clear();std::cin.ignore(10000,'\n');continue;}
        if(choice==1){std::cout<<"Name and score: ";if(!(std::cin>>name>>score)){std::cin.clear();std::cin.ignore(10000,'\n');continue;}addPlayer(board,scores,name,score);}
        else if(choice==2){std::cout<<"Name: ";std::cin>>name;removePlayer(board,scores,name);}
        else if(choice==3)top(board,10);
        else if(choice==4){std::cout<<"Low high: ";if(!(std::cin>>lo>>hi)){std::cin.clear();std::cin.ignore(10000,'\n');continue;}rangeScores(board,lo,hi);}
        else if(choice==5){name="Player"+std::to_string(std::rand()%900+100);score=std::rand()%1000;addPlayer(board,scores,name,score);std::cout<<name<<" added: "<<score<<"\n";}
        else if(choice==6){std::cout<<"Name: ";std::cin>>name;searchPlayer(board,scores,name);}
        else if(choice==7)board.displayStructure();
        else if(choice==8)board.displayTraversal();
        else if(choice!=0)std::cout<<"Wrong choice.\n";
    }
}
}
