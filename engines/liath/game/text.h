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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef LIATH_TEXT_H
#define LIATH_TEXT_H

#include "liath/shared.h"

namespace Liath {

class LiathEngine;

class TextManager {
public:

	TextManager(LiathEngine *engine);
	~TextManager();

	OpcodeRet opcodeOut(OpcodeParameters *parameters);
	OpcodeRet opcodeQuit(OpcodeParameters *parameters);
	OpcodeRet opcodeExitText(OpcodeParameters *parameters);
	OpcodeRet opcodeCheck(OpcodeParameters *parameters);
	OpcodeRet opcodeChange(OpcodeParameters *parameters);

private:
	LiathEngine* _engine;

	char _arrayTextNumber[500];

	// Opcode helpers
	OpcodeRet exitText(int32 val);


	// Helper functions
	int32 getFirstEmptyText();
	void freeNumberText(int32 index);
};

} // End of namespace Liath

#endif // LIATH_TEXT_H
