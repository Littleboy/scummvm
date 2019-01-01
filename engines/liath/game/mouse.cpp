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

#include "liath/game/mouse.h"

#include "liath/game/game.h"
#include "liath/game/graphics.h"
#include "liath/game/hero.h"
#include "liath/game/work.h"

#include "liath/helpers.h"
#include "liath/liath.h"

#include "common/system.h"

namespace Liath {

MouseManager::MouseManager(LiathEngine *engine) : _engine(engine), _count(0), _pressMouse(false), _countStopMouse(0), _timeCountStopMouse(0) {}

MouseManager::~MouseManager() {
	// Zero-out passed pointers
	_engine = nullptr;

	unload();
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void MouseManager::unload() {
	for (Common::Array<MouseBox *>::iterator it = _boxes.begin(); it != _boxes.end(); it++)
		SAFE_DELETE(*it);

	_boxes.clear();
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////

OpcodeRet MouseManager::opcodePush() {
	for (Common::Array<MouseBox *>::iterator i = _boxes.begin(); i != _boxes.end(); ++i)
		++(*i)->count;

	++_count;

	return kOpcodeRetDefault;
}

OpcodeRet MouseManager::opcodePop() {
	for (Common::Array<MouseBox *>::iterator i = _boxes.begin(); i != _boxes.end(); ++i)
		--(*i)->count;

	--_count;

	return kOpcodeRetDefault;
}

OpcodeRet MouseManager::opcodeCommandNumberBox(OpcodeParameters *parameters) {
	int32 result = numberBox(parameters->getDword(9), parameters->getDword(13));

	getGame()->letValue((ParamOrigin)parameters->getByte(0),
	                    parameters->getDword(1),
	                    parameters->getDword(5),
	                    result);

	return kOpcodeRetDefault;
}

OpcodeRet MouseManager::opcodeType(OpcodeParameters *parameters) {
	error("MouseManager::MouseManager: Not implemented!");
}

OpcodeRet MouseManager::opcodeReset(OpcodeParameters *parameters) {
	_countStopMouse = 1;
	_timeCountStopMouse = parameters->getDword(0) + g_system->getMillis() - 1501;
	_pressMouse = false;

	return kOpcodeRetDefault;
}

OpcodeRet MouseManager::opcodeKey(OpcodeParameters *parameters) {
	error("MouseManager::key: Not implemented!");
}

OpcodeRet MouseManager::opcodePress(OpcodeParameters *parameters) {
	if (_pressMouse) {
		if (_pressMouse == (bool)parameters->getDword(0)) {
			_pressMouse = true;
			return RET(true, parameters->test);
		} else {
			return RET(false, parameters->test);
		}
	}

	return RET(false, parameters->test);
}

OpcodeRet MouseManager::opcodeScroll(OpcodeParameters *parameters) {
	error("MouseManager::scroll: Not implemented!");
}

OpcodeRet MouseManager::opcodeCheck(OpcodeParameters *parameters) {
	error("MouseManager::check: Not implemented!");
}

OpcodeRet MouseManager::opcodeAddBox(OpcodeParameters *parameters) {
	error("MouseManager::addBox: Not implemented!");
}

OpcodeRet MouseManager::opcodeRemoveBox(OpcodeParameters *parameters) {
	error("MouseManager::removeBox: Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////

uint32 MouseManager::numberBox(uint32 param1, uint32 param2) {
	return getGraphics()->LPic(getGraphics()->XPic(param1), getGraphics()->YPic(param2));
}

} // End of namespace Liath
