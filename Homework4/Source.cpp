#include <iostream>
#include <fstream>
#include<sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include "Graph.h"
using namespace std;

bool sort_func(string& s1, string& s2) {
	return (s1.size() < s2.size());
}

int main() {

	ifstream input_reader;
	string from_string, to_string, txt_name, line, word;
	vector<string> transvect;
	txt_name = "words.txt";
	input_reader.open(txt_name.c_str());
	GraphWord* word_graph = NULL;
	while (getline(input_reader, line)) {
		istringstream input(line);
		input >> word;
		transvect.push_back(word);
	}
	sort(transvect.begin(), transvect.end(), sort_func);

	while (1) {
		delete word_graph;
		word_graph = new GraphWord();
		word_graph->word_vect = transvect;
		cout << "The Path is building..." << endl;
		word_graph->construct_graph();
		cout << "Please enter two string" << endl;
		cin >> from_string;
		cin >> to_string;
		cout << endl;
		if (from_string[0] == '*') {
			cout<<"The program is terminating" << endl;
			break;
		}
		bool exist = word_graph->is_exist(from_string,to_string);
		if (exist == false) {
			cout << "One of the words is not in the words.txt, please try it again" << endl;
			word_graph->GraphWordDeleter();
			continue;
		}
		word_graph->graph_algoritm(from_string, to_string);
		word_graph->GraphWordDeleter();
	}






	return 0;
}