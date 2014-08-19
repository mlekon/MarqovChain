### Marqov Chain
#####A simple Markov Chain implementation

A Markov Chain takes a given set of characters (or word in this case) and, using the frequency
of the how often each character is followed by each other character, generates a semi-random
string that more closely matches the source data than a truly random string.

This implementation is written in C++ and uses only the standard library. It supports saving and
loading data sets, adding arbitrary text to the data set at any time, and generating output
strings either at random, or based on a seed word.

The methods are documented in MarkovChain.cpp, but for a quick start, the important methods are:

* MarkovChain() - Default constructor. Starts with an empty data set.
* MarkovChain(string) - Initializes the object the data set file given in the std::string file name
* void addText(string) - Takes a std::string, analyzes the content and appends it to the current
data set.
* string generateString(string, int) - Generates a Markov string from the current data set using
a seed string. Takes a std::string around which the Markov string will be generated and an integer
representing the maximum length of the Markov string. Returns a std::string with the text of the
Markov string.
* string generateString(int) - Generates a Markov string from the current data set. Takes a 
std::string around which the Markov string will be generated, and an integer representing the
maximum length of the Markov string in words. Returns a std::string with the text of the Markov string.
* void save(string) - Saves the current data set to a file with the name of the given std::string
* void load(string) - Takes a std::string for the name of the file to load a data set file from. This
data set file is generated from the save(string) method