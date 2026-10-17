#ifndef SKIP_LIST_MODULE_H
#define SKIP_LIST_MODULE_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace skip_list_module
{
const int MAX_LEVEL = 16;

template <class K, class V>
class Node
{
public:
    K key;
    V value;
    Node<K, V> **forward;
    int nodeLevel;

    Node(K key, V value, int nodeLevel)
    {
        this->key = key;
        this->value = value;
        this->nodeLevel = nodeLevel;
        forward = new Node<K, V> *[nodeLevel + 1];

        for (int i = 0; i <= nodeLevel; i++)
        {
            forward[i] = NULL;
        }
    }

    ~Node()
    {
        delete[] forward;
    }
};

template <class K, class V>
class SkipList
{
private:
    int currentLevel;
    Node<K, V> *head;

    int getRandomLevel()
    {
        int newLevel = 0;

        while (std::rand() % 2 == 1 && newLevel < MAX_LEVEL)
        {
            newLevel++;
        }

        return newLevel;
    }

public:
    SkipList()
    {
        currentLevel = 0;
        head = new Node<K, V>(K(), V(), MAX_LEVEL);
    }

    ~SkipList()
    {
        Node<K, V> *ptr = head;

        while (ptr != NULL)
        {
            Node<K, V> *next = ptr->forward[0];
            delete ptr;
            ptr = next;
        }
    }

    void insertData(K key, V value, bool trace = false)
    {
        Node<K, V> *update[MAX_LEVEL + 1];
        Node<K, V> *ptr = head;

        if (trace)
            std::cout << "Finding the insertion position for key " << key << ".\n";

        for (int i = currentLevel; i >= 0; i--)
        {
            if (trace) std::cout << "  Level " << i << ": start from current node and move right while next key < " << key << ".\n";
            while (ptr->forward[i] != NULL && ptr->forward[i]->key < key)
            {
                if (trace) std::cout << "    Move right to key " << ptr->forward[i]->key << ".\n";
                ptr = ptr->forward[i];
            }
            update[i] = ptr;
        }

        ptr = ptr->forward[0];
        if (ptr != NULL && ptr->key == key)
        {
            ptr->value = value;
            if (trace) std::cout << "  Key already exists; value updated.\n";
            return;
        }

        int newLevel = getRandomLevel();
        if (trace) std::cout << "  Random level generated for key " << key << " = " << newLevel << ".\n";
        if (newLevel > currentLevel)
        {
            for (int i = currentLevel + 1; i <= newLevel; i++)
            {
                update[i] = head;
            }
            currentLevel = newLevel;
        }

        Node<K, V> *newNode = new Node<K, V>(key, value, newLevel);
        for (int i = 0; i <= newLevel; i++)
        {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
            if (trace) std::cout << "  Connected key " << key << " at Level " << i << ".\n";
        }
        if (trace) std::cout << "Insertion complete.\n";
    }

    bool searchData(K key, V &value, bool trace = false)
    {
        Node<K, V> *ptr = head;

        if (trace)
        {
            std::cout << "\nTarget key: " << key << std::endl;
            std::cout << "Actual level-by-level path :" << std::endl;
        }

        for (int i = currentLevel; i >= 0; i--)
        {
            if (trace)
            {
                std::cout << "Level " << i << " : ";
                if (ptr == head)
                    std::cout << "HEAD";
                else
                    std::cout << ptr->key;
            }

            // Move right only along real forward pointers at this level.
            while (ptr->forward[i] != NULL && ptr->forward[i]->key < key)
            {
                ptr = ptr->forward[i];
                if (trace)
                    std::cout << " -> " << ptr->key;
            }

            if (trace)
            {
                if (ptr->forward[i] != NULL && ptr->forward[i]->key == key)
                {
                    std::cout << " -> " << ptr->forward[i]->key;
                }
                else if (ptr->forward[i] != NULL)
                {
                    std::cout << " -> " << ptr->forward[i]->key
                              << " (next is greater, stop rightward movement)";
                }
                else
                {
                    std::cout << " -> NULL (end)";
                }
                if (i > 0)
                    std::cout << "\n           | move down to Level " << i - 1;
                std::cout << std::endl;
            }
        }

        // At Level 0, the next node after the last smaller key is the candidate.
        ptr = ptr->forward[0];
        if (ptr != NULL && ptr->key == key)
        {
            value = ptr->value;
            if (trace)
                std::cout << "Found key " << key << " at Level 0." << std::endl;
            return true;
        }

        if (trace)
            std::cout << "Key " << key << " was not found." << std::endl;
        return false;
    }

    bool deleteData(K key)
    {
        Node<K, V> *update[MAX_LEVEL + 1];
        Node<K, V> *ptr = head;

        for (int i = currentLevel; i >= 0; i--)
        {
            while (ptr->forward[i] != NULL && ptr->forward[i]->key < key)
            {
                ptr = ptr->forward[i];
            }
            update[i] = ptr;
        }

        ptr = ptr->forward[0];
        if (ptr == NULL || ptr->key != key)
        {
            return false;
        }

        for (int i = 0; i <= currentLevel; i++)
        {
            if (update[i]->forward[i] != ptr)
            {
                break;
            }
            update[i]->forward[i] = ptr->forward[i];
        }

        delete ptr;
        while (currentLevel > 0 && head->forward[currentLevel] == NULL)
        {
            currentLevel--;
        }

        return true;
    }

    bool getMinimum(K &key, V &value)
    {
        if (head->forward[0] == NULL)
        {
            return false;
        }

        key = head->forward[0]->key;
        value = head->forward[0]->value;
        return true;
    }

    bool getMaximum(K &key, V &value)
    {
        Node<K, V> *ptr = head;

        for (int i = currentLevel; i >= 0; i--)
        {
            while (ptr->forward[i] != NULL)
            {
                ptr = ptr->forward[i];
            }
        }

        if (ptr == head)
        {
            return false;
        }

        key = ptr->key;
        value = ptr->value;
        return true;
    }

    std::vector<std::pair<K, V>> getRange(K low, K high, bool trace = false)
    {
        std::vector<std::pair<K, V>> result;
        Node<K, V> *ptr = head;
        if (trace) std::cout << "Locate the first key >= " << low << ".\n";

        for (int i = currentLevel; i >= 0; i--)
        {
            if (trace) std::cout << "  Searching at Level " << i << ".\n";
            while (ptr->forward[i] != NULL && ptr->forward[i]->key < low)
            {
                if (trace) std::cout << "    Skip key " << ptr->forward[i]->key << " because it is below " << low << ".\n";
                ptr = ptr->forward[i];
            }
        }

        ptr = ptr->forward[0];
        if (trace) std::cout << "  Scan Level 0 until key > " << high << ".\n";
        while (ptr != NULL && ptr->key <= high)
        {
            if (trace) std::cout << "    Include key " << ptr->key << ".\n";
            result.push_back(std::make_pair(ptr->key, ptr->value));
            ptr = ptr->forward[0];
        }
        if (trace) std::cout << "Range scan complete.\n";
        return result;
    }

    std::vector<std::pair<K, V>> getAll()
    {
        std::vector<std::pair<K, V>> result;
        Node<K, V> *ptr = head->forward[0];

        while (ptr != NULL)
        {
            result.push_back(std::make_pair(ptr->key, ptr->value));
            ptr = ptr->forward[0];
        }

        return result;
    }

    void displayData()
    {
        Node<K, V> *ptr = head->forward[0];
        if (ptr == NULL)
        {
            std::cout << "Skip list is empty." << std::endl;
            return;
        }

        std::cout << "Key-value pairs are:" << std::endl;
        while (ptr != NULL)
        {
            std::cout << ptr->key << " : " << ptr->value << std::endl;
            ptr = ptr->forward[0];
        }
    }

    void displayLevels()
    {
        // Show at least Levels 0-3, plus any higher randomized levels.
        int highestLevelToShow = (currentLevel < 3) ? 3 : currentLevel;
        std::vector<Node<K, V> *> nodes;
        Node<K, V> *ptr = head->forward[0];
        while (ptr != NULL)
        {
            nodes.push_back(ptr);
            ptr = ptr->forward[0];
        }

        const int cellWidth = 8;
        std::cout << "\nSkip List Structure (horizontal arrows = forward pointers)\n";
        std::cout << "Vertical bars connect the same node across levels.\n\n";

        // Print higher levels first. Each Level-0 node keeps a fixed column,
        // so vertical bars clearly show which nodes participate in upper levels.
        for (int level = highestLevelToShow; level >= 0; --level)
        {
            std::cout << "Level " << level << " : ";
            std::cout.width(cellWidth);
            std::cout << std::left << "HEAD";
            for (size_t i = 0; i < nodes.size(); ++i)
            {
                if (nodes[i]->nodeLevel >= level)
                {
                    std::string label = "-> " + std::to_string(nodes[i]->key);
                    std::cout.width(cellWidth);
                    std::cout << std::left << label;
                }
                else
                {
                    std::cout.width(cellWidth);
                    std::cout << std::left << " ";
                }
            }
            std::cout << "-> NULL\n";

            if (level > 0)
            {
                std::cout << "         ";
                std::cout.width(cellWidth);
                std::cout << std::left << "|";
                for (size_t i = 0; i < nodes.size(); ++i)
                {
                    std::cout.width(cellWidth);
                    std::cout << std::left << (nodes[i]->nodeLevel >= level ? "|" : " ");
                }
                std::cout << "\n";
            }
        }
    }
    

void displayTraversal()
{
    for (int i = currentLevel; i >= 0; i--)
    {
        Node<K, V> *ptr = head;

        std::cout << "Level " << i << ": HEAD";

        while (ptr->forward[i] != NULL)
        {
            ptr = ptr->forward[i];
            std::cout << " -> " << ptr->key;
        }

        std::cout << " -> NULL" << std::endl;
    }
}


};
} // namespace skip_list_module

#endif
