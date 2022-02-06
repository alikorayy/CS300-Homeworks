#include "Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <array>

using namespace std;

GraphWord::GraphWord() { // constructor of graph
	infoword* info_node = NULL;
	infoword* curr_node = NULL;
	for(int i = 0; i < 30; i++) {
		limiter_index[i] = -1;
	}
}


bool GraphWord::is_exist(string word1,string word2) { // check if the given words are in txt

	bool is_exist1 = false;
	bool is_exist2 = false;
	for (int i = 0; i<word_vect.size(); i++) {
		if (word_vect[i] == word1) {
			is_exist1 = true;
		}
		if (word_vect[i] == word2) {
			is_exist2 = true;
		}
	
	}
	if (is_exist1 == true && is_exist2 == true) { // if they are exist return true
		return true;
	}
	else {
		return false;// else return false
	}
}
void GraphWord::construct_graph() { // a function that build the graphs

	int first_limiter, last_limiter;
	int size_vect = word_vect.size();
	for (int i = 0; i < size_vect; i++){
	info_node= new infoword();
	info_node->word = word_vect[i];
	info_node->order = i;
	graph_vect.push_back(info_node);
	}

	for (int i = 0; i < size_vect; i++){ // to reduce the size of building determine boundaries
	int word_len= word_vect[i].length();
		if (limiter_index[word_len] == -1) {
				limiter_index[word_len] = i;
		}
	}
	for (int i = 0; i < size_vect; i++) {// to reduce the size of building determine boundaries
		if (limiter_index[graph_vect[i]->word.length()] == 0)
			first_limiter = limiter_index[graph_vect[i]->word.length()];
		else if (limiter_index[graph_vect[i]->word.length()] != 0) {
			first_limiter = limiter_index[(graph_vect[i]->word.length() - 1)];
		}
		if (limiter_index[graph_vect[i]->word.length() + 2] == -1)
			last_limiter = size_vect;
		else if (limiter_index[graph_vect[i]->word.length() + 2] != -1)
			last_limiter = limiter_index[graph_vect[i]->word.length() + 2] - 1;
			
		curr_node = graph_vect[i];

		for (int k = 0; k < size_vect; k++) {
			if (i == k)
				continue;
			if (k >= first_limiter && k <= last_limiter) { // building of graph vector
				info_node = new infoword();
				info_node->word = graph_vect[k]->word;
				info_node->order = k;
				curr_node->next = info_node;
				curr_node = curr_node->next;
			}

		}

	}

}

void GraphWord::graph_algoritm(string first_word,string last_word) { // a function finds the words
	queue<infoword*> WordQueue; // creating empty queque
	string orig_first = first_word;
	string orig_last = last_word;
	infoword* temp_word = NULL;
	infoword* temp_word2 = NULL;
	int i = 0;
	bool is_completed = false;
	for(i = 0; i < graph_vect.size();i++) {
		if (graph_vect[i]->word == first_word) {
			break;
		}
	}
	temp_word = graph_vect[i];
	WordQueue.push(graph_vect[i]);
	temp_word->is_known = true;
	bool is_fit = false;
	int is_reachable = 0;
	infoword* last_element = NULL;
	while (!WordQueue.empty()) { // while queue is not empty continue to loop
		temp_word = WordQueue.front();
		WordQueue.pop();
		last_element = temp_word;
		first_word = last_element->word;
		while (last_element != NULL) { // a loop returns untill last element is found or it is NULL

			is_fit = compare_words(first_word, last_element->word);
			if (last_element->word == last_word && is_fit == true) // if the last word is found go in and finish the loop
			{
				last_element->is_known = true;
				graph_vect[last_element->order]->is_known = true;
				graph_vect[last_element->order]->before = temp_word;
				is_completed = true;
				is_reachable = 1;
				break;
			}
			last_element = last_element->next;
		}
		if (is_completed) {
			break;
		}
		temp_word2 = temp_word;
		while (temp_word != NULL) { // loop creates the path of  inner words 
			is_fit = compare_words(first_word, temp_word->word);
			if ((graph_vect[temp_word->order ]->is_known== false) && (is_fit == true)) {
				graph_vect[temp_word->order]->is_known = true;
				temp_word->is_known = true;
				WordQueue.push(graph_vect[temp_word->order]);
				graph_vect[temp_word->order]->before = temp_word2;
			}
			temp_word = temp_word->next;
		}
		if (WordQueue.empty()) { // if queue become empty print this and exit
			cout << "there is no path to " << orig_first << "-->" << orig_last << endl;
			is_reachable = 0;
			break;
		}
	}
	if (is_reachable == 1) {// reachable is 1 if the search was succesful
		output_algorithm(graph_vect[last_element->order]);

	}


}



