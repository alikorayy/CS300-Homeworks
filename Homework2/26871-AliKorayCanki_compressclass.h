#pragma once
#include <string>
#include <vector>

using namespace std;



template <class HashedObj>
class HashTable {
public:
    explicit HashTable();
    int insert(const HashedObj& x);
    void insertAsci();
private:
    struct HashEntry {
        string element;
        int isActive = 0;
    };
    vector<HashEntry> array;
    int currentSize;
    string laststring;
    int findPos(HashedObj& x) const;
}; 
