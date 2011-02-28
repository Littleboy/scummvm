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
 *
 * $URL$
 * $Id$
 *
 */

#include "ring/game/event_ring.h"

#include "ring/base/bag.h"
#include "ring/base/puzzle.h"
#include "ring/base/rotation.h"
#include "ring/base/saveload.h"
#include "ring/base/sound.h"

#include "ring/game/application_ring.h"

#include "ring/graphics/dragControl.h"

#include "ring/helpers.h"
#include "ring/ring.h"
#include "ring/shared_ring.h"

using namespace RingGame;

namespace Ring {

EventHandlerRing::EventHandlerRing(ApplicationRing *application) : _app(application), _controlNotPressed(false) {
	// Data
	_dword_4A1C00 = 0;

	// Timer data
	_disableTimerRH = false;
	_frameNumberTimerRH = 0;
}


EventHandlerRing::~EventHandlerRing() {
	// Zero-out passed pointers
	_app = NULL;
}

#pragma region Event handling

void EventHandlerRing::onMouseLeftButtonUp(Common::Event &evt, bool isControlPressed) {
	_controlNotPressed = !isControlPressed;

	onMouseLeftButtonUp(evt);
}

void EventHandlerRing::onMouseLeftButtonUp(Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseLeftButtonUp");

	error("[EventHandlerRing::onMouseLeftButtonUp] Not implemented");
}

void EventHandlerRing::onMouseLeftButtonDown(Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseLeftButtonDown");

	error("[EventHandlerRing::onMouseLeftButtonDown] Not implemented");
}

void EventHandlerRing::onMouseRightButtonUp(Common::Event &evt) {
	debugC(kRingDebugLogic, "onMouseRightButtonUp");

	if (getApp()->getDragControl()->getField20() || getApp()->getField6F())
		return;

	Puzzle *puzzleMenu = getApp()->puzzleGet(kPuzzleMenu);
	if (puzzleMenu && puzzleMenu->getField24() == 2)
		return;

	Bag *bag = getApp()->getBag();
	if (bag->getField94()) {
		bag->sub_419350();
	} else {
		getApp()->cursorDelete();
		bag->sub_4192E0();

		if (getApp()->getCurrentRotation())
			bag->setCurrentRotationFre(getApp()->getCurrentRotation()->getFre());

		getApp()->setupCurrentRotation();
	}

}

void EventHandlerRing::onKeyDown(Common::Event &evt) {
	debugC(kRingDebugLogic, "onKeyDown");

	error("[EventHandlerRing::onKeyDown] Not implemented");
}

#pragma endregion

#pragma region Keydown

void EventHandlerRing::onKeyDownZone(Common::KeyState keyState) {
	if (_app->getCurrentZone() == kZoneSY)
		onKeyDownZoneSY(keyState);
}

void EventHandlerRing::onKeyDownZoneSY(Common::KeyState keyState) {
	if (!_app->hasCurrentPuzzle())
		return;

	PuzzleId puzzleId = _app->getCurrentPuzzleId();
	switch (puzzleId) {
	default:
		error("[EventHandlerRing::onKeyDownZoneSY] Invalid puzzle Id (%d)", puzzleId.id());

	case kPuzzleLoad:
		if (keyState.keycode == Common::KEYCODE_DELETE) {
			_app->messageGet("DoYouWantToDeleteSavedGame");
			_app->messageShowQuestion(4);
		}
		break;

	case kPuzzleSave:
		switch (keyState.keycode) {
		default:
			if (_app->objectPresentationGetTextWidth(kObjectSaveName, 0, 0) >= 280)
				return;

			*_app->getSaveManager()->getName() += (char)keyState.ascii;
			break;

		case Common::KEYCODE_ESCAPE:
			_app->getSaveManager()->getName()->clear();
			break;

		case Common::KEYCODE_BACKSPACE:
			if (_app->getSaveManager()->getName()->size() != 1)
				_app->getSaveManager()->getName()->deleteLastChar();
			break;

		case Common::KEYCODE_RETURN:
			// Do nothing
			return;
		}

		_app->objectPresentationSetTextToPuzzle(kObjectSaveName, 0, 0, *_app->getSaveManager()->getName());
		_app->objectPresentationSetTextCoordinatesToPuzzle(kObjectSaveName, 0, 0, Common::Point(344, 181));
		_app->objectPresentationSetAnimationCoordinatesOnPuzzle(kObjectSaveName, 0, Common::Point(346 + _app->objectPresentationGetTextWidth(kObjectSaveName, 0, 0), 181));
		break;
	}
}

#pragma endregion

#pragma region Zone Setup

void EventHandlerRing::onSetup(Zone zone, SetupType type) {
	debugC(kRingDebugLogic, "onSetup (zone: %s, type: %d)", _app->getZoneString(zone).c_str(), type);

	switch (zone) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		onSetupZoneNI(type);
		break;

	case kZoneRH:
		onSetupZoneRH(type);
		break;

	case kZoneFO:
		onSetupZoneFO(type);
		break;

	case kZoneRO:
		onSetupZoneRO(type);
		break;

	case kZoneWA:
		onSetupZoneWA(type);
		break;

	case kZoneAS:
		onSetupZoneAS(type);
		break;

	case kZoneN2:
		onSetupZoneN2(type);
		break;
	}
}

void EventHandlerRing::onSetupZoneNI(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->playMovie("1540");
		_app->soundPlay(14001, 2);
		_app->playMovie("1541");
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->puzzleSetActive(kPuzzle10390);
		_app->soundPlay(10001, 1);
		break;

	case kSetupType3:
		_app->timerStopAll();
		_app->puzzleSetMovabilityOn(kPuzzle10410, 0, 0);
		_app->playMovie("1550");
		_app->rotationSetAlp(10301, 160.0f);
		_app->rotationSetRan(10301, 85.7f);
		_app->rotationSetActive(10301);
		_app->soundPlay(14001, 2);
		_app->soundPlay(10021, 1);
		_app->varSetByte(10303, 1);
		break;

	case kSetupType10:
		onSetupLoadTimers("alb", 90017, 90021, 90025);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->bagAdd(kObjectGlug);
		_app->bagAdd(kObjectMinerals);
		_app->bagAdd(kObjectLogeTear);
		_app->bagAdd(kObjectDivingHelmet2);
		_app->varSetByte(10106, 1);
		_app->objectPresentationShow(kObject1);
		_app->soundPlay(10409, 2);
		_app->rotationSetActive(10415);
		break;
	}
}

void EventHandlerRing::onSetupZoneRH(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemove(kObjectAntiGCells);
		_app->soundPlay(23005, 2);
		_app->playMovie("1706");
		_app->puzzleSetActive(kPuzzle22001);
		_app->soundPlay(22001, 1);
		break;

	case kSetupType10:
		onSetupLoadTimers("alb", 90017, 90021, 90025);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->bagAdd(kObjectDivingHelmet2);
		_app->bagAdd(kObjectMedallion);
		_app->bagAdd(kObjectKeyIndifference);
		_app->bagAdd(kObjectKeyMistrust);
		_app->bagAdd(kObjectKeySelfishness);
		_app->bagAdd(kObjectKeyDisgust);
		_app->rotationSetActive(20501);
		break;
	}
}

void EventHandlerRing::onSetupZoneFO(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectLogeTear2);
		_app->timerStart(kTimer1, 3000);
		_app->timerStart(kTimer2, 5000);
		_app->timerStart(kTimer3, 17000);
		_app->timerStart(kTimer4, 10000);
		_app->timerStart(kTimer5, 10);
		_app->bagAdd(kObjectWolfInstinct);
		_app->playMovie("1217");
		_app->playMovie("1218");
		_app->rotationSetAlp(30003, 180.0f);
		_app->rotationSetActive(30003);
		break;

	case kSetupType10:
		onSetupLoadTimers("sie", 90019, 90023, 90027);
		break;

	case kSetupType999:
		_app->bagAdd(kObjectWolfInstinct);
		_app->bagAdd(kObjectKey);
		_app->bagAdd(kObjectIngot);
		_app->bagAdd(kObjectIngot2);
		_app->bagAdd(kObjectIngot3);
		_app->puzzleSetActive(kPuzzle35011);
		break;
	}
}

