/*
 * Marqov Chain: A simple Markov Chain implementation
 * Word.h: Definition of the MarkovChain class.
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

#include "MarkovChain.h"
#include "Word.h"
#include <list>

/*---------------------------------------------------------------------*/
/*---------------------- Private Static Members -----------------------*/
/*---------------------------------------------------------------------*/

const char MarkovChain::startText[2] = {(char)0x11, '\0'};
const char MarkovChain::endText[2] = {(char)0x12, '\0'};

/*--------------------------------------------------------------*/
/*---------------------- Private Methods -----------------------*/
/*--------------------------------------------------------------*/


// Reinitializes the start and end words with specific ids
void MarkovChain::initTerminators(int startId, int endId)
{
	start = new Word(startText, startId, this);
	end = new Word(endText, endId, this);

	dictionary[startText] = start;
	dictionary[endText] = end;
}


// Reinitializes the stand and end words with auto-incrementing ids
void MarkovChain::initTerminators()
{
	start = new Word(startText, this);
	end = new Word(endText, this);

	dictionary[startText] = start;
	dictionary[endText] = end;
}


// Writes the word and chain data to a file that can be used to reconstruct this same chain.
void MarkovChain::serialize(ofstream& outFile)
{
	const int bufSize = 256;
	char buf[bufSize];

	auto i = dictionary.begin();

	// Write the header. For each word in the dictionary write the text and its id
	for(; i != dictionary.end(); i++)
	{
		string text = (*i).first;
		WordLink link = (*i).second;

		// Write the word text
		outFile.write(text.c_str(), text.length());
		outFile.write("\n", 1);

		// Write the id
		itoa(link.word->getId(), buf, 10);
		outFile.write(buf, strlen(buf));
		outFile.write("\n", 1);
	}

	outFile.write("\n", 1);

	// Write the links associated with each word. The word text is given on one line, followed
	// by each link, which consists of the word text, prefix occurrences and postfix occurrences
	i = dictionary.begin();
	for(; i != dictionary.end(); i++)
	{
		// Let the word serialize its own links
		(*i).second->serialize(outFile);
	}
}


void MarkovChain::unserialize(ifstream& inFile)
{
	const int bufSize = 256;
	char buf[bufSize];

	// Load the basic word info header. Contains the text of the word and its id
	while(!inFile.eof() && !inFile.bad() && inFile.is_open())
	{
		inFile.getline(buf, bufSize);
		if(!strlen(buf))
			break;

		string text = buf;

		inFile.getline(buf, bufSize);
		int id = atoi(buf);

		Word* word = new Word(text, id, this);
		dictionary[text] = word;
	}

	// Pull the new terminators from the dictionary
	start = dictionary[startText];
	end = dictionary[endText];

	// Parse each word's links now that all words have been initialized
	while(!inFile.eof() && !inFile.bad())
	{
		// First line is the word text. This must be done outside the Word's serialize function
		// go that the appropriate word can be found on which to call serialize
		inFile.getline(buf, bufSize);
		string text = buf;
		if(!text.length())
			continue;

		Word* word = dictionary[text];
		word->unserialize(inFile);
	}
	
	return;
}


// Utility function to strip off and non-letter characters from the words of a
// sentence. Non-letter characters are anything other than A-Z or a-z. Only characters
// on either end of the string are stripped. Those inside, surrounded by letters
// are preserved. The words are trimmed in situ
void MarkovChain::cleanTokens(vector<string>& tokens)
{
	//  For each word in the given sentence
	for(unsigned int i = 0; i < tokens.size(); i++)
	{
		int tokensize = tokens[i].size();
		int charCount = 0;

		// Count the non-letter characters on the left side of the string
		for(int j = 0; j < tokensize; j++)
		{
			// Break if a letter is encountered, so that interior non-letters
			// are preserved
			if(!isLetter(tokens[i][j]))
				charCount++;
			else
				break;
		}

		// Remove the characters found
		tokens[i].erase(0, charCount);

		// Reset the size since the string may have changed from
		tokensize = tokens[i].size();
		charCount = 0;

		// Count the non-letter characters on the right side of the string		
		for(int j = tokensize - 1; j >= 0; j--)
		{
			// Break if a letter is encountered, so that interior non-letters
			// are preserved
			if(!isLetter(tokens[i][j]))
				charCount++;
			else
				break;
		}

		// Remove the characters found
		tokens[i].erase(tokensize - charCount, charCount);
	}
}


