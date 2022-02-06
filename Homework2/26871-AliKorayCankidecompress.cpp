#include <iostream>
#include <fstream>
#include <string>
#include<sstream>
#include <vector>
#include "26871-AliKorayCanki_decompressclass.cpp"

using namespace std;



int main() {
	HashTable<string> hashtable;
	hashtable.insertAsci();
	ifstream reader("compout.txt");
	ofstream writer("decompout.txt");
	string line;
	int int_taker;
	while (getline(reader, line)) {
		string printcode;
		istringstream input(line);
		while (input >> int_taker) {
			printcode = hashtable.decompress(int_taker);
			writer << printcode;
		}
	}

	reader.close();
	writer.close();



	return 0;
}