void EventHandlerRing::onSetupZoneRO(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupType10:
		onSetupLoadTimers("log", 90018, 90022, 90026);
		break;

	case kSetupType999:
		_app->rotationSetAlp(40000, 0.0f);
		_app->rotationSetRan(40000, 85.3f);
		_app->rotationSetActive(40000);
		_app->bagRemove(kObjectFire);
		_app->playMovie("1506");
		_app->bagAdd(kObjectFirePower);
		_app->bagAdd(kObjectRing);
		_app->bagAdd(kObjectCrown);
		_app->puzzleSetActive(kPuzzle40100);
		_app->soundPlay(40700, 1);
		break;
	}
}

void EventHandlerRing::onSetupZoneWA(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectMagicLance);
		_app->bagAdd(kObjectGolem1);
		_app->playMovie("1880");
		_app->playMovie("1881");
		_app->rotationSetAlp(50001, 320.0f);
		_app->rotationSetActive(50001);
		_app->puzzleSetActive(kPuzzle50002);
		_app->soundIsPlaying(50001);
		break;

	case kSetupType10:
		onSetupLoadTimers("bru", 90020, 90024, 90028);
		break;

	case kSetupType999:
		_app->bagAdd(kObjectMagicLance);
		_app->bagAdd(kObjectThread);
		_app->bagAdd(kObjectGolem1);
		_app->bagAdd(kObjectDragonSword);
		_app->bagAdd(kObjectFlower);
		_app->bagAdd(kObjectApple);
		_app->bagAdd(kObjectDeadLeaf);
		_app->bagAdd(kObjectBark);
		_app->varSetFloat(90008, 90.0f);
		_app->objectPresentationShow(kObject1);
		_app->varSetByte(50012, 4);
		_app->puzzleSetActive(kPuzzle50100);
		break;
	}
}

void EventHandlerRing::onSetupZoneAS(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupType5:
		_app->playMovie("1047");
		_app->rotationSetAlp(80003, 270.0f);
		_app->rotationSetBet(80003, 0.0f);
		_app->rotationSetRan(80003, 85.3f);
		_app->rotationSetActive(80003);
		break;

	case kSetupType6:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1162", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1161", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1160", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle80001);
		_app->soundPlay(80107, 1);
		break;

	case kSetupType998:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1164", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1163", _app->getLanguageChannel());
			break;
		}

		_app->playMovie("1666");
		_app->puzzleSetActive(kPuzzle80011);
		_app->soundPlay(80100, 1);
		break;

	case kSetupType999:
		_app->rotationSetAlp(80001, 90.0f);
		_app->rotationSetRan(80001, 85.3f);
		_app->rotationSetActive(80001);
		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		break;
	}
}

void EventHandlerRing::onSetupZoneN2(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->bagAdd(kObjectFire);
		_app->playMovie("1507");
		_app->playMovie("1508");
		_app->puzzleSetActive(kPuzzle72001);
		_app->objectPresentationHide(kObject72000);
		_app->cursorSelect(kCursorDefault);
		_app->soundPlay(72001, 1);
		break;

	case kSetupType10:
		onSetupLoadTimers("log", 90018, 90022, 90026);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->bagAdd(kObjectFire);
		_app->soundPlay(70000, 2);
		_app->rotationSetActive(70400);
		break;
	}
}

void EventHandlerRing::onSetupLoadTimers(Common::String zoneName, Id testId1, Id puzzleRotationId, Id testId2) {
	if (!_app->getSaveManager()->has(zoneName + ".ars"))
		error("[EventHandlerRing::onSetupLoadTimers] Cannot find savegame (%s.ars)!", zoneName.c_str());

	_app->bagRemoveAll();

	if (_app->varGetByte(testId1)) {
		_app->puzzleSetActive((PuzzleId)_app->varGetDword(puzzleRotationId));
	} else {
		_app->rotationSetActive(_app->varGetDword(puzzleRotationId));

		if (_app->varGetByte(testId2))
			_app->rotationSetFreOn(_app->varGetDword(puzzleRotationId));
		else
			_app->rotationSetFreOff(_app->varGetDword(puzzleRotationId));
	}

	if (!_app->getSaveManager()->loadSaveTimer(zoneName, kLoadSaveRead))
		error("[EventHandlerRing::onSetupLoadTimers] Cannot load timers (%s)!", zoneName.c_str());

	_app->getSoundManager()->sub_4696F0();
}

#pragma endregion

#pragma region Timer

void EventHandlerRing::onTimer(TimerId timerId) {
	debugC(kRingDebugLogic, "onTimer (id: %d)", timerId);

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
	case kZoneWA:
		break;

	case kZoneNI:
		onTimerZoneNI(timerId);
		break;

	case kZoneRH:
		onTimerZoneRH(timerId);
		break;

	case kZoneFO:
		onTimerZoneFO(timerId);
		break;

	case kZoneRO:
		onTimerZoneRO(timerId);
		break;

	case kZoneAS:
		onTimerZoneAS(timerId);
		break;

	case kZoneN2:
		onTimerZoneN2(timerId);
		break;
	}
}

void EventHandlerRing::onTimerZoneNI(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0: {
		_app->varSetByte(10432, _app->varGetByte(10432) + 1);
		if (_app->varGetByte(10432) > 10 && _app->varGetByte(10432) < 70) {
			_app->objectPresentationHide(kObject10431);
			_app->objectPresentationShow(kObject10431, (_app->varGetByte(10432) - 10) / 5);
		} else if (_app->varGetByte(10432) == 100) {
			_app->objectPresentationShow(kObject10431, 12);
		}

		uint32 mod = (120 - _app->varGetByte(10432)) / 10;
		if (mod < 5)
			mod = 5;

		if (!(_app->varGetByte(10432) % mod)) {
			_app->soundPlay(10415, 1);
			_app->objectPresentationShow(kObject10432, rnd(2) + 1);
		}

		uint32 volume = _app->varGetByte(10432) / 5 + 80;
		if (volume > 100)
			volume = 100;

		_app->soundSetVolume(10412, volume);

		if (_app->varGetByte(10412) > 120) {
			_app->timerStop(kTimer0);
			_app->soundStopType(kSoundTypeAmbientEffect, 1024);
			_app->soundStopType(kSoundTypeBackgroundMusic, 1024);
			_app->playMovie("1538");
			_app->exitToMenu(4);
		}
		}
		break;

	case kTimer1:
		_app->varSetByte(10432, _app->varGetByte(10432) + 1);
		if (_app->varGetByte(10432) > 10 && _app->varGetByte(10303) == 1) {
			_app->timerStopAll();
			_app->soundStopAll(1024);
			_app->varSetFloat(90005, 100.0f);
			_app->playMovie("1539");
			onSwitchZoneAS(1);
		}
		break;
	}
}

void EventHandlerRing::onTimerZoneRH(TimerId id) {
	if (id != kTimer0)
		return;

	// FIXME: Since we are called synchronously, is this needed? If so, replace by mutex?
	if (_disableTimerRH)
		return;

	_disableTimerRH = true;

	float alp = _app->rotationGetAlp(20401) - 35.0;
	if (alp > 0.0f && alp < 146.0f) {
		uint32 frame = (int)(alp * 0.2631578947368421f);

		if (frame >= 1 && frame <= 28 && _frameNumberTimerRH != frame) {
			_app->objectPresentationSetAnimationActiveFrame(20401, 0, frame);
			_frameNumberTimerRH = frame;
		}
	}

	_disableTimerRH = false;
}

