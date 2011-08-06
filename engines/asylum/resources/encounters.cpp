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
 */

#include "asylum/resources/encounters.h"

#include "asylum/resources/actor.h"
#include "asylum/resources/object.h"
#include "asylum/resources/script.h"
#include "asylum/resources/worldstats.h"

#include "asylum/system/cursor.h"
#include "asylum/system/graphics.h"
#include "asylum/system/screen.h"
#include "asylum/system/speech.h"
#include "asylum/system/text.h"

#include "asylum/views/scene.h"
#include "asylum/views/video.h"

#include "asylum/asylum.h"
#include "asylum/respack.h"
#include "asylum/staticres.h"

namespace Asylum {

#define KEYWORD_MASK 0xFFF

#define OPCODE_NAME(index) (index > 25 ? "INVALID" : opcodeNames[index])
const char *opcodeNames[] = {
	"Return",     // 0
	"SetScriptVar",
	"",
	"",
	"",
	"",           // 5
	"",
	"",
	"",
	"",
	"",           // 10
	"",
	"",
	"",
	"",
	"",           // 15
	"",
	"",
	"",
	"",
	"",           // 20
	"",
	"",
	"",
	"",
	""            // 25
};

Common::String Encounter::ScriptEntry::toString() {
	return Common::String::format("0x%02X: %s (%d, %d)", opcode, OPCODE_NAME(opcode), param1, param2);
}

Encounter::Encounter(AsylumEngine *engine) : _vm(engine),
	_index(0), _keywordIndex(0), _item(NULL), _objectId1(kObjectNone), _objectId2(kObjectNone), _objectId3(kObjectNone),
	_actorIndex(kActorInvalid), _shouldEnablePlayer(false), _wasPlayerDisabled(false), _flag3(false), _isScriptRunning(false) {

	memset(&_keywordIndexes, 0, sizeof(_keywordIndexes));
	_rectIndex = -1;

	_value1 = 0;
	_tick = 0;
	_data_455B14 = 0;
	_data_455B3C = 0;
	_data_455B70 = 0;
	_data_455BCC = false;
	_isDialogOpen = false;
	_shouldCloseDialog = false;
	_data_455BD8 = false;
	_data_455BDC = false;
	_data_455BE0 = false;
	_shouldCloseBackground = false;
	_data_455BE8 = false;
	_data_455BF0 = 0;
	_data_455BF4 = 0;
	_keywordStartIndex = 0;

	load();
}

Encounter::~Encounter() {
	// Pointing to existing data
	_item = NULL;

	// Zero-out passed pointers
	_vm = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Data
//////////////////////////////////////////////////////////////////////////
void Encounter::load() {
	Common::File file;

	if (!file.open("sntrm.dat"))
		error("[Encounter::load] Could not open encounter data!");

	// Load the variables
	uint16 count = file.readUint16LE();
	for (uint i = 0; i < count; i++)
		_variables.push_back(file.readSint16LE());

	// Read anvil flag
	if (file.readSint16LE())
		error("[Encounter::load] Data file not supported!");

	// Read encounter data
	int16 dataCount = file.readSint16LE();
	for (uint8 i = 0; i < dataCount; i++) {
		EncounterItem item;

		item.keywordIndex = file.readSint16LE();
		item.field2       = file.readSint16LE();
		item.scriptResourceId  = (ResourceId)file.readSint32LE();

		for (uint j = 0; j < ARRAYSIZE(item.keywords); j++)
			item.keywords[j] = file.readSint16LE();

		item.value = file.readByte();

		_items.push_back(item);
	}

	file.close();
}

void Encounter::initData() {
	memset(&_keywordIndexes, -1, sizeof(_keywordIndexes));

	uint32 currentIndex = 0;

	for (uint i = 0; i < 50; i++) {
		if (_item->keywords[i] & KEYWORD_MASK) {
			if (!(BYTE1(_item->keywords[i]) & 0x20)) {
				_keywordIndexes[currentIndex] = i;
				currentIndex++;
			}
		}
	}

	for (uint i = 0; i < 50; i++) {
		if (_item->keywords[i] & KEYWORD_MASK) {
			if (BYTE1(_item->keywords[i]) & 0x20) {
				_keywordIndexes[currentIndex] = i;
				currentIndex++;
			}
		}
	}
}

void Encounter::initBackground() {
	_background.resourceId = getWorld()->encounterFrameBg;
	_background.frameIndex = 0;
	_background.frameCount = GraphicResource::getFrameCount(_vm, _background.resourceId);
	_background.rect = GraphicResource::getFrameRect(_vm, _background.resourceId, _background.frameCount - 1);

	Common::Point point;
	Actor *player = getScene()->getActor();
	player->adjustCoordinates(&point);
	_point = Common::Point(15, (point.y < 240) ? 464 - _background.rect.height() : 15);

	_background.transTableNum = 1;
}

void Encounter::initPortraits() {
	// Portrait 1
	if (_index == 18)
		_portrait1.resourceId = getWorld()->graphicResourceIds[51];
	else
		_portrait1.resourceId = getWorld()->graphicResourceIds[encounterPortrait1Index[getWorld()->chapter == kChapter9 ? getWorld()->actorType + 9 : getWorld()->chapter]];

	if (_portrait1.resourceId == kResourceInvalid && getWorld()->chapter == kChapter1)
		_portrait1.resourceId = getWorld()->graphicResourceIds[36];

	if (_portrait1.resourceId == kResourceInvalid)
		error("[Encounter::initPortraits] No portrait 1 for this encounter!");

	_portrait1.frameIndex = 0;
	_portrait1.frameCount = GraphicResource::getFrameCount(_vm, _portrait1.resourceId);
	_portrait1.rect = GraphicResource::getFrameRect(_vm, _portrait1.resourceId, 0);
	_portrait1.transTableNum = 0;
	_portrait1.transTableMax = 3;
	_portrait1.speech0 = 0;

	// Portrait 2
	if (_index != 59)
		_portrait2.resourceId = getWorld()->graphicResourceIds[encounterPortrait2Index[_index]];
	else if (_vm->isGameFlagSet(kGameFlag353))
		_portrait2.resourceId = getWorld()->graphicResourceIds[16];
	else if (_vm->isGameFlagSet(kGameFlag354))
		_portrait2.resourceId = getWorld()->graphicResourceIds[23];
	else if (_vm->isGameFlagSet(kGameFlag355))
		_portrait2.resourceId = getWorld()->graphicResourceIds[24];

	if (_portrait2.resourceId == kResourceInvalid && getWorld()->chapter == kChapter1)
		_portrait2.resourceId = getWorld()->graphicResourceIds[36];

	if (_portrait2.resourceId == kResourceInvalid)
		error("[Encounter::initPortraits] No portrait 2 for this encounter!");

	_portrait2.frameIndex = 0;
	_portrait2.frameCount = GraphicResource::getFrameCount(_vm, _portrait2.resourceId);
	_portrait2.rect = GraphicResource::getFrameRect(_vm, _portrait2.resourceId, 0);
	_portrait2.transTableNum = 0;
	_portrait2.transTableMax = 0;
	_portrait2.speech0 = 0;
}

void Encounter::initDrawStructs() {
	for (uint i = 0; i < ARRAYSIZE(_drawingStructs); i++) {
		_drawingStructs[i].resourceId = (i == 0) ? getWorld()->smallCurUp : getWorld()->smallCurDown;
		_drawingStructs[i].frameIndex = 0;
		_drawingStructs[i].status = 0;
		_drawingStructs[i].transTableNum = -1;

		Common::Rect frameRect = GraphicResource::getFrameRect(_vm, _drawingStructs[i].resourceId, 0);
		_drawingStructs[i].point1 = Common::Point(frameRect.height(), frameRect.width());
		_drawingStructs[i].point2 = Common::Point(_point.x + _background.rect.width() + 10,
		                                          _point.y + (i == 0 ? 5 : _point.x + _background.rect.height() - 6));
	}
}

//////////////////////////////////////////////////////////////////////////
// Run
//////////////////////////////////////////////////////////////////////////
void Encounter::run(int32 encounterIndex, ObjectId objectId1, ObjectId objectId2, ActorIndex actorIndex) {
	debugC(kDebugLevelEncounter, "[Encounter] Running Encounter %d", encounterIndex);

	if (!_keywordIndex) {
		_item = &_items[0];
		_keywordIndex = _item->keywordIndex;
	}

	if (encounterIndex < 0)
		return;

	// Original engine saves the main event handler (to be restored later)
	_index = encounterIndex;
	_item = &_items[_index];
	_objectId1 = objectId1;
	_objectId2 = objectId2;
	_actorIndex = actorIndex;

	if (getWorld()->getObjectById(objectId2))
		getWorld()->getObjectById(objectId2)->stopSound();

	getScene()->getActor(actorIndex)->stopSound();

	setVariable(1, 0);
	setVariable(2, _item->value);

	Actor *player = getScene()->getActor();
	if (player->getStatus() == kActorStatusDisabled) {
		_wasPlayerDisabled = true;
	} else {
		_wasPlayerDisabled = false;
		player->updateStatus(kActorStatusDisabled);
	}

	_shouldEnablePlayer = false;

	// Setup encounter event handler
	_vm->switchEventHandler(this);
}

void Encounter::exitEncounter() {
	setVariable(1, 32767);
	initScript(_item->scriptResourceId);
	_flag3 = true;
	runScript();

	setupEntities(true);

	++_item->value;
	// Original saves the item back here

	// Update flags
	getSharedData()->setFlag(kFlagIsEncounterRunning, false);
	getSharedData()->setFlag(kFlag3, true);

	// Check if we need to re-enable the player
	if (_wasPlayerDisabled)
		_wasPlayerDisabled = false;
	else
		_shouldEnablePlayer = true;

	if (getSharedData()->getFlag(kFlagEncounterDisablePlayerOnExit))
		getScene()->getActor()->updateStatus(kActorStatusDisabled);

	getSharedData()->setFlag(kFlagEncounterDisablePlayerOnExit, false);

	if (getSound()->getMusicVolume() != Config.musicVolume)
		getSound()->setMusicVolume(Config.musicVolume);

	_vm->switchEventHandler(getScene());
}

//////////////////////////////////////////////////////////////////////////
// Message handler
//////////////////////////////////////////////////////////////////////////
bool Encounter::handleEvent(const AsylumEvent &evt) {
	switch ((int32)evt.type) {
	default:
		break;

	case EVENT_ASYLUM_INIT:
		return init();

	case EVENT_ASYLUM_UPDATE:
		return update();

	case Common::EVENT_KEYDOWN:
		return key(evt);

	case Common::EVENT_LBUTTONDOWN:
	case Common::EVENT_LBUTTONUP:
	case Common::EVENT_RBUTTONDOWN:
	case Common::EVENT_RBUTTONUP:
		return mouse(evt);
	}

	return false;
}

bool Encounter::init() {
	if (getSound()->getMusicVolume() != Config.musicVolume - 500)
		getSound()->setMusicVolume(Config.musicVolume - 500);

	if (!getSharedData()->getMatteBarHeight()) {
		getSharedData()->setFlag(kFlagIsEncounterRunning, true);
		_shouldCloseDialog = false;
		_data_455BD8 = false;
		_data_455BDC = false;
		_data_455BE0 = false;
		_shouldCloseBackground = false;
		_data_455BCC = false;
		_data_455B3C = 1;
		_rectIndex = -1;
		_value1 = 0;
		_data_455BF4 = 0;
		_keywordStartIndex = 0;
		_data_455B14 = -1;

		getSpeech()->resetTextData();

		initData();

		getText()->loadFont(getWorld()->font1);

		initBackground();
		initPortraits();
		initDrawStructs();
	}

	_isDialogOpen = false;
	getCursor()->set(getWorld()->cursorResources[kCursorResourceTalkNPC], -1, kCursorAnimationMirror);

	if (!getSharedData()->getMatteBarHeight())
		initScript(_item->scriptResourceId);

	return true;
}

bool Encounter::update() {
	if (getSound()->getMusicVolume() != Config.musicVolume - 500)
		getSound()->setMusicVolume(Config.musicVolume - 500);

	uint32 tick = _vm->getTick();
	ResourceId id = kResourceNone;

	if (_objectId3) {
		_isDialogOpen = false;

		Object *object = getWorld()->getObjectById(_objectId3);
		id = object->getResourceId();

		if (object->getFrameIndex() == object->getFrameCount() - 1) {
			switch (getVariable(3)) {
			default:
				break;

			case 13:
				if (_actorIndex)
					getScene()->getActor(_actorIndex)->show();
				else
					setupEntities(true);
				break;

			case 14:
				getScene()->getActor()->show();
				break;

			case 15:
				if (_actorIndex)
					getScene()->getActor(_actorIndex)->show();
				else
					setupEntities(true);

				getScene()->getActor()->show();
				break;
			}

			object->disable();
			_objectId3 = kObjectNone;
			_data_455BF4 = 0;
			getCursor()->show();
		}
	}

	if (_data_455BE8) {
		if (getSharedData()->getMatteBarHeight()) {
			_isDialogOpen = false;
		} else {
			getCursor()->show();
			_data_455BE8 = false;
			_data_455BF4 = 0;
		}
	}

	if (_isDialogOpen) {
		if (_data_455BF4 == 1) {
			_data_455BF4 = 2;
			runScript();
		}

		bool doScript = false;
		if ((getSpeech()->getSoundResourceId()
		 && !getSound()->isPlaying(getSpeech()->getSoundResourceId())
		 && !_data_455BE0)
		 || (getSpeech()->getTick() && tick >= getSpeech()->getTick()))
			doScript = true;

		if (!getSharedData()->getMatteBarHeight() && doScript && _isScriptRunning) {
			if (!setupSpeech(id))
				runScript();
		}
	}

	// Redraw screen
	if (!getSharedData()->getFlag(kFlagRedraw)) {
		if (updateScreen())
			return true;

		getSharedData()->setFlag(kFlagRedraw, true);
	}

	if (tick >= getSharedData()->getNextScreenUpdate() && getSharedData()->getFlag(kFlagRedraw)) {
		if (getSharedData()->getMatteBarHeight() == 0) {
			getScreen()->copyBackBufferToScreen();
		} else {
			drawScreen();
		}

		getSharedData()->setFlag(kFlagRedraw, false);
		getSharedData()->setNextScreenUpdate(tick + 55);
	}

	return true;
}

bool Encounter::key(const AsylumEvent &evt) {
	switch (evt.kbd.keycode) {
	default:
		break;

	case Common::KEYCODE_TAB:
		getScreen()->takeScreenshot();
		break;

	case Common::KEYCODE_ESCAPE:
		if (!isSpeaking()
		 && _isDialogOpen
		 && !getSpeech()->getTextData()
		 && !getSpeech()->getTextDataPos())
			_shouldCloseDialog = true;
		break;
	}

	return true;
}

bool Encounter::mouse(const AsylumEvent &evt) {
	switch (evt.type) {
	default:
		break;

	case Common::EVENT_LBUTTONDOWN:
		if (!_data_455BD8) {
			_data_455BD8 = true;
			_rectIndex = findRect();

			if (_rectIndex != -1)
				updateDrawingStatus2(_rectIndex);
		}
		break;

	case Common::EVENT_LBUTTONUP:
		if (_rectIndex == -1) {
			if (!isSpeaking())
				choose(getKeywordIndex());

			_data_455BD8 = false;
		} else {
			_rectIndex = -1;
			updateDrawingStatus1(_rectIndex);
			_data_455BD8 = false;
		}
		break;


	case Common::EVENT_RBUTTONDOWN:
		if (!isSpeaking()
		 && _isDialogOpen
		 && !getSpeech()->getTextData()
		 && !getSpeech()->getTextDataPos())
			_shouldCloseDialog = true;
		break;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////////
void Encounter::setVariable(uint32 index, int16 val) {
	if (index >= _variables.size())
		error("[Encounter::setVariable] Invalid index (was: %d, max: %d)", index, _variables.size() - 1);

	_variables[index] = val;
}

int16 Encounter::getVariable(uint32 index) {
	if (index >= _variables.size())
		error("[Encounter::getVariable] Invalid index (was: %d, max: %d)", index, _variables.size() - 1);

	return _variables[index];
}

int16 Encounter::getVariableInv(int16 index) {
	if (index >= 0)
		return index;

	return getVariable((uint16)-index);
}

//////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////
uint32 Encounter::findKeyword(EncounterItem *item, int16 keyword) const {
	for (uint i = 0; i < ARRAYSIZE(item->keywords); i++) {
		if ((item->keywords[i] & KEYWORD_MASK) == keyword)
			return i;
	}

	error("[Encounter::findKeyword] Could not find a valid keyword!");
}

int32 Encounter::getKeywordIndex() {
	Common::Point mousePos = getCursor()->position();

	int16 counter = 0;
	for (uint i = _keywordStartIndex; i < ARRAYSIZE(_keywordIndexes); i++) {
		int32 index = _keywordIndexes[i];

		if (counter / 3 >= 8)
			break;

		if ((_item->keywords[index] & KEYWORD_MASK) && (BYTE1(_item->keywords[index]) & 0x80)) {
			int32 x = _drawingStructs[0].point1.x + 144 * (counter % 3) + _point.x + (counter % 3) + _portrait1.rect.width() + 15;
			int32 y = 16 * counter / 3 + _point.y + 5;

			if (mousePos.x >= x && mousePos.x <= (x + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 3681 + index)))
			 && mousePos.y >= y && mousePos.y <= (y + 16))
				return i;

			++counter;
		}
	}

	return -1;
}

void Encounter::choose(int32 index) {
	if (_isScriptRunning || index == -1)
		return;

	if ((_item->keywords[index] & KEYWORD_MASK) && (BYTE1(_item->keywords[index]) & 0x80)) {

		_value1 = (_item->keywords[index] & KEYWORD_MASK);
		setVariable(1, _value1);

		if (strcmp("Goodbye", getText()->get(MAKE_RESOURCE(kResourcePackText, 3681 + index))))
			if (_index != 79)
				BYTE1(_item->keywords[index]) |= 0x20;

		initScript(_item->scriptResourceId);
		runScript();
	}
}

bool Encounter::checkKeywords() const {
	if (_data_455B14 == -1 || _data_455B14 + 1 >= 50)
		return false;

	for (int32 i = _data_455B14 + 1; i < ARRAYSIZE(_keywordIndexes); i++) {
		int32 index = _keywordIndexes[i];

		if (index < 0)
			continue;

		if ((_item->keywords[index] & KEYWORD_MASK) && (BYTE1(_item->keywords[index]) & 0x80))
			return true;
	}

	return false;
}

bool Encounter::checkKeywords2() const {
	for (uint32 i = 0; i < _keywordStartIndex; i++) {
		int32 index = _keywordIndexes[i];

		if (index < 0)
			continue;

		if ((_item->keywords[index] & KEYWORD_MASK) && (BYTE1(_item->keywords[index]) & 0x80))
			return true;
	}

	return false;
}

void Encounter::updateFromRect(int32 rectIndex)  {
	if (rectIndex) {
		if (rectIndex == 1 && (_data_455B14 + 1) < 50) {
			bool cont = false;

			for (int32 i = _data_455B14 + 1; i < ARRAYSIZE(_keywordIndexes); i++) {
				int32 index = _keywordIndexes[i];

				if (index < 0)
					continue;

				if ((_item->keywords[index] & KEYWORD_MASK) && (BYTE1(_item->keywords[index]) & 0x80)) {
					cont = true;
					break;
				}
			}

			if (!cont)
				return;

			uint32 counter = 0;
			for (uint32 i = _keywordStartIndex + 1; i < ARRAYSIZE(_keywordIndexes); i++) {
				int32 index = _keywordIndexes[i];

				if (counter == 3)
					break;

				if (index < 0)
					continue;

				if ((_item->keywords[index] & KEYWORD_MASK) && (BYTE1(_item->keywords[index]) & 0x80)) {
					_keywordStartIndex = i;
					++counter;
				}
			}
		}
	} else {
		uint32 counter = 0;
		for (int32 i = (int32)_keywordStartIndex - 1; i > -1; i--) {
			int32 index = _keywordIndexes[i];

			if (counter == 3)
				break;

			if (index < 0)
				continue;

			if ((_item->keywords[index] & KEYWORD_MASK) && (BYTE1(_item->keywords[index]) & 0x80)) {
				_keywordStartIndex = (uint32)i;
				++counter;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Speech
//////////////////////////////////////////////////////////////////////////
void Encounter::resetSpeech(int16 keywordIndex, int16 a2) {
	getSpeech()->resetTextData();
	setupPortraits();

	_data_455BCC = false;
	_data_455B3C = 1;

	if (keywordIndex) {
		getSpeech()->setTextResourceId(keywordIndex + a2);
		setupSpeechText();
	}
}

void Encounter::setupPortraits() {
	_portrait1.transTableMax = 3;
	_portrait2.transTableMax = 0;
	setupSpeechData('N', &_portrait1);
	setupSpeechData('N', &_portrait2);
}

void Encounter::setupSpeechData(char val, EncounterGraphic *encounterGraphic) const {
	switch (val) {
	default:
		break;

	case 'N':
		encounterGraphic->speech3 = 0;
		break;

	case 'H':
		encounterGraphic->speech3 = 1;
		break;

	case 'E':
		encounterGraphic->speech3 = 2;
		break;

	case 'S':
		encounterGraphic->speech3 = 3;
		break;
	}
}

void Encounter::setupSpeechText() {
	setupSpeechData('N', &_portrait1);
	setupSpeechData('N', &_portrait2);

	char *text = getText()->get(getSpeech()->getTextResourceId());

	if (*text == '{') {
		_portrait1.transTableMax = 3;
		_portrait2.transTableMax = 0;

		getSpeech()->setTextData(text + 3);
		getSpeech()->setTextDataPos(NULL);

		setupSpeech(getSpeech()->getTextResourceId(), getWorld()->font1);
	} else {
		_portrait1.transTableMax = 0;
		_portrait2.transTableMax = 3;

		getSpeech()->setTextData(NULL);
		if (*text == '/')
			getSpeech()->setTextDataPos(text + 2);

		setupSpeech(getSpeech()->getTextResourceId(), getWorld()->font3);
	}

	_data_455BCC = false;
	_data_455B3C = 1;
}

void Encounter::setupSpeech(ResourceId textResourceId, ResourceId fontResourceId) {
	getText()->loadFont(fontResourceId);

	char *text = getText()->get(textResourceId);

	if (*text == '{') {
		_data_455BDC = true;
		setupEntities(true);
		setupSpeechData(text[1], &_portrait1);
	} else {
		_data_455BDC = false;
		setupEntities(false);
		if (*text == '/') {
			char *c = text + 1;
			while (*c != ' ') {
				setupSpeechData(*(c + 1), &_portrait2);
				c++;
			}
		}
	}

	_data_455BE0 = true;
	getSpeech()->setSoundResourceId(MAKE_RESOURCE(kResourcePackSharedSound, textResourceId - _keywordIndex));
}

bool Encounter::setupSpeech(ResourceId id) {
	getSpeech()->setTick(0);
	getSpeech()->setSoundResourceId(kResourceNone);

	setupEntities(false);

	char *text = getText()->get(id);
	if (text[strlen(text) - 2] == 1) {
		setupEntities(true);
		getSpeech()->setTextResourceId(kResourceNone);
		getSpeech()->setTextData(0);
		getSpeech()->setTextDataPos(0);

		_data_455BCC = false;
		_data_455B3C = 1;

		setupPortraits();

		return false;
	}

	getSpeech()->setTextResourceId(getSpeech()->getTextResourceId() + 1);
	setupSpeechText();

	return true;
}

bool Encounter::isSpeaking() {
	if (!_isScriptRunning)
		return false;

	if (getSpeech()->getSoundResourceId() != kResourceNone && getSound()->isPlaying(getSpeech()->getSoundResourceId())) {
		getSound()->stopAll(getSpeech()->getSoundResourceId());
		return true;
	}

	if (getSpeech()->getTick()) {
		getSpeech()->setTick(_vm->getTick());
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// Drawing
//////////////////////////////////////////////////////////////////////////
bool Encounter::drawBackground() {
	if (Config.performance > 1) {
		if (_vm->isGameFlagSet(kGameFlag528)) {
			Common::Point origin;

			getScreen()->addGraphicToQueueCrossfade(_background.resourceId,
			                                        _background.frameIndex,
			                                        _point,
			                                        getWorld()->getObjectById(kObjectCave)->getResourceId(),
			                                        origin,
			                                        (uint32)_background.transTableNum);
		} else {
			Common::Point origin(getWorld()->xLeft, getWorld()->yTop);

			getScreen()->addGraphicToQueueCrossfade(_background.resourceId,
			                                        _background.frameIndex,
			                                        _point,
			                                        getWorld()->backgroundImage,
			                                        origin,
			                                        (uint32)_background.transTableNum);
		}
	} else {
		getScreen()->draw(_background.resourceId, _background.frameIndex, _point);
	}

	if (_shouldCloseBackground) {
		--_background.frameIndex;

		if (_background.frameIndex == 0)
			exitEncounter();

		return false;
	}

	if (_background.frameIndex < _background.frameCount - 1) {
		++_background.frameIndex;

		return false;
	}

	return true;
}

bool Encounter::drawPortraits() {
	bool ret = true;

	if (_shouldCloseDialog) {
		_portrait1.transTableMax = 0;
		_portrait2.transTableMax = 0;
	}

	// Portrait 1
	if (_portrait1.transTableNum == 3 )
		getScreen()->draw(_portrait1.resourceId,
		                  _portrait1.frameIndex,
		                  _point + Common::Point(5, 5));
	else
		getScreen()->drawTransparent(_portrait1.resourceId,
		                             _portrait1.frameIndex,
		                             _point + Common::Point(5, 5),
		                             kDrawFlagNone,
		                             (uint32)_portrait1.transTableNum);

	if (_portrait1.transTableNum != _portrait1.transTableMax) {
		if (_portrait1.transTableNum <= _portrait1.transTableMax)
			++_portrait1.transTableNum;
		else
			--_portrait1.transTableNum;

		ret = false;
	}

	if (_portrait1.speech0) {
		if (_portrait1.speech0 == 1)
			++_portrait1.frameIndex;
		else
			--_portrait1.frameIndex;

		_portrait1.frameIndex %= _portrait1.frameCount;
	}

	// Portrait 2
	Common::Rect frameRect = GraphicResource::getFrameRect(_vm, _portrait2.resourceId, _portrait2.frameIndex);

	if (_portrait2.transTableNum == 3)
		getScreen()->draw(_portrait2.resourceId,
		                  _portrait2.frameIndex,
		                  Common::Point(_point.x + _background.rect.width() - (frameRect.width() + 6), _point.y + 5));
	else
		getScreen()->drawTransparent(_portrait2.resourceId,
		                             _portrait2.frameIndex,
		                             Common::Point(_point.x + _background.rect.width() - (frameRect.width() + 6), _point.y + 5),
		                             kDrawFlagNone,
		                             (uint32)_portrait2.transTableNum);

	if (_portrait2.transTableNum != _portrait2.transTableMax) {
		if (_portrait2.transTableNum <= _portrait2.transTableMax)
			++_portrait2.transTableNum;
		else
			--_portrait2.transTableNum;

		ret = false;
	}

	if (_portrait2.speech0) {
		if (_portrait2.speech0 == 1)
			++_portrait2.frameIndex;
		else
			--_portrait2.frameIndex;

		_portrait2.frameIndex %= _portrait2.frameCount;
	}

	if (_shouldCloseDialog)
		if (_portrait1.transTableNum == _portrait1.transTableMax
		 && _portrait2.transTableNum == _portrait2.transTableMax)
			_shouldCloseBackground = true;

	return ret;
}

void Encounter::drawStructs() {
	// Drawing structure 1
	if (_drawingStructs[0].transTableNum < -1 || _drawingStructs[0].transTableNum > 3)
		error("[Encounter::drawStructs] Something got <redacted> wrong!");

	if (checkKeywords2() || _drawingStructs[0].transTableNum > -1) {
		int32 val = _drawingStructs[0].transTableNum;

		if (_shouldCloseDialog
		 && _drawingStructs[0].status != 2
		 && _drawingStructs[0].transTableNum > -1) {
			val = _drawingStructs[0].transTableNum - 1;
			_drawingStructs[0].status = 2;
			--_drawingStructs[0].transTableNum;
		}

		switch (val) {
		default:
			break;

		case -1:
			getScreen()->drawTransparent(_drawingStructs[0].resourceId,
			                             _drawingStructs[0].frameIndex,
			                             _drawingStructs[0].point2,
			                             kDrawFlagNone,
			                             0);

			_drawingStructs[0].status = 0;
			break;

		case 0:
		case 1:
		case 2:
			getScreen()->drawTransparent(_drawingStructs[0].resourceId,
			                             _drawingStructs[0].frameIndex,
			                             _drawingStructs[0].point2,
			                             kDrawFlagNone,
			                             (uint32)val);

			if (_drawingStructs[0].status == 1)
				++_drawingStructs[0].transTableNum;
			else if (_drawingStructs[0].status == 2)
				--_drawingStructs[0].transTableNum;

			break;

		case 3:
			getScreen()->draw(_drawingStructs[0].resourceId,
			                  _drawingStructs[0].frameIndex,
			                  _drawingStructs[0].point2);

			_drawingStructs[0].status = 0;
			break;
		}
	}

	// Drawing structure 2
	if (_drawingStructs[1].transTableNum < -1 || _drawingStructs[1].transTableNum > 3)
		error("[Encounter::drawStructs] Something got <redacted> wrong!");

	if (checkKeywords() || _drawingStructs[1].transTableNum > -1) {
		int32 val = _drawingStructs[1].transTableNum;

		if (_shouldCloseDialog
		 && _drawingStructs[1].status != 2
		 && _drawingStructs[1].transTableNum > -1) {
			val = _drawingStructs[1].transTableNum - 1;
			_drawingStructs[1].status = 2;
			--_drawingStructs[1].transTableNum;
		}

		switch (val) {
		default:
			break;

		case -1:
			getScreen()->drawTransparent(_drawingStructs[1].resourceId,
			                             _drawingStructs[1].frameIndex,
			                             _drawingStructs[1].point2,
			                             kDrawFlagNone,
			                             0);

			_drawingStructs[1].status = 0;
			break;

		case 0:
		case 1:
		case 2:
			getScreen()->drawTransparent(_drawingStructs[1].resourceId,
			                             _drawingStructs[1].frameIndex,
			                             _drawingStructs[1].point2,
			                             kDrawFlagNone,
			                             (uint32)val);

			if (_drawingStructs[1].status == 1)
				++_drawingStructs[1].transTableNum;
			else if (_drawingStructs[1].status == 2)
				--_drawingStructs[1].transTableNum;

			break;

		case 3:
			getScreen()->draw(_drawingStructs[1].resourceId,
			                  _drawingStructs[1].frameIndex,
			                  _drawingStructs[1].point2);

			_drawingStructs[1].status = 0;
			break;
		}
	}
}

void Encounter::drawDialog() {
	getText()->loadFont(getWorld()->font1);

	if (_keywordStartIndex >= 50)
		return;

	int16 counter = 0;

	for (uint32 i = _keywordStartIndex; i < ARRAYSIZE(_keywordIndexes); i++) {
		if (counter / 3 >= 8)
			return;

		int32 keywordIndex = _keywordIndexes[i];
		if (keywordIndex < 0)
			continue;

		int16 keyword = _item->keywords[keywordIndex];
		if ((keyword & KEYWORD_MASK) > 0 && (BYTE1(keyword) & 0x80)) {

			if (BYTE1(keyword) & 0x20)
				getText()->loadFont(getWorld()->font2);
			else
				getText()->loadFont(getWorld()->font1);

			Common::Point coords(_drawingStructs[0].point1.y + 144 * (counter % 3) + _point.x + (counter % 3) + _portrait1.rect.width() + 15,
			                     _point.y + (int16)(16 * counter / 3));

			if (getKeywordIndex() == keywordIndex)
				getScreen()->fillRect(coords.x - 1, coords.y + 5, getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 3681 + (keyword & KEYWORD_MASK))), 18, 0);

			getText()->setPosition(coords);
			getText()->draw(MAKE_RESOURCE(kResourcePackText, 3681 + (keyword & KEYWORD_MASK)));

			++counter;
			_data_455B14 = i;
		}
	}
}

void Encounter::drawText(char *text, ResourceId font, int16 y) {
	if (!text)
		return;

	int16 width = _background.rect.width() - (_portrait1.rect.width() + _portrait2.rect.width() + 20);
	int16 x = _point.x + _portrait1.rect.width() + 10;

	getText()->loadFont(font);

	if (_data_455BCC) {
		if (_data_455B3C != 1 && _tick < _vm->getTick()) {
			_tick = _vm->getTick() + 1000 * (getResource()->get(getSpeech()->getSoundResourceId())->size / 11025) / (uint16)_data_455B3C;

			if ((_data_455BF0 + 8) < _data_455B70)
				_data_455BF0 += 8;
		}
	} else {
		_data_455BCC = true;
		_data_455B70 = getText()->draw(kTextCalculate, Common::Point(x, y), 16, width, text);
		_data_455B3C = _data_455B70 / 8 + 1;
		_data_455BF0 = 0;
		_tick = _vm->getTick() + 1000 * (getResource()->get(getSpeech()->getSoundResourceId())->size / 11025) / (uint16)_data_455B3C;
	}

	getText()->draw(_data_455BF0, 7, kTextCenter, Common::Point(x, y), 16, width, text);
}

void Encounter::drawScreen() {
	getScene()->getActor()->setLastScreenUpdate(_vm->screenUpdateCount);

	if (!getSharedData()->getMatteInitialized())
		getSharedData()->setMatteBarHeight(85);

	if (getSharedData()->getMatteBarHeight() >= 84) {
		if (getSharedData()->getMatteBarHeight() == 85) {
			if (getSharedData()->getMatteInitialized()) {
				getScreen()->drawWideScreenBars(82);

				getScreen()->updatePalette();
				getScreen()->setupPalette(NULL, 0, 0);
				getScreen()->paletteFade(0, 25, 10);
			} else {
				getSharedData()->setMatteInitialized(true);
				getScreen()->clear();
			}

			if (getSharedData()->getMatteVar1()) {
				if (!getSharedData()->getMatteVar2())
					getSound()->playMusic(kResourceNone, 0);

				// Play movie
				getScreen()->clear();
				getVideo()->play(getSharedData()->movieIndex, getSharedData()->getFlag(kFlagIsEncounterRunning) ? (EventHandler*)this : getScene());
				getScreen()->clearGraphicsInQueue();
				getScreen()->clear();
				getCursor()->hide();

				if (getSharedData()->getMattePlaySound()) {
					getScreen()->paletteFade(0, 2, 1);
					getScene()->updateScreen();
					getScreen()->drawWideScreenBars(82);

					getScreen()->updatePalette();
					getScreen()->setupPalette(NULL, 0, 0);

					if (getSharedData()->getMattePlaySound() /* Scene::updateScreen() does script processing, so the value might have changed */
					 && !getSharedData()->getMatteVar2()
					 && getWorld()->musicCurrentResourceIndex != kMusicStopped)
						getSound()->playMusic(MAKE_RESOURCE(kResourcePackMusic, getWorld()->musicCurrentResourceIndex));
				}

				getSharedData()->setMatteBarHeight(getSharedData()->getMattePlaySound() ? 346 : 170);
			} else {
				getSharedData()->setMatteBarHeight(170);
			}
		} else if (getSharedData()->getMatteBarHeight() >= 170) {
			if (getSharedData()->getFlag(kFlagIsEncounterRunning)) {
				getSharedData()->setMatteBarHeight(0);
				getCursor()->show();
			}
		} else {
			getScreen()->drawWideScreenBars(172 - getSharedData()->getMatteBarHeight());
			getSharedData()->setMatteBarHeight(getSharedData()->getMatteBarHeight() + 4);

			ResourceId paletteId = getWorld()->actions[getScene()->getActor()->getActionIndex3()]->paletteResourceId;
			getScreen()->setPaletteGamma(paletteId ? paletteId : getWorld()->currentPaletteId);

			updatePalette1();
			getScreen()->setupPalette(NULL, 0, 0);
		}
	} else {
		getScreen()->drawWideScreenBars(getSharedData()->getMatteBarHeight());
		getSharedData()->setMatteBarHeight(getSharedData()->getMatteBarHeight() + 4);

		getScreen()->setPaletteGamma(getWorld()->currentPaletteId);

		updatePalette2();
		getScreen()->setupPalette(NULL, 0, 0);
	}
}

//////////////////////////////////////////////////////////////////////////
// Misc
//////////////////////////////////////////////////////////////////////////
void Encounter::setupEntities(bool type4) {
	// Actor
	if (_actorIndex) {
		Actor *actor = getScene()->getActor(_actorIndex);

		if (actor->isDefaultDirection(20))
			actor->updateStatus(type4 ? kActorStatusDisabled : kActorStatus8);

		return;
	}

	// Objects
	if (_objectId1 == _objectId2)
		return;

	if (_index == 37) {
		if (getVariable(3)) {
			if (getVariable(3) == 1 && _vm->isGameFlagNotSet(kGameFlag470)) {
				getWorld()->getObjectById(type4 ? kObjectNPC026Talking : kObjectNPC026TalkStatusQuo)->disable();
				getWorld()->getObjectById(type4 ? kObjectNPC026TalkStatusQuo : kObjectNPC026Talking)->setNextFrame(getWorld()->getObjectById(type4 ? kObjectNPC026TalkStatusQuo : kObjectNPC026Talking)->flags);
			}
		} else {
			getWorld()->getObjectById(type4 ? _objectId1 : _objectId2)->disable();
			getWorld()->getObjectById(type4 ? _objectId2 : _objectId1)->setNextFrame(getWorld()->getObjectById(type4 ? _objectId2 : _objectId1)->flags);
		}
	} else {
		if (type4) {
			getWorld()->getObjectById(_objectId1)->disable();
			getWorld()->getObjectById(_objectId2)->setNextFrame(getWorld()->getObjectById(_objectId2)->flags);
		} else {
			if (_index == 5) {
				if (!_vm->isGameFlagSet(kGameFlag262)) {
					getWorld()->getObjectById(kObjectMariaPointsLeft)->disable();
					getWorld()->getObjectById(_objectId2)->disable();

					_objectId1 = kObjectMariaPointsRight;
					getWorld()->getObjectById(_objectId1)->setNextFrame(getWorld()->getObjectById(_objectId1)->flags);
				} else {
					getWorld()->getObjectById(_objectId1)->disable();
					getWorld()->getObjectById(_objectId2)->disable();

					_objectId1 = kObjectMariaPointsLeft;
					getWorld()->getObjectById(_objectId1)->setNextFrame(getWorld()->getObjectById(_objectId1)->flags);
				}
			} else {
				getWorld()->getObjectById(_objectId2)->disable();
				getWorld()->getObjectById(_objectId1)->setNextFrame(getWorld()->getObjectById(_objectId1)->flags);
			}
		}
	}
}

int32 Encounter::findRect() {
	Common::Point mousePos = getCursor()->position();

	for (uint32 i = 0; i < ARRAYSIZE(_drawingStructs); i++) {
		EncounterDrawingStruct *drawStruct = &_drawingStructs[i];

		if (mousePos.x >= drawStruct->point2.x
		 && mousePos.x < (drawStruct->point2.x + drawStruct->point1.y)
		 && mousePos.y >= drawStruct->point2.y
		 && mousePos.y < (drawStruct->point2.y + drawStruct->point1.x))
			return i;
	}

	return -1;
}

void Encounter::updateDrawingStatus() {
	if (checkKeywords2()) {
		if (_rectIndex) {
			if (_drawingStructs[0].transTableNum == -1) {
				_drawingStructs[0].status = 1;
				_drawingStructs[0].transTableNum = 0;
			}
		}
	} else {
		if (_drawingStructs[0].transTableNum == 3) {
			_drawingStructs[0].status = 2;
			_drawingStructs[0].transTableNum = 2;
		}
	}

	if (checkKeywords()) {
		if (_rectIndex != 1) {
			if (_drawingStructs[1].transTableNum == -1) {
				_drawingStructs[1].status = 1;
				_drawingStructs[1].transTableNum = 0;
			}
		}
	} else {
		if (_drawingStructs[1].transTableNum == 3) {
			_drawingStructs[1].status = 2;
			_drawingStructs[1].transTableNum = 2;
		}
	}
}

void Encounter::updateDrawingStatus1(int32 rectIndex) {
	switch (rectIndex) {
	default:
		error("[Encounter::updateDrawingStatus1] Invalid rect index (%d)", rectIndex);
		break;

	case 0:
		if (checkKeywords2()) {
		    _drawingStructs[rectIndex].status = 1;
		    ++_drawingStructs[rectIndex].transTableNum;
	    }
		break;

	case 1:
		if (checkKeywords()) {
		    _drawingStructs[rectIndex].status = 1;
		    ++_drawingStructs[rectIndex].transTableNum;
	    }
		break;
	}
}

void Encounter::updateDrawingStatus2(int32 rectIndex) {
	switch (rectIndex) {
	default:
		error("[Encounter::updateDrawingStatus1] Invalid rect index (%d)", rectIndex);
		break;

	case 0:
		if (checkKeywords2()) {
		    _drawingStructs[rectIndex].status = 2;
		    --_drawingStructs[rectIndex].transTableNum;
			updateFromRect(rectIndex);
	    }
		break;

	case 1:
		if (checkKeywords()) {
		    _drawingStructs[rectIndex].status = 2;
		    --_drawingStructs[rectIndex].transTableNum;
			updateFromRect(rectIndex);
	    }
		break;
	}
}

bool Encounter::updateScreen() {
	if (getScene()->updateScreen())
		return true;

	getText()->loadFont(getWorld()->font1);

	if (!drawBackground()) {
		_isDialogOpen = false;
		return false;
	}

	if (!drawPortraits()) {
		_isDialogOpen = false;

		if (_shouldCloseDialog)
			drawStructs();

		return false;
	}

	if (_isDialogOpen) {

		if (!getSpeech()->getTextDataPos() && !getSpeech()->getTextData()) {

			drawDialog();
			updateDrawingStatus();
			drawStructs();

			if (_rectIndex == -1 && findRect() == -1)
				updateFromRect(-1);

			return false;
		}

		if (Config.showEncounterSubtitles) {
			drawText(getSpeech()->getTextDataPos(), getWorld()->font3, _point.y);
			drawText(getSpeech()->getTextData(), getWorld()->font1, _point.y);
		}

		if (_data_455BE0) {
			_data_455BE0 = false;

			// Check resources
			if (!_data_455BDC && !_objectId1 && !_objectId2 && !_actorIndex)
				error("[Encounter::updateScreen] Invalid encounter resources!");

			getSound()->playSound(getSpeech()->getSoundResourceId(), false, Config.voiceVolume);
		}

		return false;
	}

	if (_objectId3 || _data_455BE8)
		return false;

	if (!_data_455BF4)
		_data_455BF4 = 1;

	_isDialogOpen = true;

	return false;
}

void Encounter::updatePalette1() {
	error("[Encounter::updatePalette1] Not implemented!");
}

void Encounter::updatePalette2() {
	error("[Encounter::updatePalette2] Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Scripts
//////////////////////////////////////////////////////////////////////////
void Encounter::initScript(ResourceId resourceId) {
	_scriptData.reset(resourceId);
	_flag3 = false;
}

Encounter::ScriptEntry Encounter::getScriptEntry(ResourceId resourceId, uint32 offset) {
	ResourceEntry *entry = getResource()->get(resourceId);

	return ScriptEntry(entry->getData(offset));
}

void Encounter::runScript() {
	_isScriptRunning = true;
	bool done = false;

	do {
		ScriptEntry entry = getScriptEntry(_scriptData.resourceId, _scriptData.offset);

		debugC(kDebugLevelEncounter, "[Encounter] %s", entry.toString().c_str());

		switch (entry.opcode) {
		default:
			break;

		case kOpcodeReturn:
			_isScriptRunning = false;
			done = true;
			_value1 = 0;
			break;

		case kOpcodeSetScriptVar:
			_scriptData.vars[entry.param1] = getVariableInv(entry.param2);
			break;

		case kOpcode2:
			_scriptData.counter = _scriptData.vars[entry.param1] - getVariableInv(entry.param2);
			break;

		case kOpcode3:
			_scriptData.offset = entry.param2 - 1;
			break;

		case kOpcode4:
			if (_scriptData.counter >= 0)
				break;

			_scriptData.offset = entry.param2;
			break;

		case kOpcode5:
			if (_scriptData.counter > 0)
				break;

			_scriptData.offset = entry.param2;
			break;

		case kOpcode6:
			if (_scriptData.counter)
				break;

			_scriptData.offset = entry.param2 - 1;
			break;

		case kOpcode7:
			if (!_scriptData.counter)
				break;

			_scriptData.offset = entry.param2;
			break;

		case kOpcode8:
			if (_scriptData.counter < 0)
				break;

			_scriptData.offset = entry.param2;
			break;

		case kOpcode9:
			if (_scriptData.counter <= 0)
				break;

			_scriptData.offset = entry.param2 - 1;
			break;

		case kOpcode10:
			if (entry.param1)
				_item->keywords[findKeyword(_item, entry.param2)] &= ~kEncounterArray2000;
			else
				_item->keywords[findKeyword(_item, entry.param2)] |= kEncounterArray8000;
			break;

		case kOpcode11:
			if (entry.param1)
				_item->keywords[findKeyword(_item, entry.param2)] |= kEncounterArray2000;
			else
				_item->keywords[findKeyword(_item, entry.param2)] &= ~kEncounterArray8000;
			break;

		case kOpcode12:
			_items[entry.param1].keywords[findKeyword(&_items[entry.param1], entry.param2)] |= kEncounterArray4000;
			_items[entry.param1].keywords[findKeyword(&_items[entry.param1], entry.param2)] |= kEncounterArray8000;

			// Original saves the item back here
			break;

		case kOpcode13:
			if (!_flag3)
				_shouldCloseDialog = true;

			done = true;
			break;

		case kOpcode14:
			resetSpeech(_item->keywordIndex, getVariableInv(entry.param2));

			done = true;
			break;

		case kOpcode15:
			setVariable(entry.param2, (int16)_scriptData.vars[entry.param1]);
			break;

		case kOpcode16:
			_scriptData.vars[entry.param1] += entry.param2;
			break;

		case kOpcode17:
			switch (getVariable(3)) {
			default:
				break;

			case 13:
			case 15:
				if (_actorIndex) {
					getScene()->getActor(_actorIndex)->hide();
				} else {
					getWorld()->getObjectById(_objectId1)->disable();
					getWorld()->getObjectById(_objectId2)->disable();
				}

				if (getVariable(3) == 13)
					break;
				// Fallback to next case

			case 14:
				getScene()->getActor()->hide(); // Hide player
				break;

			case 16: {
				Object *obj = getWorld()->getObjectById((ObjectId)getVariableInv(entry.param2));
				obj->setNextFrame(obj->flags);
				_data_455BF4 = 1;
				done = true;
				}
				break;

			case 17:
				getWorld()->getObjectById((ObjectId)getVariableInv(entry.param2))->setNextFrame(32);
				_data_455BF4 = 1;
				done = true;
				break;

			case 18:
				getWorld()->getObjectById((ObjectId)getVariableInv(entry.param2))->setNextFrame(512);
				_data_455BF4 = 1;
				done = true;
				break;
			}

			if (!done) {
				_objectId3 = (ObjectId)getVariableInv(entry.param2);
				getWorld()->getObjectById(_objectId3)->setNextFrame(8);
				getCursor()->hide();
				done = true;
			}
			break;

		case kOpcode18:
			if (entry.param1)
				getScene()->getActor()->removeReactionHive(getVariableInv(entry.param2), _scriptData.vars[1]);
			else
				getScene()->getActor()->addReactionHive(getVariableInv(entry.param2), _scriptData.vars[1]);
			break;

		case kOpcode21:
			_scriptData.counter = getScene()->getActor()->hasMoreReactions(getVariableInv(entry.param2), _scriptData.vars[1]) ? 0 : 1;
			break;

		case kOpcode23:
			if (!getSharedData()->getMatteBarHeight()) {
				getScreen()->loadPalette();
				getSharedData()->setMatteBarHeight(1);
				getSharedData()->movieIndex = (uint16)getVariableInv(entry.param2);
				getSharedData()->setMatteVar1(1);
				getSharedData()->setMattePlaySound(true);
				getSharedData()->setMatteInitialized(true);
				getSharedData()->setMatteVar2(0);
				done = true;
			}

			if (!_data_455BE8) {
				_data_455BE8 = true;
				getCursor()->hide();
			}
			break;

		case kOpcode24:
			if (entry.param1)
				_vm->setGameFlag((GameFlag)getVariableInv(entry.param2));
			else
				_vm->clearGameFlag((GameFlag)getVariableInv(entry.param2));
			break;

		case kOpcode25:
			_scriptData.counter = _vm->isGameFlagSet((GameFlag)getVariableInv(entry.param2)) ?  1 : 0;
			break;
		}

		++_scriptData.offset;

	} while (!done);
}

}
