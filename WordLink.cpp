/*
 * Marqov Chain: A simple Markov Chain implementation
 * Word.h: Definition of the WordLink class.
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