void EventHandlerRing::onTimerZoneFO(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		switch (_app->varGetByte(30073)) {
		default:
			break;

		case 0:
			_app->objectPresentationShow(kObject30061);
			_app->varSetByte(30073, 1);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 100);
			break;

		case 1:
			_app->objectPresentationHide(kObject30061);
			_app->varSetByte(30073, 2);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 10);
			break;

		case 2:
			_app->objectPresentationShow(kObject30061);
			_app->varSetByte(30073, 3);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 100);
			break;

		case 3:
			_app->objectPresentationHide(kObject30061);
			_app->varSetByte(30073, 0);
			_app->timerStop(kTimer0);
			_app->timerStart(kTimer0, 1000 * (rnd(10) + 15));
			break;
		}
		break;

	case kTimer1: {
		Id soundId = rnd(15) + 30301;
		_app->soundSetPan(soundId, 10 - rnd(20));
		_app->soundPlay(soundId, 1);

		_app->timerStop(kTimer1);
		_app->timerStart(kTimer1, 500 * (rnd(10) + 10));
		}
		break;

	case kTimer2:
		_app->objectPresentationUnpauseAnimation(kObjectSleepingPotion, 0);
		_app->timerStop(kTimer2);
		break;

	case kTimer3: {
		Id soundId = rnd(3) + 30316;
		_app->soundSetPan(soundId, 95);
		_app->soundPlay(soundId, 1);

		_app->timerStop(kTimer3);
		_app->timerStart(kTimer3, 500 * (rnd(10) + 30));
		}
		break;

	case kTimer4: {
		Id soundId = rnd(9) + 30319;
		_app->soundSetPan(soundId, 5 - rnd(10));
		_app->soundPlay(soundId, 1);

		_app->timerStop(kTimer4);
		_app->timerStart(kTimer4, 4000 * (rnd(10) + 5));
		}
		break;

	case kTimer5:
		_app->objectPresentationUnpauseAnimation(kObject30110, 1);
		_app->objectPresentationUnpauseAnimation(kObject30109, 0);
		_app->objectSetAccessibilityOff(kObject30109, 0, 0);
		_app->timerStop(kTimer5);
		_app->timerStart(kTimer5, 2000 * rnd(10) + 15000);
		break;
	}
}

void EventHandlerRing::onTimerZoneRO(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer0:
		_app->varSetByte(40806, _app->varGetByte(40806) + 1);

		if (_app->varGetByte(40806) == 10) {
			_app->timerStop(kTimer0);
			_app->varSetByte(40601, 10);
		}

		_app->objectPresentationShow(kObject40101, _app->varGetByte(40806));
		break;

	case kTimer1:
		_app->varSetByte(40807, _app->varGetByte(40807) + 1);

		if (_app->varGetByte(40807) == 90) {
			_app->timerStop(kTimer1);
			_app->varSetByte(40602, 90);
		}

		_app->objectPresentationShow(kObject40102, _app->varGetByte(40807));
		break;
	}
}

void EventHandlerRing::onTimerZoneAS(TimerId id) {
	switch (id) {
	default:
		break;

	case kTimer2:
		_app->soundPlay(80018, 1);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 20);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004, 1);
		break;

	case kTimer3:
		_app->soundPlay(80019, 1);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 30);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004, 1);
		break;

	case kTimer4:
		_app->soundPlay(80020, 1);

		if (!_app->hasTimer(kTimer5))
			_app->timerStart(kTimer5, 10);

		if (!_app->hasTimer(kTimer6))
			_app->timerStart(kTimer6, 10);

		if (!_app->soundIsPlaying(kSoundTypeDialog))
			_app->soundPlay(rnd(12) + 80004, 1);
		break;

	case kTimer5:
		_app->varSetByte(80001, _app->varGetByte(80001) + 1);

		_app->getCurrentRotation()->setBet(_app->getCurrentRotation()->getBet() + _app->varGetFloat(80001) * _app->varGetFloat(80002));
		_app->getCurrentRotation()->setAlp(_app->getCurrentRotation()->getAlp() + _app->varGetFloat(80001) * _app->varGetFloat(80002) * 0.5f);

		// Update values
		_app->varSetFloat(80001, _app->varGetFloat(80001) * -1.0f);
		_app->varSetFloat(80002, _app->varGetFloat(80002) * 5.0f / 6.0f);

		if (_app->varGetByte(80001) > 50) {
			_app->varSetByte(80001, 0);
			_app->varSetFloat(80002, 2.0f);

			_app->timerStop(kTimer5);
			_app->timerStop(kTimer6);

			_app->objectPresentationHide(kObject80016);
		}
		break;

	case kTimer6: {
		int32 val = rnd(10) & 0x80000001;

		bool show = (val == 0);
		if (val < 0)
			show = (((val - 1) | -2) == -1);

		if (show)
			_app->objectPresentationShow(kObject80016);
		else
			_app->objectPresentationHide(kObject80016);
		}
		break;
	}
}

void EventHandlerRing::onTimerZoneN2(TimerId id) {
	if (id == kTimer0)
		_app->soundPlay(rnd(12) + 70004, 1);
}

#pragma endregion

#pragma region Bag

void EventHandlerRing::onBag(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	debugC(kRingDebugLogic, "onBag (object: %d)", id.id());

	if (puzzleRotationId == 1 && a4 == 1) {
		onBagZoneSY(id, a2, 1, 1, dragControl, a6);
		return;
	}

	switch (_app->getCurrentZone()) {
	default:
	case kZoneRH:
	case kZoneWA:
	case kZoneAS:
		break;

	case kZoneSY:
		onBagZoneSY(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;

	case kZoneNI:
		onBagZoneNI(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;

	case kZoneFO:
		onBagZoneFO(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;

	case kZoneRO:
		onBagZoneRO(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;

	case kZoneN2:
		onBagZoneN2(id, a2, puzzleRotationId, a4, dragControl, a6);
		break;
	}
}

void EventHandlerRing::onBagZoneSY(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneSY] Not implemented");
}

void EventHandlerRing::onBagZoneNI(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneNI] Not implemented");
}

void EventHandlerRing::onBagZoneFO(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneFO] Not implemented");
}

void EventHandlerRing::onBagZoneRO(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneRO] Not implemented");
}

void EventHandlerRing::onBagZoneN2(ObjectId id, uint32 a2, Id puzzleRotationId, uint32 a4, DragControl *dragControl, byte a6) {
	error("[EventHandlerRing::onBagZoneN2] Not implemented");
}

void EventHandlerRing::onBagClickedObject(ObjectId id) {
	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneFO:
		onBagClickedObjectZoneFO(id);
		break;
	}
}

void EventHandlerRing::onBagClickedObjectZoneFO(ObjectId objectId) {
	if (objectId != kObjectWolfInstinct)
		return;

	Id id = objectId;
	if (_app->hasCurrentRotation())
		id = _app->getCurrentRotationId();

	if (_app->hasCurrentPuzzle())
		id = _app->getCurrentPuzzleId();

	// Rotation
	if (id == 30302 || id == 30303) {
		if (_app->varGetByte(30017) == 1) {
			_app->rotationSetAlp(30302, _app->rotationGetAlp(30303));
			_app->rotationSetBet(30302, _app->rotationGetBet(30303));
			_app->rotationSetRan(30302, _app->rotationGetRan(30303));
			_app->rotationSetActive(30302);

			_app->varSetByte(30017, 0);
		} else {
			_app->rotationSetAlp(30303, _app->rotationGetAlp(30302));
			_app->rotationSetBet(30303, _app->rotationGetBet(30302));
			_app->rotationSetRan(30303, _app->rotationGetRan(30302));
			_app->rotationSetActive(30303);

			_app->varSetByte(30017, 1);
		}
	} else if (id == kPuzzle35020) { //  Puzzle 35020
		if (_app->varGetByte(30017) == 1) {
			_app->varSetByte(30017, 0);
			_app->objectPresentationHide(kObjectWorms, 0);
			_app->objectPresentationHide(kObjectWorms, 1);
			_app->objectSetAccessibilityOff(kObjectWorms, 0, 0);
		} else {
			_app->varSetByte(30017, 1);
			_app->objectPresentationShow(kObjectWorms, 0);
			_app->objectPresentationShow(kObjectWorms, 1);
			_app->objectSetAccessibilityOn(kObjectWorms, 0, 0);
		}
	} else if (id == kPuzzle35019) { //  Puzzle 35019
		if (_app->varGetByte(30017) == 1) {
			_app->varSetByte(30017, 0);
			_app->objectPresentationHide(kObjectNotung);
			_app->objectSetAccessibilityOff(kObjectNotung, 0, 0);
		}

		_app->varSetByte(30017, 1);
		_app->objectSetAccessibilityOn(kObjectNotung, 0, 0);

		if (_app->varGetByte(30066) != 1 && _app->varGetByte(30067) != 1 ) {
			_app->objectPresentationShow(kObjectNotung, 0);
		} else {
			_app->objectPresentationShow(kObjectNotung, 0);
			_app->objectPresentationShow(kObjectNotung, 2);
		}
	} else if (id == kPuzzle35002) { //  Puzzle 35002
		if (_app->varGetByte(30017)) {
			_app->objectPresentationHide(kObjectBerries, 1);
			_app->objectPresentationHide(kObjectBerries, 2);
			_app->objectPresentationHide(kObjectBerries, 3);
			_app->varSetByte(30017, 0);
		} else {
			_app->varSetByte(30017, 1);
			_app->objectPresentationShow(kObjectBerries, 1);

			if (_app->varGetByte(30075) == 1)
				_app->objectPresentationHide(kObjectBerries, 3);
			else
				_app->objectPresentationShow(kObjectBerries, 3);

			if (_app->varGetByte(30076) == 1)
				_app->objectPresentationHide(kObjectBerries, 2);
			else
				_app->objectPresentationShow(kObjectBerries, 2);
		}
	} else {
		return;
	}

	_app->setField78(false);
}

