#ifndef WORD_H
#define WORD_H

#include "WordLink.h"
#include <fstream>
#include <map>

class MarkovChain;

using namespace std;

// This class represents a single textual word in the corpus of a Markov Chain.
// It contains all probability data concerning the words that follow it
class Word
{
private:
	// Contains the id of the next word to be created. Incremented with each new word
	static int nextId;

	// The text of the word
	string text;

	// A collection of all words that have been seen to follow this
	map<int, WordLink> links;

	// The number of times this word has occurred in the corpus. This is almost
	// implicitly, the total count of all postfix occurrences
	int occurrences;

	// Link to the chain this word is in
	MarkovChain* chain;

	// Unique id for this word to allow indexing without string-based maps
	int id;
public:
	Word(string, int, MarkovChain*);
	Word(string, MarkovChain*);
	Word(char, int, MarkovChain*);
	Word(char, MarkovChain*);
	~Word();

	void addOccurrence();
	int getOccurrences();
	string getText();
	int getId();

	void addPostfix(Word*);
	void addPrefix(Word*);
	Word* getRandomPostfix();
	Word* getRandomPrefix();
	Word* getRandom(int);

	void serialize(ofstream&);
	void unserialize(ifstream&);
};

#endif