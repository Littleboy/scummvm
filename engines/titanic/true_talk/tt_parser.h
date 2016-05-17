/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef TITANIC_TT_PARSER_H
#define TITANIC_TT_PARSER_H

#include "titanic/true_talk/tt_input.h"
#include "titanic/true_talk/tt_string.h"

namespace Titanic {

enum NumberFlag { NF_1 = 1, NF_2 = 2, NF_4 = 4, NF_8 = 8, NF_10 = 0x10 };

class CScriptHandler;

struct NumberEntry {
	CString _text;
	int _value;
	int _flags;

	NumberEntry() : _value(0), _flags(0) {}
	NumberEntry(const CString &text, int value, int flags) :
		_text(text), _value(value), _flags(flags) {}
};
typedef Common::Array<NumberEntry> NumberArray;

class TTparser {
private:
	StringArray _replacements1;
	StringArray _replacements2;
	StringArray _replacements3;
	StringArray _phrases;
	NumberArray _numbers;
private:
	/**
	 * Loads the various replacement string data arrays
	 */
	void loadArrays();

	/**
	 * Normalizes a passed input, taking care of things like removing extra
	 * spaces and lowercasing everything
	 */
	int normalize(TTinput *input);
	
	/**
	 * Submethod called by normalize to handle expanding contacted word pairs
	 * like can't, should've, and so on.
	 */
	bool normalizeContraction(const TTstring &srcLine, int srcIndex, TTstring &destLine);

	/**
	 * Checks for what is likely special developer cheat codes
	 */
	static int isEmoticon(const TTstring &str, int &index);

	/**
	 * Checks if any word within a passed line has an entry in the list of replacements,
	 * and if found, replaces it with it's equivalent replacement string
	 * @param line			Line to check
	 * @param strings		List of strings to check for. Strings come in pairs, with the
	 * first being the string to match, and the second the replacement
	 */
	static void searchAndReplace(TTstring &line, const StringArray &strings);

	/**
	 * Checks the string starting at a given index for any word in the passed string array,
	 * and if found, replaces it in the line with it's replacement
	 * @param line			Line to check
	 * @param startIndex	Starting index in the start to check
	 * @param strings		List of strings to check for. Strings come in pairs, with the
	 * first being the string to match, and the second the replacement
	 * @returns				Index of the start of the following word			
	 */
	static int searchAndReplace(TTstring &line, int startIndex, const StringArray &strings);

	/**
	* Checks the string starting at a given index for a number representation
	* such as roman numericals, spelled out numbers, etc. and replaces it with
	* a plain decimal representation.
	* @param line		Line to check
	* @param startIndex	Starting index in the start to check
	* @returns			Index of the start of the following word, or -1 if at end of line
	*/
	int replaceNumbers(TTstring &line, int startIndex);

	/**
	 * Checks the string starting at a given index for a number representation
	 * such as roman numericals, spelled out numbers, etc. and replaces it with
	 * a plain decimal representation.
	 * @param line			Line to check
	 * @param startIndex	Starting index in the start to check
	 * @returns				Pointer to matching number entry, if match occurred
	 */
	const NumberEntry *replaceNumbers2(TTstring &line, int *startIndex);
public:
	CScriptHandler *_owner;
	int _field4;
	TTinput *_input;
	int _fieldC;
	int _field10;
	int _field14;
	int _field18;
public:
	TTparser(CScriptHandler *owner);

	/**
	 * Preprocesses the passed input text, to handle things like lowercasing
	 * all the words, and replcaing common slang with their full equivalents
	 */
	int preprocess(TTinput *input);
};

} // End of namespace Titanic

#endif /* TITANIC_TT_PARSER_H */
