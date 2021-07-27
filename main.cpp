#include <deque>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

struct wordFreq{ //Used for keeping track of the frequency of each word
	std::string word = "";
	int frequency = 0;
};

int find_wordDeque_index(std::deque<struct wordFreq> wordDeque, std::string word); //Used for finding where a word has already been added to the deque. If a word is not in there, it returns a -1, indicating that it is not there.
std::deque<std::string> find_references_for_word(std::string word, std::string sourceFile); //Reads through the entire source file and returns the references where the word is found (includes the same reference multiple times if appropriate)
std::deque<std::string> parse_line(std::string line); //Splits a string into individual words which separated by spaces
std::string sanitize_string(std::string line); //Removes all punctuation from a string and makes it entirely lowercase

int main(int argc, char *argv[]){
	if(argc != 4){
		printf("Too many or too few arguments, exiting.\n"); return 1;
	}
	std::string sourceFile = argv[1];
	int min = std::stoi(argv[2]); int max = std::stoi(argv[3]);
	std::ifstream readFile; readFile.open(sourceFile); std::string line;
	
	std::deque<std::string> parsedLine;
	std::deque<struct wordFreq> wordDeque; int wordDequeIndex;
	
	while(getline(readFile, line)){
		parsedLine = parse_line(sanitize_string(line.substr(line.find("|"), line.size() - 1)));
		for(std::string &i : parsedLine){
			wordDequeIndex = find_wordDeque_index(wordDeque, i);
			if(wordDequeIndex == -1){
				wordDeque.push_back({i, 1});
			}else{
				wordDeque[wordDequeIndex].frequency++;
			}
		}
	}
	
	std::deque<std::string> refs;
	for(struct wordFreq &i : wordDeque){
		if(i.frequency >= min && i.frequency <= max){
			printf("%s|", i.word.c_str());
			refs = find_references_for_word(i.word, sourceFile);
			for(int j = 0; j < refs.size() - 1; j++){
				printf("%s, ", refs[j].c_str());
			}
			printf("%s\n", refs[refs.size() - 1].c_str());
		}
	}

	return 0;
}


int find_wordDeque_index(std::deque<struct wordFreq> wordDeque, std::string word){
	int index = 0;
	for(struct wordFreq &i : wordDeque){
		if(i.word == word){
			return index;
		}
		index++;
	}
	return -1;
}

std::deque<std::string> find_references_for_word(std::string word, std::string sourceFile){
	std::deque<std::string> ret; std::deque<std::string> parsedLine;
	std::ifstream openFile; openFile.open(sourceFile); std::string line;
	while(getline(openFile, line)){
		parsedLine = parse_line(sanitize_string(line.substr(line.find("|") + 1, line.size() - 1)));
		for(std::string &i : parsedLine){
			if(i == word){
				ret.push_back(line.substr(0, line.find("|")));
			}
		}
	}
	return ret;
}

std::deque<std::string> parse_line(std::string line){
	std::deque<std::string> ret;
	int wordStart;
	while(1){
		wordStart = line.find(" ");
		if(wordStart > 0){
			ret.push_back(line.substr(0, wordStart));
			line.erase(0, wordStart + 1);
		}else{
			ret.push_back(line);
			return ret;
		}
	}
	return ret;
}

std::string sanitize_string(std::string line){
	std::string ret = "";
	for(char &i : line){
		switch(i){
			case 'A' ... 'Z':
				ret += i + 32;
				break;
			case ' ':
			case '\'':
			case 'a' ... 'z':
				ret += i;
				break;
		}
	}
	return ret;
}