#pragma endregion

#pragma region Rides

void EventHandlerRing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		onBeforeRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneRH:
		onBeforeRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneFO:
		onBeforeRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneRO:
		onBeforeRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneWA:
		onBeforeRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneAS:
		onBeforeRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneN2:
		onBeforeRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom == 10005)
			if (movabilityTo == 10101) {
				_app->soundStop(10901, 1024);
				_app->soundPlay(rnd(9) + 13001, 1);
			}
		break;

	case kMovabilityRotationToPuzzle:
		switch (movabilityIndex) {
		default:
			break;

		case 41:
			if (_app->varGetByte(10420))
				_app->playMovie("1529");
			else
				_app->playMovie("1528");
			break;

		case 42:
			if (_app->varGetByte(10421))
				_app->playMovie("1531");
			else
				_app->playMovie("1530");
			break;

		case 61:
			_app->varSetWord(10600, 12);
			_app->varSetWord(10601, 0);
			_app->varSetWord(10602, 24);

			_app->objectPresentationHideAndRemove(kObject10601);
			_app->objectPresentationShow(kObject10601, 12);
			_app->objectPresentationHideAndRemove(kObject10602);
			_app->objectPresentationShow(kObject10602, 0);
			_app->objectPresentationHideAndRemove(kObject10603);
			_app->objectPresentationShow(kObject10603, 24);
			break;
		}
		break;

	case kMovabilityPuzzleToRotation:
		switch (movabilityIndex) {
		default:
			break;

		case 41:
			if (_app->varGetByte(10420))
				_app->playMovie("1534");
			else
				_app->playMovie("1533");
			break;

		case 42:
			if (_app->varGetByte(10421))
				_app->playMovie("1536");
			else
				_app->playMovie("1535");
			break;
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (movabilityTo == 10501 || movabilityTo == 10511 || movabilityTo == 10521)
			if (!movabilityIndex)
				_app->soundPlay(10501, 1);

		if (movabilityFrom == 10501 || movabilityFrom == 10511 || movabilityFrom == 10521)
			if (!movabilityIndex)
				_app->soundPlay(10502, 1);

		if (movabilityFrom == 10001) {
			_app->soundPlay(10804, 1);
		} else if (movabilityFrom == 10002) {
			_app->soundPlay(10804, 1);
			_app->soundStop(10800, 1024);
		}
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 20401)
		_app->timerStop(kTimer0);
}

void EventHandlerRing::onBeforeRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom == 30402 && movabilityTo == 30501) {
			_app->varSetByte(30037, 1);
			_app->objectPresentationShow(kObject30051, 0);
			_app->objectPresentationHide(kObject30050);
			_app->objectPresentationShow(kObject30050, 3);
			_app->rotationSetMovabilityOff(30011, 1, 2);
			_app->objectSetAccessibilityOn(kObject30051, 0, 0);
			_app->objectSetAccessibilityOn(kObject30051, 3, 3);
		}
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityFrom == 30601 && movabilityTo == 35003)
			_app->objectSetAccessibilityOn(kObjectBow, 0, 0);
		break;

	case kMovabilityPuzzleToRotation:
		switch (movabilityFrom) {
		default:
			break;

		case 35002:
			if (movabilityTo == 30801) {
				if (!_app->varGetByte(30076)) {
					_app->objectPresentationHide(kObjectBerries);
					_app->varSetByte(30017, 0);
				}
			}
			break;

		case 35003:
			if (movabilityTo == 30601)
				_app->objectPresentationHide(kObjectBow);
			break;

		case 35004:
			if (movabilityTo == 30601) {
				if (_app->varGetByte(30091) == 1 && _app->varGetByte(30021) == 1) {
					_app->playMovie("1184");
					_app->bagAdd(kObjectLogeTear2);

					if (_app->varGetByte(30020) == 1) {
						_app->bagAdd(kObjectMould);
						_app->varSetByte(30020, 0);
					}

					_app->objectPresentationHide(kObject30028);
					_app->objectPresentationShow(kObject30028, 0);
					_app->objectPresentationShow(kObject30028, 3);

					_app->varSetByte(30021, 0);
				}
			}
			break;

		case 35006:
			if (movabilityTo == 30601) {
				_app->objectPresentationHide(kObject30040);
				_app->varSetByte(30022, 0);
				_app->varSetByte(30023, 0);
				_app->objectSetAccessibilityOn(kObject30040, 0, 1);
				_app->objectSetAccessibilityOff(kObject30040, 2, 5);
			}
			break;

		case 35007:
			if (movabilityTo == 30601) {
				_app->objectPresentationHide(kObject30042);
				_app->objectSetAccessibilityOff(kObject30042, 1, 7);
				_app->objectSetAccessibilityOn(kObject30042, 0, 0);
				_app->varSetByte(30025, 0);
				_app->varSetByte(30026, 0);
				_app->varSetByte(30027, 0);
				_app->varSetByte(30028, 0);
				_app->varSetByte(30029, 0);
				_app->varSetByte(30030, 0);
				_app->varSetByte(30031, 0);
			}
			break;

		case 35008:
			if (movabilityTo == 30601) {
				if (_app->varGetByte(30032) == 1) {
					_app->bagAdd(kObjectGolem);
					_app->objectPresentationHide(30044, 0);
				}
			}
			break;

		case 35009:
			if (movabilityTo == 30601) {
				_app->objectPresentationHide(kObject30045);
			} else if (movabilityTo == 30602) {
				_app->playMovie("1205");
				_app->objectPresentationShow(kObject30045, 7);
			}
			break;

		case 30701:
		case 30702:
		case 30703:
		case 30704:
			if (_app->varGetByte(30033)) {
				_app->objectPresentationHide(kObjectFishingRod);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 1, 1);
			} else {
				_app->objectPresentationHide(kObjectFishingRod);
				_app->objectSetAccessibilityOn(kObjectFishingRod, 0, 0);
				_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 1);
			}
			break;

		case 35111:
			if (movabilityTo == 30101) {
				_app->playMovie("1206");
				_app->timerStop(kTimer5);
			}
			break;
		}

		_app->objectSetAccessibilityOff(kObjectNotung, 0, 0);
		_app->objectPresentationHide(kObjectNotung);
		_app->objectPresentationHide(kObjectWorms, 0);
		_app->objectPresentationHide(kObjectWorms, 1);
		_app->objectPresentationHide(kObjectBerries, 3);
		_app->objectPresentationHide(kObjectBerries, 1);
		_app->varSetByte(30017, 0);
		_app->objectSetAccessibilityOff(kObjectWorms, 0, 0);
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityPuzzleToRotation)
		return;

	if (movabilityFrom == 40060 || movabilityFrom == 40005) {
		_app->soundSetVolume(40102, 100);
		_app->soundPlay(40102, 2);

		if (_app->varGetByte(40804) > 4) {
			do {
				_app->objectPresentationHide(kObject40060);
				_app->objectPresentationShow(kObject40060, _app->varGetByte(40804));

				handleEvents();

				_app->varSetByte(40804, _app->varGetByte(40804) + 1);
			} while (!checkEvents() && _app->varGetByte(40804) > 0);
		}

		_app->varSetByte(40804, 0);
		_app->soundStop(40102, 1024);
		if (_app->varGetByte(40802) == 1)
			_app->playMovie("1785");

		_app->objectSetAccessibilityOff(kObject40060);
		_app->objectSetAccessibilityOn(kObject40060, 0, 0);
		_app->objectSetAccessibilityOn(kObject40201);
		_app->objectSetAccessibilityOff(kObject40202);

		_app->objectPresentationHide(kObject40060);
		_app->objectPresentationHide(kObject40201);
		_app->objectPresentationHide(kObject40202);

		_app->varSetByte(40802, 0);
		_app->varSetString(40902, "00000000");

		_dword_4A1C00 = 0;
	}
}

