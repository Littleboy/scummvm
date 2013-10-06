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

#ifndef LIATH_UTILS_H
#define LIATH_UTILS_H

#include "liath/shared.h"

namespace Liath {

class LiathEngine;

class PositionManager {
public:

	PositionManager(LiathEngine *engine);
	~PositionManager();

	// Opcodes
	OpcodeRet opcodeCheckLimits(OpcodeParameters *parameters);
	OpcodeRet opcodeAutoStart(OpcodeParameters *parameters);
	OpcodeRet opcodeAutoCheck(OpcodeParameters *parameters);
	OpcodeRet opcodeAutoStop(OpcodeParameters *parameters);
	OpcodeRet opcodePers(OpcodeParameters *parameters);
	OpcodeRet opcodeHeroPers(OpcodeParameters *parameters);
	OpcodeRet opcodeDist(OpcodeParameters *parameters);
	OpcodeRet opcodeFeel(OpcodeParameters *parameters);
	OpcodeRet opcodeBarrier(OpcodeParameters *parameters);

private:
	LiathEngine* _engine;

	struct FeelResult {
		int32 res1;
		int32 res2;
		int32 res3;

		FeelResult() {
			res1 = -1;
			res2 = -1;
			res3 = -1;
		}
	};

	struct Box {
		int32 field_0;
		int32 field_4;
		int32 field_8[18];
		int32 field_50[9];
		int32 field_74[9];
	};

	Common::Array<Box *> _boxes;

	// Helpers methods
	bool checkLimit(ObjectIndex objectIndex, uint32 *a2, uint32 *a3);
	void feeler(int32 expr1, int32 expr2, int32 expr3, FeelResult *result);
	int32 calcDir(uint32 val, int32 expr1, FeelResult *result);
	HeroIndex checkPers(OpcodeParameters *parameters);
	int32 calcDist(Work *work, int32 *expr);
	bool checkDistPers(Work *work);
	bool checkHeroPers(HeroIndex heroIndex, int32 expr);
	int isPath(int index, int value);

};

} // End of namespace Liath

#endif // LIATH_UTILS_H
