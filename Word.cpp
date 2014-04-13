#include "Word.h"
#include "MarkovChain.h"

/*---------------------------------------------------------------------*/
/*---------------------- Private Static Members -----------------------*/
/*---------------------------------------------------------------------*/


int Word::nextId = 1;


/*--------------------------------------------------------------------------------*/
/*---------------------- Public Constructors & Destructors -----------------------*/
/*--------------------------------------------------------------------------------*/


// Initializes a word the text and a pre-selected id. This is used to unserializing
// a file, where each word has already been assigned an id.
Word::Word(string text, int id, MarkovChain* chain)
{
	occurrences = 0;
	this->text = text;
	this->chain = chain;
	this->id = id;

	if(Word::nextId < id)
		Word::nextId = id + 1;
}


// Initializes a word with its text. Called when creating words with the MarkovChain::addText
Word::Word(string text, MarkovChain* chain)
{
	occurrences = 0;
	this->text = text;
	this->chain = chain;
	this->id = Word::nextId++;
}


// Inititalizes a word with a 1-character text field. Used to initialize start and end
Word::Word(char text, int id, MarkovChain* chain)
{
	occurrences = 0;
	this->text = text;
	this->chain = chain;
	this->id = id;
}


// Inititalizes a word with a 1-character text field. Used to initialize start and end
Word::Word(char text, MarkovChain* chain)
{
	occurrences = 0;
	this->text = text;
	this->chain = chain;
	this->id = Word::nextId++;
}


// No dynamically allocated memory. Words should only be deleted from within the MarkovChain.
Word::~Word()
{
}


/*--------------------------------------------------------------------*/
/*---------------------- Public Methods ------------------------------*/
/*--------------------------------------------------------------------*/


// Increments the occurrences value
void Word::addOccurrence()
{
	occurrences++;
}


// Returns the number of times this word has occurred
int Word::getOccurrences()
{
	return occurrences;
}


// Returns the text of the Word
string Word::getText()
{
	return text;
}


// Returns the id of this word
int Word::getId()
{
	return id;
}


// Add a word found to come after this one. If the word already exists in the list
// increment the occurrence counter
void Word::addPostfix(Word* word)
{
	int postfixId = word->getId();

	// Add the word if it doesn't already exist in the list
	if(links.find(postfixId) == links.end())
		 links[postfixId] = WordLink(word);

	// Increment the occurrence counter, increasing the probability of this sequence
	links[postfixId].postfixOccurrences++;
}


// Add a word found to come after this one. If the word already exists in the list
// increment the occurrence counter
void Word::addPrefix(Word* word)
{
	int prefixId = word->getId();

	// Add the word if it doesn't already exist in the list
	if(links.find(prefixId) == links.end())
		 links[prefixId] = WordLink(word);

	// Increment the occurrence counter, increasing the probability of this sequence
	links[prefixId].prefixOccurrences++;
}


// Randomly chooses a Word found to follow this, weighted by frequency of occurrence
Word* Word::getRandomPostfix()
{
	// If there are no links, this is probably the end word. End the sequence
	// by returning NULL
	if(links.size() == 0)
		return NULL;

	// Generate a random value between 0 and the number of occurrances of all postfixes,
	// which is also the number of occurrences of this word
	int r = rand() % occurrences;

	// For each link...
	auto i = links.begin();
	for(i; i != links.end(); i++)
	{
		// Subtract the number of occurrences of the word link from the random value
		// When r is 0 or less, stop and return that Word of the WordLink being examined.
		if(i->second.postfixOccurrences > 0)
		{
			// If this word would put the accumulator to 0, that is the word to select
			if(r <= i->second.postfixOccurrences)
				return i->second.word;
			r -= i->second.postfixOccurrences;
		}
	}

	// Should never occur, since an empty links map would return at the beginning,
	// and r can't be greater than the total number of WordLink occurrences.
	return NULL;
}


