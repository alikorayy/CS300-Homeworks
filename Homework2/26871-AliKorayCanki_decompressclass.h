#pragma once
#include <string>
#include <vector>

using namespace std;

// header file of HashTable function

template <class HashedObj>
class HashTable {

public:
    explicit HashTable();
    HashedObj decompress(int x);
    void insertAsci();
    void  insert_to_last(const HashedObj& x);
private:

    // struct that has object and integer
    struct HashEntry {
        string element;
        int isActive = 0;
    };
    vector<HashEntry> array;
    int counter1;
    HashedObj last_string;




}; 
