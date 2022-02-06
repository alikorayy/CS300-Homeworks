#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <array>

using namespace std;

struct infoword
{
	int value=0;
	string word = "";
	int order = -1;
	bool is_known = false;
	infoword* next=NULL;
	infoword* before=NULL;
	string prev_word = "";
};

class GraphWord {

public:
	GraphWord();
	vector<string> word_vect;
	void construct_graph();
	bool is_exist(string w1,string w2);
	void graph_algoritm(string w1,string w2);
	bool compare_words(string w1, string w2);
	void output_algorithm(infoword* l);
	void GraphWordDeleter();


private:
	vector<infoword*> graph_vect;
	infoword* info_node = NULL;
	int limiter_index[30];
	infoword* curr_node;








};










#pragma once
