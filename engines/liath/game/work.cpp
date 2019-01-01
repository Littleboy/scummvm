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

#include "liath/game/work.h"

#include "liath/game/game.h"
#include "liath/game/hero.h"

#include "liath/helpers.h"
#include "liath/liath.h"

namespace Liath {

WorkManager::WorkManager(LiathEngine *engine) : _engine(engine) {}

WorkManager::~WorkManager() {
	// Zero-out passed pointers
	_engine = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Loading
//////////////////////////////////////////////////////////////////////////
void WorkManager::unload() {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++) {
		Work *work = (*it);

		if (work->status == 1) {
			SAFE_FREE(work->object->field_18);
			SAFE_FREE(work->field_84);
			SAFE_FREE(work->field_80);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Opcodes
//////////////////////////////////////////////////////////////////////////
OpcodeRet WorkManager::opcodeCel(OpcodeParameters *parameters) {
	return RET(_currentWork->field_76 == parameters->getDword(0), parameters->test);
}

OpcodeRet WorkManager::opcodeCelExt(OpcodeParameters *parameters) {
	HeroIndex heroIndex = getGame()->getValue((ParamOrigin) parameters->getByte(4), parameters->getDword(5), parameters->getDword(9));
	uint32 val = getGame()->getValue((ParamOrigin) parameters->getByte(13), parameters->getDword(14), parameters->getDword(18), true, true, true);

	Work *work = getHero()->get(heroIndex)->work;
	return RET(work && work->field_68 == val && work->field_76 == parameters->getDword(0), parameters->test);
}

OpcodeRet WorkManager::opcodeStartObject(ObjectIndex object) {
	if (_currentWork->field_58 == 1)
		return kOpcodeRetNext;

	_currentWork->field_58 = 1;
	_currentWork->objectIndex = object;

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::opcodeHeroTime(OpcodeParameters *parameters) {
	_currentWork->field_FA = parameters->getDword(0);
	_currentWork->field_FE = parameters->getDword(0);

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::opcodeHeroDark(OpcodeParameters *parameters) {
	_currentWork->field_579 = parameters->getByte(0);

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::opcodeRestart() {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++) {
		Work *work = (*it);

		if (work->status == 1) {
			SAFE_FREE(work->object->field_18);
			SAFE_FREE(work->field_84);
			SAFE_FREE(work->field_80);
		}

		SAFE_FREE(work->field_E4);
		SAFE_DELETE(work->sprite);

		if (work->workHeroData) {
			SAFE_FREE(work->workHeroData->field_B5);
			SAFE_FREE(work->workHeroData);
		}
	}

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::opcodePopHeros() {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++) {
		Work *work = (*it);

		if (!(work->field_34 & 2) && work->field_32 > 0)
			--work->field_32;
	}

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::opcodePushHeros() {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++) {
		Work *work = (*it);

		if (work->field_34 & 2)
			continue;

		++work->field_32;
	}

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::opcodeStop(OpcodeParameters *parameters) {
	Work *work = (parameters->getDword(0) ? getHero()->get(parameters->getDword(0))->work : _currentWork);

	if (work) {
		if (work->status || work->field_58 == 1 || !work->sprite)
			return kOpcodeRetNext;

		work->status = 2;
		work->field_59 = work->field_58;
		work->field_58 = 3;
	} else {
		Hero *hero = getHero()->get(parameters->getDword(0));
		if (hero->object->field_1C == 0) {
			hero->object->field_1C = 2;
			hero->object->field_24 = hero->object->isObjectIndexSet;
		}
	}

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::opcodeStopGlobal(OpcodeParameters *parameters) {
	parameters->setDword(0, GLOBAL(parameters->getDword(8)));

	return opcodeStop(parameters);
}

OpcodeRet WorkManager::opcodeStopHeroVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getHero()->getData(parameters->getDword(4), parameters->getDword(8)));

	return opcodeStop(parameters);
}

OpcodeRet WorkManager::opcodeStopVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getHero()->getData(_currentWork->heroIndex, parameters->getDword(8)));

	return opcodeStop(parameters);
}

OpcodeRet WorkManager::opcodeContinue(OpcodeParameters *parameters) {
	Hero *hero = getHero()->get(parameters->getDword(0) ? parameters->getDword(0) : _currentWork->heroIndex);
	Work *work = parameters->getDword(0) ? hero->work : _currentWork;

	if (work && work->status == 2) {
		work->status = 0;
		work->field_58 = work->field_59;
	} else if (hero->object->field_1C == 2) {
		hero->object->field_1C = 4;
		hero->object->isObjectIndexSet = hero->object->field_24;
	}

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::opcodeContinueGlobal(OpcodeParameters *parameters) {
	parameters->setDword(0, GLOBAL(parameters->getDword(8)));

	return opcodeContinue(parameters);
}

OpcodeRet WorkManager::opcodeContinueHeroVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getHero()->getData(parameters->getDword(4), parameters->getDword(8)));

	return opcodeContinue(parameters);
}

OpcodeRet WorkManager::opcodeContitnueVar(OpcodeParameters *parameters) {
	parameters->setDword(0, getHero()->getData(_currentWork->heroIndex, parameters->getDword(8)));

	return opcodeContinue(parameters);
}

OpcodeRet WorkManager::opcodeGlass(OpcodeParameters *parameters, bool doIncrement) {
	error("WorkManager::glass: Not implemented!");
}

OpcodeRet WorkManager::opcodeSavetop(OpcodeParameters *parameters) {
	_currentWork->field_B1 = _currentWork->field_A9;
	_currentWork->field_B3 = _currentWork->field_AB;
	_currentWork->field_B5 = _currentWork->field_AD;
	_currentWork->field_B7 = _currentWork->field_AF;

	return kOpcodeRetDefault;
}

OpcodeRet WorkManager::opcodeEndtop() {
	_currentWork->field_B1 = 10000;
	return kOpcodeRetDefault;
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////

Work *WorkManager::get(HeroIndex index) {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++)
		if ((*it)->heroIndex == index)
			return (*it);

	return nullptr;
}

Work *WorkManager::getFromTextIndex(int32 index) {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++)
		if ((*it)->workHeroData && (*it)->workHeroData->textIndex == index)
			return (*it);

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////////
void WorkManager::resetField_E00() {
	for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++)
		(*it)->data.resetField_E00();
}

Work *WorkManager::seWork(Work* current) {
	Common::Array<Work *>::iterator work = _works.begin();

	// Get iterator starting at current work position
	if (current != nullptr) {
		for (Common::Array<Work *>::iterator it = _works.begin(); it != _works.end(); it++)
			if (*it == current) {
				work = it;
				break;
			}
	}

	while (work != _works.end() && (*work)->field_32 > 0)
		work++;

	if (work == _works.end()) {
		// TODO set variables
		warning("[WorkManager::seWork] Missing variable set!");

		return nullptr;
	}

	return *work;
}

} // End of namespace Liath
