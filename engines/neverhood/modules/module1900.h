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

#ifndef NEVERHOOD_MODULES_MODULE1900_H
#define NEVERHOOD_MODULES_MODULE1900_H

#include "neverhood/neverhood.h"
#include "neverhood/module.h"
#include "neverhood/scene.h"
#include "neverhood/modules/module1200.h"

namespace Neverhood {

class Module1900 : public Module {
public:
	Module1900(NeverhoodEngine *vm, Module *parentModule, int which);
	virtual ~Module1900();
protected:
	int _sceneNum;
	uint32 handleMessage(int messageNum, const MessageParam &param, Entity *sender);
	void createScene(int sceneNum, int which);
	void updateScene();
};

// Scene1901

class Scene1901 : public Scene {
public:
	Scene1901(NeverhoodEngine *vm, Module *parentModule, int which);
};

// Scene1907

class Scene1907;

class AsScene1907Symbol : public AnimatedSprite {
public:
	AsScene1907Symbol(NeverhoodEngine *vm, Scene1907 *parentScene, int elementIndex, int positionIndex);
	void moveUp();
	void moveDown();
	void fallOff(int newPositionIndex, int fallOffDelay);
	bool isPluggedIn() { return _isPluggedIn; }
	bool isMoving() { return _isMoving; }
protected:
	Scene1907 *_parentScene;
	int _elementIndex;
	int _currPositionIndex;
	int _newPositionIndex;
	bool _isPluggedIn;
	bool _isMoving;
	int _someX, _someY;
	int _xBreak;
	int _currStep;
	int _yAccel;
	int _yIncr;
	int _fallOffDelay;
	int _deltaX, _smallDeltaX;
	int _deltaY, _smallDeltaY;
	// Dumb, change if possible
	static bool _plugInFailed;
	static int _plugInTryCount;
	void update();
	uint32 handleMessage(int messageNum, const MessageParam &param, Entity *sender);
	uint32 hmTryToPlugIn(int messageNum, const MessageParam &param, Entity *sender);
	void suTryToPlugIn();
	void suFallOff();
	void suFallOffHitGround();
	void suMoveDown();
	void suMoveUp();
	void tryToPlugIn();
	void stFallOffHitGround();
	void cbFallOffHitGroundEvent();
	void stPlugIn();
	void stPlugInFail();
};

class AsScene1907WaterHint : public AnimatedSprite {
public:
	AsScene1907WaterHint(NeverhoodEngine *vm);
	void show();
protected:
	void update();
	uint32 hmShowing(int messageNum, const MessageParam &param, Entity *sender);
	void hide();
};

class SsScene1907UpDownButton : public StaticSprite {
public:
	SsScene1907UpDownButton(NeverhoodEngine *vm, Scene1907 *parentScene, AsScene1907Symbol *asScene1907Symbol);
	void setToUpPosition();
	void setToDownPosition();
protected:
	Scene1907 *_parentScene;
	AsScene1907Symbol *_asScene1907Symbol;
	int _countdown1;
	void update();
	uint32 handleMessage(int messageNum, const MessageParam &param, Entity *sender);
};

class Scene1907 : public Scene {
public:
	Scene1907(NeverhoodEngine *vm, Module *parentModule);
	void plugInFailed();
	void setPositionFree(int index, bool value) { _positionFree[index] = value; }
	int getNextPosition() { return _pluggedInCount++; }
protected:
	AsScene1907Symbol *_asSymbols[9];
	SsScene1907UpDownButton *_ssUpDownButton;
	AsScene1907WaterHint *_asWaterHint;
	int _currMovingSymbolIndex;
	int _pluggedInCount;
	int _moveDownCountdown;
	int _moveUpCountdown;
	int _countdown3;
	bool _hasPlugInFailed;
	bool _positionFree[9];
	void update();
	uint32 handleMessage(int messageNum, const MessageParam &param, Entity *sender);
	int getRandomPositionIndex();
};

} // End of namespace Neverhood

#endif /* NEVERHOOD_MODULES_MODULE1900_H */
