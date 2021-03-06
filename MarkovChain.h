/*
 * Marqov Chain: A simple Markov Chain implementation
 * Word.h: Declaration of the MarkovChain class. Represents all data set operations.
 * Copyright (C) 2014  Mike Lekon
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MARKOV_CHAIN_H
#define MARKOV_CHAIN_H

#define GENERATE_PREFIX 1
#define GENERATE_POSTFIX 2
#define GENERATE_BOTH 3

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>

class Word;

using namespace std;

// This class generates a Markov chain of textual words. The chain is created by
// the sequence of words for each sentence in the given corpus. It is designed
// to generate semi-random text strings using the probabilities of the following words.
class MarkovChain
{
private:
	// Text identifiers for the start and end words
	const static char startText[2];
	const static char endText[2];

	// Central repository for Words in the corpus, keyed by the text of the Word
	map<string, Word*> dictionary;

	// A dummy word that is used to indicate the start of a sentence. This is added
	// to each sentence before the first word is added.
	Word* start;

	// A dummy word indicating the end of a sentence. This is always added to the
	// end of a sentence sequence.
	Word* end;

	// The number of real words to compare as a single token
	int order;

	void initTerminators(int, int);
	void initTerminators();

	void serialize(ofstream&);
	void unserialize(ifstream&);

	// Utility methods
	void cleanTokens(vector<string>&);
	vector<string> tokenize(string, int);
	bool isWhitespace(char);
	bool isWhitespace(string);
	bool isLetter(char);

public:
	MarkovChain();
	MarkovChain(string);
	~MarkovChain();

	// Saving and loading methods
	void load(string);
	void save(string);
	void clear();

	void addText(string);
	void setOrder(int);
	string generateString(int, Word*, int);
	string generateString(string, int);
	string generateString(int);

	Word* getWord(string);
};

#endif