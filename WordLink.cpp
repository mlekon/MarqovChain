#include "WordLink.h"

/*--------------------------------------------------------------------------------*/
/*---------------------- Public Constructors & Destructors -----------------------*/
/*--------------------------------------------------------------------------------*/


// Initializes the link with all nulls. Never called explicitly
WordLink::WordLink()
{
	this->word = NULL;
	prefixOccurrences = 0;
	postfixOccurrences = 0;
}


// Initializes the link with a specific word. All words must use this constructor. If the
// default was called previously, it must be reinitialized with this one.
WordLink::WordLink(Word* word) 
{ 
	this->word = word; 
	prefixOccurrences = 0; 
	postfixOccurrences = 0; 
}