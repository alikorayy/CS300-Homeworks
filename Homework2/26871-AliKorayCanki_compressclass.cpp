#include "26871-AliKorayCanki_compressclass.h"
#include <string>
#include <vector>

using namespace std;

template <class HashedObj>
HashTable<HashedObj>::HashTable() // constructor of hashtable
{
    array.resize(4096);
    laststring = "";
}


template <class HashedObj>
int HashTable<HashedObj>::findPos(HashedObj& x) const // a function that finds the last position of object and returns the position
{


   // int p_index = hashFunction(x); // current position
    int p_index = 0;
    while (array[p_index].isActive == 1 && array[p_index].element != x) {

        p_index += 1;  // add the difference

        if (p_index >= array.size()) {           // perform the mod
            p_index -= array.size();              // if necessary
        }
    }
    
    return p_index;

}



template <class HashedObj>
int HashTable<HashedObj>::insert(const HashedObj& x) // a function returns the index of the object that will be written into compout file
{

    HashedObj tempstring = laststring + x;
    bool exist;
    int currentPos = findPos(tempstring); // find the position of obj
    if (array[currentPos].element == tempstring) { // if the object at the given index is equal the lastsring+x return true 
        exist = true;
    }
    else if (array[currentPos].element != tempstring) {
        exist = false;
    }
    if (exist) { // if exist just change the last string 
        laststring = tempstring;
        return -1;
    }
    if (exist == false) { // if not exist add it to the vector, change laststring and return the position of  old laststring
        int y = findPos(laststring);
        array[currentPos].element = tempstring;
        array[currentPos].isActive = 1;
        laststring = x;
        return y;
    }

}
template <class HashedObj>
void HashTable<HashedObj>::insertAsci() { //  firstly insert all the ascii values to the vector
    char a;
    for (int i = 0; i < 256; i++) {
        a = i;
        array[i].element = a;
        array[i].isActive = 1;
    }



}