void EventHandlerRing::onBeforeRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityRotationToRotation)
		return;

	switch (movabilityFrom){
	default:
		break;

	case 50001:
		if (movabilityTo == 50101)
			if (!_app->soundIsPlaying(51006))
				_app->soundPlay(51006, 2);
		break;

	case 50103:
		if (movabilityTo == 50701) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51007, 2);
		}
		break;

	case 50304:
		if (movabilityTo == 50302) {
			if (_app->varGetByte(50003)) {
				if (_app->varGetByte(50005) >= 2)
					_app->playMovie("1871");
				else
					_app->playMovie("1870");
			} else if (_app->varGetByte(50005)) {
				_app->playMovie("1869");
			} else {
				_app->playMovie("1868");
			}
		}
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityPuzzleToRotation)
		return;

	switch (movabilityFrom){
	default:
		break;

	case 80006:
		if (movabilityTo == 80101) {
			if (_app->varGetByte(90001))
				_app->playMovie("1152");
			else
				_app->playMovie("1151");
		}
		break;

	case 80007:
		if (movabilityTo == 80101)
			_app->playMovie("1153");
		break;

	case 80008:
		if (movabilityTo == 80101)
			_app->playMovie("1155");
		break;

	case 80009:
		if (movabilityTo == 80101)
			_app->playMovie("1154");
		break;

	case 80010:
		if (movabilityTo == 80101)
			_app->playMovie("1156");
		break;
	}
}

void EventHandlerRing::onBeforeRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 70000 || movabilityFrom == 70001)
		_app->timerStop(kTimer0);

	if (movabilityType == kMovabilityPuzzleToPuzzle) {
		if (movabilityTo == 70501 || movabilityTo == 70511 || movabilityTo == 70521)
			_app->soundPlay(70501, 1);

		if (movabilityFrom == 70501 || movabilityFrom == 70511 || movabilityFrom == 70521)
			_app->soundPlay(70502, 1);
	}
}

void EventHandlerRing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityTo == 1)
		return;

	switch (_app->getCurrentZone()) {
	default:
	case kZoneSY:
		break;

	case kZoneNI:
		onAfterRideZoneNI(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneRH:
		onAfterRideZoneRH(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneFO:
		onAfterRideZoneFO(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneRO:
		onAfterRideZoneRO(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneWA:
		onAfterRideZoneWA(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneAS:
		onAfterRideZoneAS(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;

	case kZoneN2:
		onAfterRideZoneN2(movabilityFrom, movabilityTo, movabilityIndex, a4, movabilityType);
		break;
	}
}

void EventHandlerRing::onAfterRideZoneNI(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (a4 == 100) {
		_app->rotationSetMovabilityOff(movabilityFrom, 0, 0);
	} else if (a4 == 110) {
		if (_app->soundIsPlaying(10800))
			_app->soundStop(10800, 1024);

		if (_app->soundIsPlaying(10901))
			_app->soundPlay(10902, 1);

		_app->soundStop(10901, 1024);

		if (movabilityFrom == 10301) {
			_app->soundPlay(10300, 2);
			_app->varSetByte(10301, 0);
		}
	}

	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom == 10201) {
			if (!_app->soundIsPlaying(14002)) {
				_app->soundStop(14001, 1024);
				_app->soundStop(14003, 1024);
				_app->soundPlay(14002, 2);
			}
		}

		if (movabilityFrom > 10201 || movabilityFrom == 10101) {
			if (!_app->soundIsPlaying(14003)) {
				_app->soundStop(14002, 1024);
				_app->soundStop(14001, 1024);
				_app->soundPlay(14003, 2);
			}
		}

		if (movabilityFrom >= 10000 && movabilityFrom <= 10005) {
			if(!_app->soundIsPlaying(14001)) {
				_app->soundStop(14002, 1024);
				_app->soundStop(14003, 1024);
				_app->soundPlay(14001, 2);
			}
		}

		switch (a4) {
		default:
			break;

		case 1:
			_app->rotationSetMovabilityOff(movabilityFrom);
			_app->rotationSetMovabilityOn(movabilityFrom, 0, 0);

			if (movabilityFrom == 10000 || movabilityFrom == 10002)
				_app->rotationSetMovabilityOn(movabilityFrom, 2, 2);
			break;

		case 2:
			_app->rotationSetMovabilityOff(movabilityFrom);
			_app->rotationSetMovabilityOn(movabilityFrom, 1, 1);
			break;

		case 3:
			_app->rotationSetMovabilityOff(movabilityFrom, 0, 0);
			break;

		case 5:
		case 21:
			_app->objectPresentationHide(kObject10003, 8);
			_app->objectPresentationHide(kObject10003, 9);
			_app->objectPresentationHide(kObject10003, 10);
			_app->objectPresentationHide(kObject10003, 11);
			_app->objectPresentationHide(kObject10003, 12);
			_app->objectPresentationShow(kObject10003, 3);
			_app->objectPresentationShow(kObject10003, 4);
			_app->objectPresentationShow(kObject10003, 5);
			_app->objectPresentationShow(kObject10003, 6);
			_app->objectPresentationShow(kObject10003, 7);

			_app->rotationSetMovabilityOn(movabilityFrom, 0, 0);
			break;

		case 16:
			_app->exitToMenu(3);
			return;

		case 41:
			_app->objectPresentationHide(kObject10003, 3);
			_app->objectPresentationHide(kObject10003, 4);
			_app->objectPresentationHide(kObject10003, 5);
			_app->objectPresentationHide(kObject10003, 6);
			_app->objectPresentationHide(kObject10003, 7);
			_app->objectPresentationShow(kObject10003, 8);
			_app->objectPresentationShow(kObject10003, 9);
			_app->objectPresentationShow(kObject10003, 10);
			_app->objectPresentationShow(kObject10003, 11);
			_app->objectPresentationShow(kObject10003, 12);

			_app->rotationSetMovabilityOn(movabilityFrom, 0, 0);
			break;
		}

		if (movabilityTo == 10005) {
			if (movabilityFrom == 10101) {
				_app->soundStop(13001, 1024);
				_app->soundStop(13002, 1024);
				_app->soundStop(13003, 1024);
				_app->soundStop(13004, 1024);
				_app->soundStop(13005, 1024);
				_app->soundStop(13006, 1024);
				_app->soundStop(13007, 1024);
				_app->soundStop(13008, 1024);
				_app->soundStop(13009, 1024);

				return;
			}
		} else if (movabilityTo == 10415) {
			_app->soundStop(10409, 1024);
		}

		if (movabilityFrom == 10406) {
			if (!_app->soundIsPlaying(10410)) {
				_app->soundPlay(10410, 2);
				_app->soundPlay(10411, 2);
			}
		}
		break;

	case kMovabilityPuzzleToPuzzle:
		if (a4 == 55) {
			_app->playMovie("1537", 0.0);
			_app->puzzleSetActive(kPuzzle12001, 1, 1);
			_app->soundPlay(12001, 1);
		}
		break;
	}
}

void EventHandlerRing::onAfterRideZoneRH(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 20401)
		_app->timerStart(kTimer0, 50);

	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom >= 20010) {
			_app->varSetByte(21001, 0);

			if (_app->bagHas(kObjectKeyIndifference)) {
				_app->rotationSetMovabilityOn(20010, 0, 0);
				_app->rotationSetMovabilityOff(20010, 1, 1);
			}
		}

		if (movabilityFrom >= 20020) {
			_app->varSetByte(21001, 1);

			if (_app->bagHas(kObjectKeyMistrust)) {
				_app->rotationSetMovabilityOn(20020, 0, 0);
				_app->rotationSetMovabilityOff(20020, 1, 1);
			}
		}

		if (movabilityFrom >= 20030) {
			_app->varSetByte(21001, 2);

			if (_app->bagHas(kObjectKeySelfishness)) {
				_app->rotationSetMovabilityOn(20030, 0, 0);
				_app->rotationSetMovabilityOff(20030, 1, 1);
			}
		}
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityFrom >= 20011 && movabilityFrom <= 20031) {
			ObjectId id = 20001 + _app->varGetByte(21001);
			_app->objectPresentationShow(id, 0);
			_app->objectPresentationSetAnimationOnPuzzle(id, 2, 0, 20001);
			_app->objectPresentationShow(id, 2);
		}
		break;
	}
}