// Randomly chooses a Word found to follow this, weighted by frequency of occurrence
Word* Word::getRandomPrefix()
{
	// If there are no links, this is probably the end word. End the sequence
	// by returning NULL
	if(links.size() == 0)
		return NULL;

	// Generate a random value between 0 and half the number of occurrances of all postfixes.
	// which is also the number of occurrences of this word
	int r = rand() % occurrences;

	// For each link...
	auto i = links.begin();
	for(i; i != links.end(); i++)
	{
		// Subtract the number of occurrences of the word link from the random value
		// When r is 0 or less, stop and return that Word of the WordLink being examined.
		if(i->second.prefixOccurrences > 0)
		{
			WordLink l = i->second;
			if(r <= l.prefixOccurrences)
				return l.word;
			r -= l.prefixOccurrences;
		}
	}

	// Should never occur, since an empty links map would return at the beginning,
	// and r can't be greater than the total number of WordLink occurrences.
	return NULL;
}


// Randomly chooses a Word found to follow this, weighted by frequency of occurrence
Word* Word::getRandom(int direction)
{
	// If there are no links, this is probably the end word. End the sequence
	// by returning NULL
	if(links.size() == 0)
		return NULL;

	// Generate a random value between 0 and the number of occurrances of all postfixes,
	// which is also the number of occurrences of this word
	int r = rand() % occurrences;

	// For each link...
	auto i = links.begin();
	for(i; i != links.end(); i++)
	{
		int occurrences = (direction == GENERATE_PREFIX) ? i->second.prefixOccurrences : i->second.postfixOccurrences;

		// Subtract the number of occurrences of the word link from the random value
		// When r is 0 or less, stop and return that Word of the WordLink being examined.
		if(occurrences > 0)
		{
			if(r <= 0)
				return i->second.word;
			r -= occurrences;
		}
	}

	// Should never occur, since an empty links map would return at the beginning,
	// and r can't be greater than the total number of WordLink occurrences.
	return NULL;
}


// Writes a textual representation of this word and all of its links to given file.
// First line is the word next, followed by groups of 3 lines for each link. First of
// each group is the id of the word being linked to, second is the number of times it
// appears as a postfix, third is the number of times it appears as a prefix. Once all
// links are written, an extra newline is added to delimit this word
void Word::serialize(ofstream& file)
{
	char buf[256];

	// Write this word's text
	file.write(text.c_str(), text.length());
	file.write("\n", 1);

	// Write the occurrences of this word
	itoa(occurrences, buf, 10);
	file.write(buf, strlen(buf));
	file.write("\n", 1);

	auto i = links.begin();
	for(; i != links.end(); i++)
	{
		int linkId = (*i).first;
		WordLink link = (*i).second;
		string wordText = link.word->getText().c_str();

		// An empty (and therefore invalid) word could cause problems
		if(!wordText.length())
			continue;

		// Write the linked word's text
		file.write(wordText.c_str(), wordText.size());
		file.write("\n", 1);

		// Write postfix occurences
		itoa(link.postfixOccurrences, buf, 10);
		file.write(buf, strlen(buf));
		file.write("\n", 1);

		// Write prefix occrrences
		itoa(link.prefixOccurrences, buf, 10);
		file.write(buf, strlen(buf));
		file.write("\n", 1);
	}
	file.write("\n", 1);
}


// Initializes a word's links from the given file. The file stream begins just before the
// first line of the first link for this word.
void Word::unserialize(ifstream& file)
{
	const int bufSize = 256;
	char buf[bufSize];

	// Read the occurrences of this word
	file.getline(buf, bufSize);
	occurrences = atoi(buf);

	do
	{
		// Try to read the link text
		file.getline(buf, bufSize);

		// Make sure nothing bad happened with the file before continuing
		if(!strlen(buf) || file.eof() || file.bad())
			break;

		string wordText = buf;
		WordLink link(chain->getWord(wordText));

		// Read the postfix occurrences
		file.getline(buf, bufSize);
		link.postfixOccurrences = atoi(buf);

		// Read the prefix occurrences
		file.getline(buf, bufSize);
		link.prefixOccurrences = atoi(buf);

		// Add the link to the map
		links[link.word->getId()] = link;
	}
	while(!file.eof());
}