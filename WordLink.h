/*
 * Marqov Chain: A simple Markov Chain implementation
 * Word.h: Declaration of the WordLink class. Links two Word objects, with their frequencies.
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