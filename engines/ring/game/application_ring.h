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

#ifndef RING_APPLICATION_RING_H
#define RING_APPLICATION_RING_H

#include "ring/base/application.h"

namespace Ring {

class ApplicationRing : public Application {
public:
	ApplicationRing(RingEngine *engine);
	~ApplicationRing();

	//////////////////////////////////////////////////////////////////////////
	// Initialization
	virtual void setup();
	virtual void initZones();

	//////////////////////////////////////////////////////////////////////////
	// Startup & Menu
	virtual void showStartupScreen();
	virtual void startMenu(bool savegame);
	virtual void showMenu(Zone zone, uint32 a2);
	virtual void showCredits();

	//////////////////////////////////////////////////////////////////////////
	// Drawing
	virtual void draw();

	//////////////////////////////////////////////////////////////////////////
	// Setup
	void setupZone(Zone zone, uint32 a2);
	void setZone(Zone zone, uint32 a2);
	void setZoneAndEnableBag(Zone zone);

	//////////////////////////////////////////////////////////////////////////
	// Zone name, short string and readFrom
	virtual Common::String getZoneString(Zone zone) const;
	virtual Common::String getZoneLongName(Zone zone) const;
	virtual ArchiveType getReadFrom(Zone zone) const;

private:
	//////////////////////////////////////////////////////////////////////////
	// Zone initialization
	//////////////////////////////////////////////////////////////////////////
	void initZoneSY();
	void initZoneNI();
	void initZoneRH();
	void initZoneFO();
	void initZoneRO();
	void initZoneWA();
	void initZoneAS();
	void initZoneN2();
};

} // End of namespace Ring

#endif // RING_APPLICATION_RING_H
