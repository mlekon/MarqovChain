/*
 * Marqov Chain: A simple Markov Chain implementation
 * Word.h: Declaration of the Word class, which represents one datum in the active data set
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