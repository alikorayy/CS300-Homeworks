#include <iostream>
#include <fstream>
#include "26871-AliKorayCanki_compressclass.cpp"
#include <string>
#include <vector>
using namespace std;





int main()
{
    HashTable<string> hashtable;
    hashtable.insertAsci();
    char ch;
    int printer = 0;
    string converter;
    ifstream deneme("compin.txt");
    ofstream sonuc("compout.txt");
    while (!deneme.eof())   //eof returns true if next character
                            //is eof
    {
        deneme.get(ch);       //get command reads next character
        converter = ch;
        printer = hashtable.insert(converter);
        if (printer != -1) {
            sonuc << printer;
            sonuc << " ";
        }


    }
    deneme.close();
    sonuc.close();
    return 0;
}