void EventHandlerRing::onAfterRideZoneFO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		switch (movabilityFrom) {
		default:
			break;

		case 30006:
			if (movabilityTo == 30008 ) {
				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 100);
			} else if (movabilityTo == 30005) {
				_app->rotationSetMovabilityRideName(30005, 1, "fom");
			}
			break;

		case 30008:
			if (movabilityTo == 30006)
				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 80);
			break;

		case 30011:
			if (movabilityTo == 30401)
				for (uint32 i = 0; i < 27; i++)
					_app->soundSetVolume(i + 30300, 100);
			break;

		case 30012:
			if (movabilityTo == 30501) {
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer3);
				_app->timerStart(kTimer1, 10);
				_app->timerStart(kTimer3, 10);
			}
			break;

		case 30101:
			if (movabilityTo == 30003) {
				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 80);

				for (uint32 i = 0; i < 14; i++)
					_app->soundSetVolume(i + 30300, 100);
			}
			break;

		case 30401:
			if (movabilityTo == 30011)
				for (uint32 i = 0; i < 27; i++)
					_app->soundSetVolume(i + 30300, 85);
			break;

		case 30501:
			if (movabilityTo == 30012) {
				_app->timerStop(kTimer1);
				_app->timerStop(kTimer3);

				if (!_app->varGetByte(30071)) {
					_app->soundPlay(30117, 1);
					_app->varSetByte(30071, 1);
				}
			}
			break;
		}
		break;

	case kMovabilityPuzzleToRotation:
		if (movabilityFrom == 30703 && movabilityTo == 35010) {
			_app->objectSetAccessibilityOn(kObjectFishingRod, 0, 0);
			_app->objectSetAccessibilityOff(kObjectFishingRod, 1, 2);
			_app->objectPresentationHide(kObjectFishingRod);
		} else if (movabilityFrom == 30301 && movabilityTo == 35011) {
			_app->objectPresentationHide(kObject30001);
			_app->objectSetAccessibilityOff(kObject30001, 1, 3);
			if (!_app->varGetByte(30043))
				_app->objectSetAccessibilityOn(kObject30001, 0, 0);
		}
		break;
	}
}

void EventHandlerRing::onAfterRideZoneRO(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityTo == 40000 || movabilityFrom == 40010)
			_app->varSetByte(40804, 0);

		if (movabilityTo == 40005 || movabilityFrom == 40060) {
			_app->soundPlay(40003, 2);
			_app->soundSetVolume(40003, 88);
		}
		break;

	case kMovabilityPuzzleToRotation:
		if (movabilityTo == 40060 || movabilityFrom == 40005)
			_app->soundSetVolume(40003, 82);
		break;
	}
}

void EventHandlerRing::onAfterRideZoneWA(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityRotationToRotation)
		return;

	switch (movabilityFrom) {
	default:
		break;

	case 50201:
		if (movabilityTo == 50102) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51002, 2);
			_app->soundPlay(51012, 2);
		}

		if (_app->varGetByte(50001) == 2) {
			if (_app->varGetByte(50003) == 1) {
				_app->objectPresentationHide(kObjectRopes, 1);
				_app->objectPresentationShow(kObjectRopes, 2);
				_app->varSetByte(50001, 3);
				_app->playMovie("1858");
				_app->bagAdd(kObjectDeadLeaf);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
				_app->varSetByte(50012, _app->varGetByte(50012) + 1);

				if (_app->varGetByte(50012) == 2) {
					_app->objectPresentationShow(kObject50700, 0);
					_app->objectSetAccessibilityOff(kObject50700);
					_app->objectSetAccessibilityOn(kObject50700, 1, 1);
					_app->rotationSetMovabilityOff(50103, 2, 2);
					_app->rotationSetMovabilityOn(50103, 3, 3);
				}

				if (_app->varGetByte(50012) == 4) {
					_app->objectPresentationShow(kObject50700, 1);
					_app->objectSetAccessibilityOff(kObject50700);
					_app->objectSetAccessibilityOn(kObject50700, 2, 2);
				}
			}
		}
		break;

	case 50102:
		if (movabilityTo == 50201) {
			_app->soundStop(51002, 1024);
			_app->soundStop(51012, 1024);
			_app->soundPlay(51006, 2);
		}
		break;

	case 50301:
		if (movabilityTo == 50104) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51004, 2);

			if (_app->varGetByte(50003) > 0)
				_app->soundPlay(51013, 2);
		}
		break;

	case 50104:
		if (movabilityTo == 50301) {
			_app->soundStop(51004, 1024);
			_app->soundStop(51013, 1024);
			_app->soundPlay(51006, 2);
		}
		break;

	case 50106:
		if (movabilityTo == 50501) {
			_app->soundStop(51001, 1024);
			_app->soundStop(51011, 1024);
			_app->soundPlay(51006, 2);
		}
		break;

	case 50103:
		if (movabilityTo == 50701) {
			_app->soundStop(51007, 1024);
			_app->soundPlay(51006, 2);
		}
		break;

	case 50108:
		if (movabilityTo == 50601)
			_app->soundPlay(51006, 2);
		break;
	}
}

void EventHandlerRing::onAfterRideZoneAS(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityType == kMovabilityPuzzleToRotation && movabilityFrom == 80101)
		if (movabilityTo == 80006 || movabilityTo == 80007 || movabilityTo == 80008
		 || movabilityTo == 80009 || movabilityTo == 80010)
			_app->varSetByte(80003, 0);
}

void EventHandlerRing::onAfterRideZoneN2(Id movabilityFrom, Id movabilityTo, uint32 movabilityIndex, uint32 a4, MovabilityType movabilityType) {
	if (movabilityFrom == 70000 || movabilityFrom == 70001)
		if (!_app->varGetByte(70012))
			_app->timerStart(kTimer0, 10000);

	if (movabilityType == kMovabilityRotationToRotation) {
		if (a4 == 7)
			sub_433EE0();
		else if (a4 == 16)
			_app->exitToMenu(1);
	}
}

#pragma endregion

#pragma region Sound

void EventHandlerRing::onSound(Id id, SoundType type, uint32 a3) {
	debugC(kRingDebugLogic, "onSound (id: %d, type: %d)", id, type);

	bool process = (a3 & 0x1000) != 0;
	a3 &= 239;

	switch (_app->getCurrentZone()) {
	default:
		break;

	case kZoneSY:
		onSoundZoneSY(id, type, a3, process);
		break;

	case kZoneNI:
		onSoundZoneNI(id, type, a3, process);
		break;

	case kZoneRH:
		onSoundZoneRH(id, type, a3, process);
		break;

	case kZoneFO:
		onSoundZoneFO(id, type, a3, process);
		break;

	case kZoneRO:
		onSoundZoneRO(id, type, a3, process);
		break;

	case kZoneAS:
		onSoundZoneAS(id, type, a3, process);
		break;

	case kZoneWA:
		onSoundZoneWA(id, type, a3, process);
		break;

	case kZoneN2:
		onSoundZoneN2(id, type, a3, process);
		break;
	}
}