// Breaks up the given sentence into whitespace-delimited words.
// Returns a vector containing each word that was found.
vector<string> MarkovChain::tokenize(string text, int order)
{
	vector<string> words;

	// Index where the previous whitespace was found
	int lastSpace = 0;

	// Index of the current character being examined
	unsigned int i = 0;

	// The number of real words that have been included in the current token
	int orderCount = 0;

	// In the case of multiple sequential whitespace characters, this is set when
	// encounting the first whitespace character, and unset on the first non-whitespace
	// character, ensuring the orderCount is only incremented once.
	bool orderFlag = false;

	// For each character in the sentence
	for(; i < text.length(); i++)
	{
		// Continue until a whitespace character is found, meaning a word has
		// been traversed
		if(isWhitespace(text[i]))
		{
			// Skip tokenizing on this whitespace if the number of words in the
			// token has not reached the order yet
			if(orderFlag == false && orderCount < order - 1)
			{
				orderFlag = true;
				orderCount++;
				continue;
			}

			string s;

			// A word must be at least 1 character long, so there must be a minimum
			// difference of 2 between lastSpace and i. However, on the first iteration,
			// lastSpace points to index 0, which is likely a letter character.
			// Use lastSpace inclusively in this case
			if(i - lastSpace > 1 && lastSpace > 0)
				s = text.substr(lastSpace + 1, i - 1 - lastSpace);
			else if(i - lastSpace == 1 || lastSpace == 0)
				s = text.substr(lastSpace, i - lastSpace);

			// An additional, and probably unnecessary check for empty words
			if(!isWhitespace(s))
				words.push_back(s);

			// Begin traversing the next word
			lastSpace = i;
		}
		else if(orderFlag == true)
		{
			// Reset the flag when encountering a non-whitespace character, meaning
			// future whitespace will be the boundary of a new real word
			orderFlag = false;
		}
	}

	// Since most sentences don't end with a space, add the final word separately
	if(i - lastSpace >= 1)
	{
		string lastword = text.substr(lastSpace + (lastSpace > 0 ? 1 : 0), i - (lastSpace > 0 ? 1 : 0) - lastSpace);
		words.push_back(lastword);
	}

	// Remove non-letter characters
	cleanTokens(words);

	return words;
}


// Utility function. Checks the given character for one of the "C" whitespace characters
bool MarkovChain::isWhitespace(char c)
{
	return c == '\n' || c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f';
}


// Utility function. Check whether the given string is composed of only whitespace.
bool MarkovChain::isWhitespace(string s)
{
	for(unsigned int i = 0; i < s.length(); i++)
	{
		if(!isWhitespace(s[i]))
			return false;
	}

	return true;
}


