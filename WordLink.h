#ifndef WORD_LINK_H
#define WORD_LINK_H

#include <cstdlib>

class Word;

// A simple structure containing the number of times a particular word has occurred
// following another word.
class WordLink
{
public:
	// The linked word
	Word* word;

	// The number of times this word has followed/been followed by
	int prefixOccurrences;
	int postfixOccurrences;

	WordLink();
	WordLink(Word* word);
};

#endif