void EventHandlerRing::onSoundZoneSY(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	if (id == 90001) {
		_app->objectPresentationHideAndRemove(kObject7, 0);
		_app->showCredits();
		_app->startMenu(false);
	}
}

void EventHandlerRing::onSoundZoneNI(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventHandlerRing::onSoundZoneNI] Not implemented");
}

void EventHandlerRing::onSoundZoneRH(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventHandlerRing::onSoundZoneRH] Not implemented");
}

void EventHandlerRing::onSoundZoneFO(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventHandlerRing::onSoundZoneFO] Not implemented");
}

void EventHandlerRing::onSoundZoneRO(Id id, SoundType type, uint32 a3, bool process) {
	error("[EventHandlerRing::onSoundZoneRO] Not implemented");
}

void EventHandlerRing::onSoundZoneWA(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 50021:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1875", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1873", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1874", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle51002);
		_app->soundPlay(50022, 1);
		break;

	case 50022:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1878", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1876", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1877", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle51003);
		_app->soundPlay(51008, 2);
		_app->soundPlay(50026, 1);
		break;

	case 50026:
		_app->puzzleSetActive(kPuzzle51004);
		_app->soundPlay(50027, 1);
		break;

	case 50027:
		_app->puzzleSetActive(kPuzzle51005);
		_app->soundPlay(50028, 1);
		break;

	case 50028:
		_app->puzzleSetActive(kPuzzle51006);
		_app->soundPlay(50029, 1);
		break;

	case 50029:
		_app->puzzleSetActive(kPuzzle51007);
		_app->soundPlay(50030, 1);
		break;

	case 50030:
		_app->puzzleSetActive(kPuzzle51008);
		_app->soundPlay(50031, 1);
		break;

	case 50031:
		_app->puzzleSetActive(kPuzzle51009);
		_app->soundPlay(50032, 1);
		break;

	case 50032:
		_app->puzzleSetActive(kPuzzle51010);
		_app->soundPlay(50033, 1);
		break;

	case 50033:
		_app->puzzleSetActive(kPuzzle51011);
		_app->soundPlay(50034, 1);
		break;

	case 50034:
		_app->puzzleSetActive(kPuzzle51012);
		_app->soundPlay(50035, 1);
		break;

	case 50035:
		_app->puzzleSetActive(kPuzzle51013);
		_app->soundPlay(50036, 1);
		break;

	case 50036:
		_app->playMovie("1879");
		_app->timerStopAll();
		_app->soundStopAll(1024);
		onSwitchZoneAS(4);
		break;

	case 50001:
		_app->puzzleSetActive(kPuzzle50001);
		_app->soundPlay(50002, 1);

		break;
	case 50002:
		_app->rotationSetActive(50001);
		_app->soundPlay(50003, 1);
		break;

	case 50003:
		_app->soundPlay(51006, 2);
		break;

	case 50009:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50010, 1);
		break;

	case 50010:
		_app->puzzleSetActive(kPuzzle50702);
		_app->soundPlay(50011, 1);
		break;

	case 50011:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50012, 1);
		break;

	case 50012:
		_app->puzzleSetActive(kPuzzle50701);
		_app->soundPlay(50013, 1);
		break;

	case 50013:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50014, 1);
		break;

	case 50014:
		_app->puzzleSetActive(kPuzzle50702);
		_app->soundPlay(50015, 1);
		break;

	case 50015:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50016, 1);
		break;

	case 50016:
		_app->objectSetAccessibilityOff(kObject50700);
		_app->rotationSetActive(50701);
		break;
	}
}

void EventHandlerRing::onSoundZoneAS(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 80100:
		_app->puzzleSetActive(kPuzzle80012);
		_app->soundPlay(80101, 1);
		break;

	case 80101:
		_app->puzzleSetActive(kPuzzle80011);
		_app->soundPlay(80102, 1);
		break;

	case 80102:
		_app->puzzleSetActive(kPuzzle80012);
		_app->soundPlay(80103, 1);
		break;

	case 80103:
		_app->playMovie("1157");
		_app->puzzleSetActive(kPuzzle80013);
		_app->soundPlay(80104, 1);
		break;

	case 80104:
	case 80040:
	case 80041:
	case 80042:
	case 80043:
	case 80044:
	case 80045:
	case 80046:
	case 80047:
	case 80049:
	case 80050:
	case 80051:
	case 80052:
	case 80053:
	case 80054:
	case 80055:
	case 80056:
	case 80058:
	case 80059:
	case 80060:
	case 80061:
	case 80062:
	case 80063:
	case 80064:
	case 80065:
	case 80066:
	case 80068:
	case 80069:
	case 80070:
	case 80071:
	case 80072:
	case 80073:
	case 80074:
	case 80075:
	case 80076:
	case 80077:
	case 80078:
	case 80079:
		_app->soundPlay(id + 1, 1);
		break;

	case 80105:
		_app->playMovie("1158");
		_app->puzzleSetActive(kPuzzle80014);
		_app->soundPlay(80106, 1);
		break;

	case 80106:
		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		_app->rotationSetAlp(80001, 270.0f);
		_app->rotationSetBet(80001, -26.0f);
		_app->rotationSetRan(80001, 85.3f);
		_app->rotationSetActive(80001);
		break;

	case 80048:
		_app->soundSetVolume(80201, 100);
		_app->varSetByte(80002, 2);
		break;

	case 80057:
		_app->soundSetVolume(80203, 100);
		_app->varSetByte(80002, 3);
		break;

	case 80067:
		_app->soundSetVolume(80204, 100);
		_app->varSetByte(80002, 4);
		break;

	case 80080:
		_app->soundSetVolume(80205, 100);
		_app->varSetByte(80002, 5);
		break;
	case 80107:
		_app->playMovie("1159");
		_app->showMenu(kZoneAS, 0);
		break;
	}
}