// Utility function. Check whether the given character is a letter. Letters consist
// of A-Z and a-z
bool MarkovChain::isLetter(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/*--------------------------------------------------------------------------------*/
/*---------------------- Public Constructors & Destructors -----------------------*/
/*--------------------------------------------------------------------------------*/


// Initializes up an empty MarkovChain
MarkovChain::MarkovChain()
{
	// Initialize the start and end to empty strings so that they will not interfere
	// with any valid word that could be added to the dictionary
	initTerminators();
}


// Initializes a MarkovChain using the given serialized chain file
MarkovChain::MarkovChain(string fileName)
{
	initTerminators();
	load(fileName);
}


// Deletes dynamically allocated memory and saves all training sentences to a file
// allowing it to be read back in during the next startup to recreate the chain.
MarkovChain::~MarkovChain()
{
	clear();
}


/*--------------------------------------------------------------------*/
/*---------------------- Public Methods ------------------------------*/
/*--------------------------------------------------------------------*/


// Load the MarkovChain stored in the given file into this by calling the unserialize method
void MarkovChain::load(string fileName)
{
	ifstream chainFile(fileName.c_str(), ios::in);
	if(chainFile.is_open())
	{
		clear();
		unserialize(chainFile);
		chainFile.close();
	}
}


// Saves the MarkovChain into the given file by calling the serialize method
void MarkovChain::save(string fileName)
{
	ofstream saveFile(fileName.c_str(), ios::out | ios::trunc);
	if(saveFile.is_open())
	{
		serialize(saveFile);
		saveFile.close();
	}
}


// Removes all words from the dictionary and reinitializes start and end.
void MarkovChain::clear()
{
	// Clean up by deleting all of the Words. Each Word deletes its own set of links
	auto i = dictionary.begin();
	for(i; i != dictionary.end(); i++)
		delete i->second;

	initTerminators();
}


// Add text to the chain's corpus. The text should have space delimited sentences
void MarkovChain::addText(string text)
{
	// Find the index of end of the first sentence
	int period = text.find('.');

	// The index of end of the previous sentence (start of the current)
	int lastPeriod = 0;

	if(period == -1)
		period = text.length() - 1;

	// For each sentence in the text
	while(period != string::npos && period >= 0)
	{
		if(text[lastPeriod] == '.')
			lastPeriod++;

		// Pull out the text between the two period index
		string line = text.substr(lastPeriod, period - lastPeriod + 1);

		// Find the next sentence boundaries for the next iteration
		lastPeriod = period;
		period = text.find('.', period + 1);

		// Tokenize the sentence, splitting it into words
		vector<string> words = tokenize(line, order);

		// If there are no words in the sentence (elipses, for example), immediately
		// skip to the next sentence
		if(words.size() == 0)
			continue;

		// The current word in the sentence's sequence being analyzed. Initially
		// set to the start word so that links to words commonly starting sentences
		// can be found.
		Word* word = start;

		// In order to generate random words quickly, the number of times the word
		// occurrs is needed. Since start never actually occurs, it must be set
		// to occurr artificially.
		word->addOccurrence();

		// For each word in the sentence
		for(unsigned int i = 0; i < words.size(); i++)
		{
			// Another, probably unnecessary, check for empty words
			if(words[i].length() == 0)
				continue;

			// Pull the Word object for the next textual word from the dictionary
			Word* nextWord;

			// If the word is not in the dictionary, add it
			if(dictionary.find(words[i]) == dictionary.end())
				dictionary[words[i]] = new Word(words[i], this);

			nextWord = dictionary[words[i]];

			// Examining this word implies it has occurred again in the corpus.
			// Increment its count of occurrences
			nextWord->addOccurrence();

			// The nextWord value comes after word in the sentence sequence, therefore
			// nextWord is a postfix of word. Add it as such, so nextWord becomes
			// a possiblity to follow word when generating the final string.
			word->addPostfix(nextWord);
			nextWord->addPrefix(word);

			// The following word of the sequence comes after nextWord, so nextWord
			// assumes the role of its predecessor
			word = nextWord;
		}

		// Add the end to the final word in the sequence to mark it as a possible
		// ending point for sentences (because by definition nothing follows end).
		if(word != start)
		{
			word->addPostfix(end);
			end->addPrefix(word);
		}
	}
}


// Sets the number of words to group together when forming the dictionary
void MarkovChain::setOrder(int order)
{
	this->order = order;
}


// Generates a semi-random string using the chain data structure generated from
// the given text corpus. No more than maxWordCount words will be included in the
// returned string, but fewer words is possible, should the end word be chosen
string MarkovChain::generateString(int direction, Word* seed, int maxWordCount)
{
	// A list of word to be concatenated to form the final string
	list<Word*> randomWords;

	// References to words generated in prefix (ws) and postfix (we) directions
	Word* ws = seed;
	Word* we = seed;

	// The string that will be returned. It will contain the values in randomWords
	// separated by spaces.
	string finalString;

	// Flags representing the directions to generate words. These are set to false when an endpoint
	// word in reached in either direction. Generation is finished when both are true
	bool startReached = (direction & GENERATE_PREFIX) == 0 || ws == start;
	bool endReached = (direction & GENERATE_POSTFIX) == 0 || we == end;

	// Start with the seed. The string will grow in both directions from here
	randomWords.push_back(seed);

	// Continue until the maximum number of words is reached, or an ending point
	// is encountered
	for(int i = 0; i < maxWordCount;)
	{
		// If both ends of the string have been reached, we're done collecting random words
		if(startReached && endReached)
			break;

		// If the start word hasn't been encountered, choose a random prefix and add it to the
		// front of randomWords
		if(!startReached)
		{
			ws = ws->getRandomPrefix();
			randomWords.push_front(ws);
			i++;

			// Once start is reached, don't generate anymore prefixes
			if(ws == start)
				startReached = true;
		}

		// If the end word hasn't been encountered, choose a random postfix and add it to the
		// end of randomWords
		if(!endReached)
		{
			we = we->getRandomPostfix();
			randomWords.push_back(we);
			i++;

			// Once end is reached, don't generate anymore postfixes
			if(we == end)
				endReached = true;
		}
	}

	// If only generating in one direction, either start or end will be missing. Add them explicitly
	// so they're guaranteed to be there. Multiples do nothing to the final result.
	randomWords.push_front(start);
	randomWords.push_back(end);

	// For each word in the list of words to use, add the text and a space to
	// the final string
	auto i = randomWords.begin();
	for(i; i != randomWords.end(); i++)
	{
		if(*i != end && *i != start)
		{
			finalString.append((*i)->getText());
			finalString.append(" ");
		}
	}

	return finalString;
}


// Generates a semi-random sentence using a pre-made sentence as a seed. A random
// word is chosen from the seed and, if it's in the dictionary, used to generate
// a sentence around it.
string MarkovChain::generateString(string seed, int maxWordCount)
{
	// Split the seed into separate words
	vector<string> seedWords = tokenize(seed, order);

	// Select a random word from the seed and take it from the dictionary if it exists
	Word* seedWord = NULL;

	// Try 5 times to find a seed word in the dictionary. Stop if one is found
	if(seedWords.size() > 0)
	{
		for(int i = 0; i < 5; i++)
		{
			int randomIndex = rand() % seedWords.size();
			string randomSeed = seedWords[randomIndex];

			// If the word exists, use it as the seed
			if(dictionary.find(randomSeed) != dictionary.end())
			{
				seedWord = dictionary[randomSeed];
				break;
			}
		}
	}

	// If it's not in the dictionary, just use the start word
	if(seedWord == NULL)
		seedWord = start;

	return generateString(GENERATE_BOTH, seedWord, maxWordCount);
}


// Generates a semi-random sentence beginning with an empty sentence start string
string MarkovChain::generateString(int maxWordCount)
{
	return generateString(GENERATE_POSTFIX, start, maxWordCount);
}


// Returns the Word object that has the given text
Word* MarkovChain::getWord(string text)
{
	return dictionary[text];
}
