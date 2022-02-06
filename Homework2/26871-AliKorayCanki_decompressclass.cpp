#include "26871-AliKorayCanki_decompressclass.h"
#include <string>
#include <vector>

using namespace std;

template <class HashedObj>
HashTable<HashedObj>::HashTable() //  default constructor of hash table

{
    array.resize(4096);
    counter1 = 0;
    last_string = "";
}

template <class HashedObj>
void HashTable<HashedObj>::insertAsci() { // a function inserts all the ascii values to the vector
    char a;
    for (int i = 0; i < 256; i++) {
        a = i;
        array[i].element = a;
        array[i].isActive = 1;
    }
}



template <class HashedObj>
void HashTable<HashedObj>::insert_to_last(const HashedObj& x) { // a function that takes the object and inserts the last element of  array 

   
    int position1 = 0;
        while (array[position1].isActive == 1 && array[position1].element != x) {

            position1 += 1;  // add the difference

            if (position1 >= array.size()) {           // perform the mod
                position1 -= array.size();              // if necessary
            }
        }
        array[position1].element = x;
        array[position1].isActive = 1;

}



template <class HashedObj>
HashedObj HashTable<HashedObj>::decompress(int x) { // a function that returns the object value to be printed to the decompout

    if (counter1 == 0) { // this means you are taking the first value from the compout
        HashedObj word_string = array[x].element;
        counter1++;
        last_string = word_string;
        return word_string;
    }
    else {
        bool isexist;
        if (array[x].isActive == 1) { // checks the object whether inserted or not
            isexist = true;
        }
        else if (array[x].isActive == 0) {
            isexist = false;
        }
        if (isexist) { // determine the last string , insert the combination of last string + current string[0] to the dictionary and return the object
            HashedObj next_last_string = array[x].element;
            HashedObj stringtobeadded = last_string;
            stringtobeadded = last_string + next_last_string[0];
            insert_to_last(stringtobeadded);
            counter1++;
            last_string = next_last_string;
            return last_string;



        }
        else if (isexist == false) { // change the last string insert the combination of object + object[0] to the dictionary, return the index of object
            HashedObj tempstring = last_string;
            tempstring = tempstring + tempstring[0];
            insert_to_last(tempstring);
            counter1++;
            last_string = tempstring;
            return last_string;

        }




    }



}