bool GraphWord::compare_words(string word1, string word2) { // a function that compares 2 word if they have only 1 difference letter return true else false
	int word1_length = word1.length();
	int word2_length = word2.length();
	string short_word = "";
	string long_word="";
	if (word1 == word2) {
		return false;
	}
	int sum_length = word1_length - word2_length;
	if (sum_length<0) {
		sum_length = -1 * (sum_length);	
	}
	if (sum_length > 1) {
		return false;
	}
	int similar_count = 0;
	if (sum_length == 0) {
		for (int i = 0; i < word1_length; i++) {
			if (word1[i] == word2[i]) {
				similar_count++;
			}
		}
		if ((word1_length - similar_count) == 1) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (sum_length == 1) {
		int longest_index = 0;
		if (word1_length > word2_length) {
			longest_index = word1_length;
			long_word = word1;
			short_word = word2;
		}
		else {
			longest_index = word2_length;
			long_word = word2;
			short_word = word1;
		}
		int k = 0;
		for (int i = 0; i < longest_index; i++) {
			if (long_word[i] == short_word[k]) {
				k++;
				similar_count++;
			}

		}
		if (similar_count != longest_index-1) {
			return false;
		}
		else {
			return true;
		}
	}
	
}
void GraphWord::output_algorithm(infoword* last_element) { // a function prints the path of the word
	cout << "---------------------------------------------------------" << endl;
	vector<string>print_vect;
	string word1 = "";
	infoword* last_node = last_element;
	//print_vect.push_back(word1);
	while (last_node->before!=NULL) {
		word1 = last_node->word;
		print_vect.push_back(word1);
			last_node = graph_vect[last_node->before->order];
	
		
	}
	print_vect.push_back(last_node->word);
	cout << print_vect[print_vect.size() - 1] << endl;
	for (int i = print_vect.size()-2; i >= 0; i--) {
		if (print_vect[i].length() == print_vect[i + 1].length()) {
			for (int k = 0; k < print_vect[i].length(); k++) {
				if (print_vect[i + 1][k] != print_vect[i][k]) {
					cout << print_vect[i] << "	" << "(change " << print_vect[i + 1][k] << " at position " << k+1 << " to " << print_vect[i][k] << ")" << endl;
					break;
				}
				
			}
		}
		else if (print_vect[i].length() > print_vect[i + 1].length()) {
			int z = 0;
			for (int k = 0; k < print_vect[i].length();k++) {
				if (print_vect[i][k] == print_vect[i + 1][z]) {
					z++;
				}
				else {
					cout << print_vect[i] << "	(insert " << print_vect[i][k] << " after position " << k << ")" << endl;
					break;
				}
			}
		
		}
		else if (print_vect[i + 1].length() > print_vect[i].length()) {
			int z = 0;
			for (int k = 0; k < print_vect[i + 1].length(); k++) {
				if (print_vect[i + 1][k] == print_vect[i][z]) {
					z++;
				}
				else {
					cout << print_vect[i] << "	(delete " << print_vect[i + 1][k] << " at position " << k+1 << ")" << endl;
					break;
				}
			}
		
		}
	}




}

void GraphWord::GraphWordDeleter() { // empty the graph nodes to prevent memory leak
	for (int i = 0; i < graph_vect.size(); i++) {
		infoword* tempnode=NULL;
		infoword* notdeletenode= NULL;
		tempnode = graph_vect[i];
		while (tempnode->next != NULL) {
			notdeletenode = tempnode->next;
			delete tempnode;
			tempnode = notdeletenode;
		
		}
		tempnode = NULL;

	}

}