void EventHandlerRing::onSoundZoneN2(Id id, SoundType type, uint32 a3, bool process) {
	if (!process)
		return;

	switch (id){
	default:
		break;

	case 70001:
		_app->puzzleSetActive(kPuzzle70306);
		_app->soundPlay(70002, 1);
		break;

	case 70002:
		_app->puzzleSetActive(kPuzzle70303);
		_app->soundPlay(70003, 1);
		break;

	case 70003:
		_app->playMovie("1504");
		_app->playMovie("1505");
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->varSetFloat(90006, 50.0);
		onSwitchZoneRO(0);
		break;

	case 71002:
	case 71003:
		_app->playMovie(Common::String::format("N2_%dA", _app->varGetByte(70014)));
		_app->objectPresentationShow(kObject70001, _app->varGetByte(70014) - 1);
		break;

	case 72001:
		_app->puzzleSetActive(kPuzzle72002);
		_app->soundPlay(72002, 1);
		break;

	case 72002:
		_app->playMovie("1499");
		_app->puzzleSetActive(kPuzzle72003);
		_app->soundPlay(72003, 1);
		break;

	case 72003:
		_app->puzzleSetActive(kPuzzle72004);
		_app->soundPlay(72004, 1);
		break;

	case 72004:
		_app->puzzleSetActive(kPuzzle72005);
		_app->soundPlay(72005, 1);
		break;

	case 72005:
		_app->playMovie("1500");
		_app->puzzleSetActive(kPuzzle72006);
		_app->soundPlay(72006, 1);
		break;

	case 72006:
		_app->puzzleSetActive(kPuzzle72007);
		_app->soundPlay(72007, 1);
		break;

	case 72007:
		_app->puzzleSetActive(kPuzzle72008);
		_app->soundPlay(72008, 1);
		break;

	case 72008:
		_app->puzzleSetActive(kPuzzle72009);
		_app->soundPlay(72009, 1);
		break;

	case 72009:
		_app->playMovie("1501");
		_app->puzzleSetActive(kPuzzle72010);
		_app->soundPlay(72010, 1);
		break;

	case 72010:
		_app->playMovie("1502");
		_app->soundPlay(70000, 2);
		_app->varSetByte(70013, 0);
		_app->puzzleSetActive(kPuzzle70301);
		_app->soundPlay(70056, 1);
		break;

	case 70043:
	case 70044:
	case 70045:
	case 70046:
	case 70047:
	case 70048:
	case 70049:
	case 70050:
	case 70051:
	case 70052:
	case 70053:
	case 70054: {
		byte index = _app->varGetByte(70013);
		char str = _app->varGetString(70099)[index];
		if ( str > '0' )
			_app->puzzleSetActive((PuzzleId)(str + 70551));

		_app->soundPlay(id + 1, 1);

		_app->varSetByte(70013, index + 1);
		}
		break;

	case 70055:
		_app->objectSetAccessibilityOff(kObject70700, 0, 0);
		_app->objectSetAccessibilityOn(kObject70700, 1, 1);
		_app->rotationSetActive(70200);
		break;

	case 70017:
	case 70018:
	case 70019:
	case 70020:
	case 70021:
	case 70024:
	case 70025:
	case 70026:
	case 70056:
	case 70057:
	case 70058:
	case 70059:
	case 70060:
	case 70061:
	case 70062:
	case 70063:
	case 70064:
	case 70065: {
		byte index = _app->varGetByte(70013);
		char str = _app->varGetString(70099)[index];
		_app->puzzleSetActive((PuzzleId)(str + 70251));

		_app->soundPlay(id + 1, 1);

		_app->varSetByte(70013, index + 1);
		}
		break;

	case 70066:
		_app->rotationSetAlp(70300, 160.0f);
		_app->rotationSetActive(70300);
		break;

	case 70022:
		sub_433EE0();
		_app->rotationSetActive(70001);
		_app->playMovie("1503");
		_app->puzzleSetActive(kPuzzle70303);
		_app->varSetByte(70013, 51);
		_app->soundPlay(70024, 1);
		break;

	case 70101:
		_app->puzzleSetActive(kPuzzle70305);
		_app->soundPlay(70023, 1);
		break;

	case 70023:
		_app->objectSetAccessibilityOff(kObject70108);
		_app->puzzleSetActive(kPuzzle70102);
		break;

	case 70027:
		_app->puzzleSetActive(kPuzzle70000);
		_app->soundPlay(71001, 1);
		break;

	case 71001:
		_app->playMovie(Common::String::format("N2_%dA", _app->varGetByte(70014)));
		_app->objectSetAccessibilityOn(kObjectCentaur, 1, 1);
		_app->objectPresentationShow(kObject70001, _app->varGetByte(70014) - 1);
		break;

	case 71010:
	case 71011:
	case 71012:
		_app->objectPresentationHide(kObject70001);
		_app->playMovie(Common::String::format("N2_%d%c", _app->varGetByte(70014), _app->varGetByte(70015) + 67));
		_app->soundPlay(_app->varGetByte(70015) + 10 * (_app->varGetByte(70014) + 7109), 1);
		break;

	case 71100:
	case 71101:
	case 71102:
	case 71103:
	case 71104:
	case 71105:
	case 71106:
	case 71107:
	case 71108:
	case 71109:
	case 71110:
	case 71111:
	case 71112:
	case 71113:
	case 71114:
	case 71115:
	case 71116:
	case 71117:
	case 71118:
	case 71119:
	case 71120:
	case 71121:
	case 71122:
	case 71123:
	case 71124:
	case 71125:
	case 71126:
	case 71127:
	case 71128:
	case 71129:
		if (_app->varGetByte(70014) >= 3) {
			if (_app->varGetByte(70015) != 2) {
				_app->exitToMenu(2);
				break;
			}

			_app->puzzleSetActive(kPuzzle70303);
			_app->soundPlay(70001, 1);
		} else {
			if (_app->varGetByte(70014) == 2 && !_app->varGetByte(70015)) {
				_app->exitToMenu(2);
				break;
			}

			_app->soundPlay(_app->varGetByte(70014) + 71001, 1);
			_app->varSetByte(70014, _app->varGetByte(70014) + 1);

			_app->objectSetAccessibilityOn(kObjectCentaur);
			_app->objectSetAccessibilityOn(kObjectDragon);
			_app->objectSetAccessibilityOn(kObjectPhoenix1);
		}
		break;
	}
}

#pragma endregion

#pragma region Zone and helper functions

void EventHandlerRing::sub_433EE0() {
	if (_app->varGetByte(70012) == 1
	 && _app->varGetByte(70001) == 0
	 && _app->bagHas(kObjectCage)
	 && _app->bagHas(kObjectCentaur)
	 && _app->bagHas(kObjectDragon)
	 && _app->bagHas(kObjectPhoenix1)) {
		_app->varSetByte(70013, 11);
		_app->puzzleSetActive(kPuzzle70305, 1, 1);
		_app->soundPlay(70017, 1);
	}
}

void EventHandlerRing::onSwitchZoneNI(uint32 type) {
	switch (type) {
	default:
		break;

	case 0:
		if (_app->varGetByte(90009))
			_app->setupZone((Zone)_app->varGetDword(90013), kSetupType10);
		else
			_app->setupZone(kZoneNI, kSetupTypeNone);
		break;

	case 3:
		_app->setupZone(kZoneNI, kSetupType3);
		break;

	case 9:
		_app->timerStart(kTimer9, 1000);
		break;
	}
}

void EventHandlerRing::onSwitchZoneRH(uint32 type) {
	if (type == 0)
		_app->setupZone(kZoneRH, kSetupTypeNone);
}

void EventHandlerRing::onSwitchZoneFO(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90011))
			_app->setupZone((Zone)_app->varGetDword(90015), kSetupType10);
		else
			_app->setupZone(kZoneRO, kSetupTypeNone);
	}
}

void EventHandlerRing::onSwitchZoneRO(uint32 type) {
	if (type == 0)
		_app->setupZone(kZoneRO, kSetupTypeNone);
}

void EventHandlerRing::onSwitchZoneAS(uint32 type) {
	switch (type) {
	default:
		break;

	case 1:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 0, 0);
		_app->varSetByte(90001, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 2);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80040, 1);
		_app->objectSetAccessibilityOff(kObject80019, 0, 0);
		_app->objectSetAccessibilityOn(kObject80019, 1, 1);
		break;

	case 2:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 1, 1);
		_app->varSetByte(90002, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 3);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80049, 1);
		break;

	case 3:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->soundSetMultiplier(kSoundTypeAmbientMusic, 100);
		_app->objectSetAccessibilityOn(kObject80018, 2, 2);
		_app->varSetByte(90003, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);


		_app->objectPresentationShow(kObject80018, 4);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80058, 1);
		break;

	case 4:
		_app->setZoneAndEnableBag(kZoneAS);
		_app->objectSetAccessibilityOn(kObject80018, 3, 3);
		_app->varSetByte(90004, 1);
		_app->bagRemoveAll();

		if (_app->varGetByte(90001) == 1
		 && _app->varGetByte(90002) == 1
		 && _app->varGetByte(90003) == 1
		 && _app->varGetByte(90004) == 1)
			_app->bagAdd(kObjectDeath);

		_app->objectPresentationShow(kObject80018, 5);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);
		_app->soundPlay(80068, 1);
		break;

	case 5:
		_app->setupZone(kZoneAS, kSetupType5);
		break;
	case 13:
		_app->bagRemoveAll();
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->setZoneAndEnableBag(kZoneAS);
		_app->rotationSetAlp(80101, 90.0f);
		_app->rotationSetRan(80101, 85.3f);
		_app->rotationSetActive(80101);

		_app->getBag()->sub_419350();

		_app->timerStart(kTimer2, 100000);
		_app->timerStart(kTimer3, 220000);
		_app->timerStart(kTimer4, 150000);
		break;
	}
}

void EventHandlerRing::onSwitchZoneWA(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90012))
			_app->setupZone((Zone)_app->varGetDword(90016), kSetupType10);
		else
			_app->setupZone(kZoneWA, kSetupTypeNone);
	}
}

void EventHandlerRing::onSwitchZoneN2(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90010))
			_app->setupZone((Zone)_app->varGetDword(90014), kSetupType10);
		else
			_app->setupZone(kZoneN2, kSetupTypeNone);
	}
}

#pragma endregion

} // End of namespace Ring
