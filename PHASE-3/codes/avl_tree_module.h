#ifndef AVL_TREE_MODULE_H
#define AVL_TREE_MODULE_H
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace avl_tree_module {
template<class K, class V>
class Node {
public:
    K key; V value;
    Node<K,V>* left;
    Node<K,V>* right;
    int height;
    Node(const K& k, const V& v): key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

template<class K, class V>
class AVLTree {
    Node<K,V>* root;

    int h(Node<K,V>* n) const { return n ? n->height : 0; }
    int bf(Node<K,V>* n) const { return n ? h(n->left)-h(n->right) : 0; }
    void update(Node<K,V>* n) {
        if (n) n->height = 1 + std::max(h(n->left), h(n->right));
    }
    Node<K,V>* rotateRight(Node<K,V>* y) {
        Node<K,V>* x = y->left;
        Node<K,V>* t = x->right;
        x->right = y; y->left = t;
        update(y); update(x);
        return x;
    }
    Node<K,V>* rotateLeft(Node<K,V>* x) {
        Node<K,V>* y = x->right;
        Node<K,V>* t = y->left;
        y->left = x; x->right = t;
        update(x); update(y);
        return y;
    }
    Node<K,V>* balance(Node<K,V>* n) {
        if (!n) return nullptr;
        update(n);
        int b = bf(n);
        if (b > 1) {
            if (bf(n->left) < 0) n->left = rotateLeft(n->left); // LR
            return rotateRight(n);                              // LL
        }
        if (b < -1) {
            if (bf(n->right) > 0) n->right = rotateRight(n->right); // RL
            return rotateLeft(n);                                  // RR
        }
        return n;
    }
    Node<K,V>* insert(Node<K,V>* n, const K& k, const V& v) {
        if (!n) return new Node<K,V>(k,v);
        if (k < n->key) n->left = insert(n->left,k,v);
        else if (n->key < k) n->right = insert(n->right,k,v);
        else { n->value = v; return n; }
        return balance(n);
    }
    Node<K,V>* minNode(Node<K,V>* n) const {
        while (n && n->left) n=n->left;
        return n;
    }
    Node<K,V>* erase(Node<K,V>* n, const K& k, bool& removed) {
        if (!n) return nullptr;
        if (k < n->key) n->left=erase(n->left,k,removed);
        else if (n->key < k) n->right=erase(n->right,k,removed);
        else {
            removed=true;
            if (!n->left || !n->right) {
                Node<K,V>* child = n->left ? n->left : n->right;
                delete n;
                return child;
            }
            Node<K,V>* s=minNode(n->right);
            n->key=s->key; n->value=s->value;
            bool ignored=false;
            n->right=erase(n->right,s->key,ignored);
        }
        return balance(n);
    }
    bool find(Node<K,V>* n,const K& k,V& v) const {
        while(n) {
            if(k<n->key) n=n->left;
            else if(n->key<k) n=n->right;
            else {v=n->value; return true;}
        }
        return false;
    }
    void all(Node<K,V>* n,std::vector<std::pair<K,V>>& out) const {
        if(!n) return;
        all(n->left,out); out.push_back(std::make_pair(n->key,n->value)); all(n->right,out);
    }
    void range(Node<K,V>* n,const K& lo,const K& hi,std::vector<std::pair<K,V>>& out) const {
        if(!n) return;
        if(lo<n->key) range(n->left,lo,hi,out);
        if(!(n->key<lo) && !(hi<n->key)) out.push_back(std::make_pair(n->key,n->value));
        if(n->key<hi) range(n->right,lo,hi,out);
    }
    void sideways(Node<K,V>* n,int depth) const {
        if(!n) return;
        sideways(n->right,depth+1);
        std::cout << std::string(depth*5,' ') << n->key
                  << " [h=" << n->height << ", BF=" << bf(n) << "]\n";
        sideways(n->left,depth+1);
    }
    void destroy(Node<K,V>* n) {
        if(!n) return;
        destroy(n->left); destroy(n->right); delete n;
    }
public:
    AVLTree():root(nullptr) {}
    ~AVLTree(){ destroy(root); }
    AVLTree(const AVLTree&)=delete;
    AVLTree& operator=(const AVLTree&)=delete;

    void insertData(const K& k,const V& v){ root=insert(root,k,v); }
    bool searchData(const K& k,V& v) const { return find(root,k,v); }
    bool deleteData(const K& k){ bool removed=false; root=erase(root,k,removed); return removed; }
    bool getMinimum(K& k,V& v) const {
        Node<K,V>* n=minNode(root); if(!n) return false; k=n->key; v=n->value; return true;
    }
    bool getMaximum(K& k,V& v) const {
        Node<K,V>* n=root; if(!n) return false;
        while(n->right) {
            n=n->right;
        }
        k=n->key;
        v=n->value;
        return true;
    }
    std::vector<std::pair<K,V>> getAll() const {
        std::vector<std::pair<K,V>> out; all(root,out); return out;
    }
    std::vector<std::pair<K,V>> getRange(const K& lo,const K& hi) const {
        std::vector<std::pair<K,V>> out; if(hi<lo) return out; range(root,lo,hi,out); return out;
    }
    void displayData() const {
        std::vector<std::pair<K,V>> out=getAll();
        if(out.empty()){std::cout<<"AVL Tree is empty.\n";return;}
        for(std::size_t i=0;i<out.size();++i) std::cout<<out[i].first<<" : "<<out[i].second<<"\n";
    }
    void displayTraversal() const {
        std::vector<std::pair<K,V>> out=getAll();
        std::cout<<"Inorder: ";
        for(std::size_t i=0;i<out.size();++i){if(i)std::cout<<" -> ";std::cout<<out[i].first;}
        std::cout<<"\n";
    }
    void displayStructure() const {
        if(!root){std::cout<<"AVL Tree is empty.\n";return;}
        std::cout<<"AVL Tree (right subtree is above; left subtree is below):\n";
        sideways(root,0);
    }
    bool isEmpty() const { return root==nullptr; }
};
}
#endif
