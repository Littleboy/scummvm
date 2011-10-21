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

#ifndef RING_PILGRIM2_APPLICATION_H
#define RING_PILGRIM2_APPLICATION_H

#include "ring/base/application.h"

namespace Ring {

class ZoneSystemPilgrim2;
class Zone1Pilgrim2;
class Zone2Pilgrim2;
class Zone3Pilgrim2;
class Zone4Pilgrim2;
class Zone5Pilgrim2;

class ApplicationPilgrim2 : public Application {
public:
	ApplicationPilgrim2(RingEngine *engine);
	~ApplicationPilgrim2();

	//////////////////////////////////////////////////////////////////////////
	// Initialization
	virtual void initFont();
	virtual void setup();
	virtual void initZones();
	virtual void initBag();
	virtual void initData();

	//////////////////////////////////////////////////////////////////////////
	// Startup & Menu
	virtual void showStartupScreen();
	virtual void startMenu(bool savegame);
	virtual void showMenu(ZoneId zone, MenuAction menuAction);
	virtual void showCredits();
	virtual void startGame();

	//////////////////////////////////////////////////////////////////////////
	// Drawing
	virtual void draw();

	//////////////////////////////////////////////////////////////////////////
	// Event handlers
	virtual void onMouseLeftButtonUp(const Common::Event &evt, bool isControlPressed);
	virtual void onMouseLeftButtonDown(const Common::Event &evt);
	virtual void onMouseRightButtonUp(const Common::Event &evt);
	virtual void onKeyDown(Common::Event &evt);
	virtual void onTimer(TimerId id);

private:
	// Event handlers
	ZoneSystemPilgrim2 *_zoneSystem;
	Zone1Pilgrim2      *_zone1;
	Zone2Pilgrim2      *_zone2;
	Zone3Pilgrim2      *_zone3;
	Zone4Pilgrim2      *_zone4;
	Zone5Pilgrim2      *_zone5;
};

} // End of namespace Ring

#endif // RING_PILGRIM2_APPLICATION_H
